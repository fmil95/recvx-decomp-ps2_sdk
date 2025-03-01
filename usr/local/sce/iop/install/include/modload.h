/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* $Id: modload.h,v 1.2 1999/10/30 09:57:38 tei Exp $ */

/*
 *                     I/O Processor System Services
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         modload.h
 *                         Module loader
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.00           1999/10/12      tei
 */

#ifndef _MODLOAD_H
#define _MODLOAD_H

int  LoadStartModule(const char *filename,
		     int args, const char *argp, int *result);

int  LoadModule(const char *filename);
int  StartModule(int modid, const char *filename,
		 int args, const char *argp, int *result);

void ReBootStart(const char *filename, int flags);

/* flags */
#define RBS_no_init_buscon	(1<<0)
#define RBS_no_init_cachecon	(1<<1)

#endif /* _MODLOAD_H */
