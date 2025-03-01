/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* $Id: loadcore.h,v 1.12 2000/06/27 12:32:52 tei Exp $ */

/*
 *                     I/O Processor System Services
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         loadcore.h
 *                         Module manager
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.00           1999/10/12      tei
 */

#ifndef _LOADCORE_H
#define _LOADCORE_H

#define RESIDENT_END	0
#define NO_RESIDENT_END	1
#define FAREWELL_END	1

typedef struct _libhead {
    struct _libhead	*next;
    struct _libcaller	*client;
    unsigned short	version;
    unsigned short	flags;
    char		name[8];
} libhead ;

#define LIBMAGIC	(0x41e00000)
typedef struct _libcaller {
    unsigned long	magic;
    struct _libcaller	*client;
    unsigned short	version;
    unsigned short	flags;
    char		name[8];
} libcaller;

typedef struct _moduleinfo {
    char		*name;
    unsigned short	version;
} ModuleInfo;

typedef struct _libinfo {
    unsigned long	dummy[2];
    unsigned short	version;
    unsigned short	dummy2;
    char		name[8];
} LibInfo;

void FlushIcache();
void FlushDcache();

int RegisterLibraryEntries(libhead *lib);
int RegisterNonAutoLinkEntries(libhead *lib);
void *QueryLibraryEntryTable(LibInfo *libinfo);

#endif /* _LOADCORE_H */
