/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* $Id: string.h,v 1.3 2000/02/08 09:17:58 tei Exp $ */

/*
 *                     I/O Processor System Services
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         string.h
 *                         string functions pseudo definition header 
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.00           1999/10/12      tei
 */

#ifndef	_STRING_H
#define	_STRING_H

#define LMAX 256

#ifndef _TYPES_H
#include <sys/types.h>
#endif
#include <memory.h>

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif
extern char *strcat (char *, char *);
extern char *strncat(char *, char *, int);
extern int   strcmp (/* char *, char * */);	/* To avoid conflicting */
extern int   strncmp(char *, char *, int);
extern char *strcpy (/* char *, char * */);	/* To avoid conflicting */
extern char *strncpy(char *, char *, int);
extern int   strlen (/* char * */);		/* To avoid conflicting */
extern char *index  (char *, char);
extern char *rindex (char *, char);

extern char *strchr (char *, char);
extern char *strrchr(char *, char);
extern char *strpbrk(char *, char *);
extern int   strspn (char *, char *);
extern int   strcspn(char *, char *);
extern char *strtok (char *, char *);
extern char *strstr (char *, char *);
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif	/* _STRING_H */

