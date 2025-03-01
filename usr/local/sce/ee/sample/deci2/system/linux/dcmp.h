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

#ifndef _DCMP_H
#define _DCMP_H

typedef struct {
	u_char type;		/* Message Type */
	u_char code;		/* Message Code */
	u_short unused;		/* (unused) */
} DCMP_HDR;

/* Message Type */
#define DCMP_TYPE_CONNECT	0	/* connect/disconnect req/rep */
#define DCMP_TYPE_ECHO		1	/* echo req/rep */
#define DCMP_TYPE_STATUS	2	/* status is changed */
#define DCMP_TYPE_ERROR		3	/* error report */

/* Message Code for DCMP_TYPE_STATUS */
#define DCMP_CODE_CONNECTED	0	/* no-connect -> connect */
#define DCMP_CODE_PROTO		1	/* no-proto -> proto */
#define DCMP_CODE_UNLOCKED	2	/* locked -> unlocked */
#define DCMP_CODE_SPACE		3	/* no-space -> space */
#define DCMP_CODE_ROUTE		4	/* no-route -> route */

typedef struct {
	u_short proto;			/* protocol/node-id */
} DCMP_STATUS_DATA;

/* Message Code for DCMP_TYPE_ERROR */
#define DCMP_CODE_NOROUTE	0	/* no route */
#define DCMP_CODE_NOPROTO	1	/* protocol unreachable */
#define DCMP_CODE_LOCKED	2	/* locked */
#define DCMP_CODE_NOSPACE	3	/* no space */
#define DCMP_CODE_INVALHEAD	4	/* invalid header */
#define DCMP_CODE_NOCONNECT	5	/* not connected */

typedef struct {
	DECI2_HDR orig_hdr;	/* original DECI2 header */
	u_char orig_data[16];	/* 1st 128bits of original datagram */
} DCMP_ERROR_DATA;

#endif _DCMP_H

// Local variables:
// tab-width: 4
// End:
// vi:set tabstop=4:
