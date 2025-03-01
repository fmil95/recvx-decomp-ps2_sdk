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
// 0.00           10/13/99        koji        1st version

#include <eekernel.h>
#include <stdio.h>
#include <deci2.h>
#include "pdriver.h"

typedef struct {
	volatile int    s;			// socket descriptor
	volatile u_int  wlen;		// write len
	volatile u_int  rlen;
	u_char         *wptr;		// write buffer pointer
	u_char         *rptr;		// read  buffer pointer
	volatile int    rbytes;
} OPT;

static OPT opt;
static char wbuf[BUFSZ] __attribute__((aligned (64))); // must be 64 for cache
static char rbuf[BUFSZ] __attribute__((aligned (64))); // must be 64 for cache


static void EventHandler(int event, int param, OPT *opt) {
	int n;
	switch (event) {
		case DECI2_READ:
			if ((opt->rlen + param) > BUFSZ) {
				opt->rbytes = -1;
				return;
			}
			if ((n = sceDeci2ExRecv(opt->s, &opt->rptr[opt->rlen], param)) < 0)
				opt->rbytes = -1;
			opt->rlen += n;
			break;

		case DECI2_READDONE:
			opt->rbytes = opt->rlen;
			opt->rlen = 0;
		case DECI2_WRITE:
		case DECI2_WRITEDONE:
		case DECI2_CHSTATUS:
			break;

		case DECI2_ERROR:
		default:
			opt->rbytes = -1;
			break;
	}
}


int InitProtocolDriver(void) {
	opt.wptr = UNCACHED(wbuf);
	opt.rptr = UNCACHED(rbuf);
	opt.wlen = 0;
	opt.rlen = 0;
	opt.rbytes = 0;

	if ((opt.s = sceDeci2Open(PROTO_SAMPLE, &opt, (void *)EventHandler)) < 0)
		return 0;

	return 1;
}


void GetMouseInfo(int *xyb) {
	sceDeci2Hdr *hdr = (sceDeci2Hdr *)opt.rptr;
	int *p = (int *)(hdr + 1);

//	printf("GetMouseInfo x:%d, y:%d, b:%d\n", *p, *(p + 1), *(p + 2));
	xyb[0] = *p;
	xyb[1] = *(p + 1);
	xyb[2] = *(p + 2);
}

// Local variables:
// tab-width: 4
// End:
// vi:set tabstop=4:

