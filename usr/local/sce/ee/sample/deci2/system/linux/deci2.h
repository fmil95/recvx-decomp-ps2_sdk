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

#ifndef _DECI2_H
#define _DECI2_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	u_short length;		/* Packet Length (includes this header) */
	u_short reserved;	/* (reserved) */
	u_short protocol;	/* Protocol */
	u_char  source;		/* Source Node ID */
	u_char  destination;	/* Destination Node ID */
} DECI2_HDR;

/* length */
#define DECI2_MAX_LEN		(0x10000 - 1)

/* Node ID (source, destination) */
#define DECI2_NODE_IOP		'I'	/* (0x49) IOP */
#define DECI2_NODE_EE		'E'	/* (0x45) EE */
#define DECI2_NODE_HOST		'H'	/* (0x48) Host */

/* protocol */
#define DECI2_PROTO_DCMP	0x0001	/* Control Message */
#define DECI2_PROTO_NETMP	0x0400	/* Net Manager */

/* 0x0000-0x0fff SCE use */
/* 0x1000-0xdfff Tool licensee use */
/* 0xe000-0xefff Local use for licensee */
/* 0xf000-0xffff reserved */

#ifdef __cplusplus
}
#endif

#endif _DECI2_H

// Local variables:
// tab-width: 4
// End:
// vi:set tabstop=4:
