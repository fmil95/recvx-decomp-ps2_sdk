/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
// Version 0.00
// ASCII
// 
// Copyright (C) 1998-1999 Sony Computer Entertainment Inc. All Rights Reserved.
// 
// Version        Date            Design      Log
// --------------------------------------------------------------------
// 0.00           06/18/99        koji        1st version

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/param.h>

#include "deci2.h"
#include "netmp.h"
#include "dcmp.h"

#define DECI2_DEFAULT_PORT	"8510"
#define DECI2_DEFAULT_TIMEOUT		10

#define PROTO_SAMPLE	0xe000

static char *baseName;
static int waitingNetmpConnectr = 0;

static int Usage(int f_true){
	if (!f_true) return(0);
	printf("Usage: %s [-d <host>[:<port>]] \n", baseName);
	exit(1);
}


static int ShowError(char *mes) {
	printf("%s\n", mes); return -1;
}


static void MakeDeci2Hdr(DECI2_HDR *hdr,u_short l,u_short p,u_char s,u_char d) {
	hdr->length      = l;
	hdr->reserved    = 0;
	hdr->protocol    = p;
	hdr->source      = s;
	hdr->destination = d;
}


static int PacketComing(int fd, struct timeval *timeout) {
	int n;
	fd_set rfds;

	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);
	if ((n = select(FD_SETSIZE, &rfds, NULL, NULL, timeout)) < 0) {
		if (errno == EINTR) {
			printf("EINTR\n");
			return 0;
		}
		return ShowError("select() faild.");
	}
    if (!n) return 0;
    if (!FD_ISSET(fd, &rfds)) return ShowError("select() unknown fd.");
	return 1;
}


static int SendPacket(int fd, u_char *buf, int len) {
	return write(fd, buf, len);
}


static int RecvPacket(int fd, u_char *buf) {
    int n;
	int len = sizeof(DECI2_HDR);
    DECI2_HDR *hdr = (DECI2_HDR *)buf;

	if ((n = read(fd, buf, len)) < 0) return ShowError("read() failed");
	if (n != len) return ShowError("read() failed.");

	if ((n = read(fd, buf+len, hdr->length-len)) < 0)
		return ShowError("read() failed");
	if (n != hdr->length-len) return ShowError("read() failed.");

	return hdr->length;
}


static void ProcessNetmp(char *buf){
	NETMP_HDR *hdr = (NETMP_HDR *)buf;

	switch (hdr->code) {
		case NETMP_CODE_RESETR:
			printf("target resetting.\n");
			break;
		case NETMP_CODE_CONNECTR:
			if(hdr->result != NETMP_ERR_OK) {
				printf("cannot connect to dsnetm.\n");
				exit(1);
			}
			waitingNetmpConnectr = 0;
			printf("Connected.\n");
			break;
		default:
			printf("unknown netmp packet received.\n");
			break;
	}
}


static void ProcessDcmp(char *buf){
	DCMP_HDR *hdr = (DCMP_HDR *)buf;
	DCMP_STATUS_DATA *id;

	switch (hdr->type) {
		case DCMP_TYPE_STATUS:
			printf("DCMP_TYPE_STATUS: ");
			id = (DCMP_STATUS_DATA *)(hdr + 1);
			switch (hdr->code) {
				case DCMP_CODE_CONNECTED: printf("CONNECTED"); break;
				case DCMP_CODE_PROTO:     printf("PROTO");     break;
				case DCMP_CODE_UNLOCKED:  printf("UNLOCKED");  break;
				case DCMP_CODE_SPACE:     printf("SPACE");     break;
				default:                  printf("UNKNOWN");   break;
			}
			printf(" proto/node=0x%x", id->proto);
			break;
		case DCMP_TYPE_ERROR:
			printf("DCMP_TYPE_ERROR: ");
			switch (hdr->code) {
				case DCMP_CODE_NOPROTO:   printf("NOPROTO");   break;
				case DCMP_CODE_LOCKED:    printf("LOCKED");    break;
				case DCMP_CODE_NOSPACE:   printf("NOSPACE");   break;
				case DCMP_CODE_INVALHEAD: printf("INVALHEAD"); break;
				case DCMP_CODE_NOCONNECT: printf("NOCONNECT"); break;
				default:                  printf("UNKNOWN");   break;
			}
			break;
		default:
			printf("Unknown DCMP message");
			break;
	}
	printf(" Received.\n");
}


static void ProcessSample(int fd, char *buf){
	FILE *tfile;
	char *tname;
	char c;
	u_char *data;
	u_int *result;
	u_int len = 0;
	DECI2_HDR *hdr = (DECI2_HDR *)buf;

	result = (u_int *)(hdr + 1);
	data = (u_char *)(result + 1);

	if ((tname = tmpnam(NULL)) == NULL) {
		printf("cannot create temporary file\n");
		exit(1);
	}

	printf("Received Command: %s\n", data);
	strcat(data, ">");
	strcat(data, tname);
	*result = system(data);

	tfile = fopen(tname, "r");
	while ((c = fgetc(tfile)) != EOF) data[len++] = c;
	data[len++] = '\0';
	fclose(tfile);
	if (unlink(tname) < 0) printf("cannot remove temporary file\n");

	printf("Result of Command:\n %s", data);

	len += sizeof(DECI2_HDR) + sizeof(result);
    MakeDeci2Hdr(hdr, len, hdr->protocol, hdr->destination, hdr->source);
    SendPacket(fd, buf, len);
}


static int ProcessPacket(int fd, char *buf) {
	DECI2_HDR *pkt = (DECI2_HDR *)buf;

	switch (pkt->protocol) {
		case DECI2_PROTO_NETMP:
			ProcessNetmp((char *)(pkt + 1));
			break;
		case DECI2_PROTO_DCMP:
			ProcessDcmp((char *)(pkt + 1));
			break;
		case PROTO_SAMPLE:
			ProcessSample(fd, buf);
			break;
		default:
			break;
	}
}


static int SendNetmpConnect(int fd) {
	struct {
		DECI2_HDR deci2;
		NETMP_HDR netmp;
		NETMP_PROTOS protos;
	} pkt;

    MakeDeci2Hdr(&pkt.deci2, sizeof(pkt), DECI2_PROTO_NETMP,
							DECI2_NODE_HOST, DECI2_NODE_HOST);
    pkt.netmp.code = NETMP_CODE_CONNECT;
    pkt.netmp.result = 0;
	pkt.protos.pri = NETMP_PRI_DEFAULT;
	pkt.protos.reserved = 0;
	pkt.protos.proto = PROTO_SAMPLE;

    return SendPacket(fd, (u_char *)&pkt, sizeof(pkt));
}


static int ConnectNet(char *arg) {
	int fd;
    char *pos;
    char hostname[MAXHOSTNAMELEN + 1];
    char *strport = DECI2_DEFAULT_PORT;
    u_long addr, port;
    struct hostent *hp;
    struct sockaddr_in server;

	if (arg == NULL || !*arg) {					// arg indicates "host:port"
		if (gethostname(hostname, sizeof(hostname)) < 0)
			return ShowError("gethostname() fail");
	} else if ((pos = strchr(arg, ':')) != NULL) {
		strncpy(hostname, arg, pos - arg);
		if (*(pos + 1)) strport = pos + 1;
	} else {
		if (strlen(arg) > sizeof(hostname))
			return ShowError("argument larger than expect");
		strcpy(hostname, arg);
	}

    bzero((char *)&server, sizeof(server));
	if ((addr = inet_addr(hostname)) == -1) {
		if ((hp = gethostbyname(hostname)) == NULL)
			return ShowError("gethostbyname() fail");
		addr = *(u_long *)hp->h_addr;
		server.sin_family = hp->h_addrtype;
	} else {
		server.sin_family = AF_INET;
	}

    if (sscanf(strport, "%d", &port) != 1 || !port)	// get port number
		return ShowError("invalid port number");

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)	// create socket
		return ShowError("socket() fail");

    server.sin_addr.s_addr = addr;
    server.sin_port = htons(port);
	// connect to host
    if (connect(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
		return ShowError("connect() fail");

	return fd;
}


int main(int ac, char *av[]) {
	int r, fd;
	char *p;
	char *hostname = getenv("DSNETM");
    struct timeval timeout;
	static char buffer[DECI2_MAX_LEN];

	baseName = ((p = strrchr(av[0], '/')) != NULL) ? (p+1) : (av[0]);

	for (--ac, ++av; 0 < ac && *av[0] == '-'; --ac, ++av) {
		if (!strcmp("-d", av[0])) Usage(--ac <= 0), hostname = *++av;
		else Usage(1);
	}

	Usage(0 < ac);

	if ((fd = ConnectNet(hostname)) < 0) {
		printf("%s: cannot connect to host\n", baseName);
		return 1;
	}

	if (SendNetmpConnect(fd) < 0) {
		printf("%s: faild to send connect packet\n", baseName);
		return 1;
	}
	waitingNetmpConnectr = 1;

    timeout.tv_sec = DECI2_DEFAULT_TIMEOUT;
    timeout.tv_usec = 0;

	while ((r = PacketComing(fd, &timeout)) >= 0) {
		if (r == 0) {
			if (waitingNetmpConnectr) {
				printf("cannot get netmp connect replay - timeout\n");
				return 1;
			}
			timeout.tv_sec = DECI2_DEFAULT_TIMEOUT;
			timeout.tv_usec = 0;
		}
		if (r == 1) {
			while (RecvPacket(fd, buffer) > 0) ProcessPacket(fd, buffer);
		}
		if (r < 0) return 1;
	}

	return 0;
}

// Local variables:
// tab-width: 4
// End:
// vi:set tabstop=4:
