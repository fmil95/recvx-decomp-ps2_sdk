/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* $Id: stdio.h,v 1.4 1999/10/25 06:21:24 tei Exp $ */

/*
 *                     I/O Processor System Services
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         stdio.h
 *                         like ansi stdio
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.00           1999/10/12      tei
 */

#ifndef	_STDIO_H
#define	_STDIO_H
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

#ifndef EOF
#define EOF (-1)
#endif

extern int  open(char *filename, int flag);
extern int  close(int fd);
extern long lseek(int fd, long offset, int whence);
extern int  read(int fd, void *buf, long nbyte);
extern int  write(int fd, void *buf, long nbyte);
extern int  ioctl(int fd, long request, long arg);

extern int  sprintf(char *buf, const char *fmt, ...);
extern int  printf(const char *fmt, ...);
extern int  getchar();
extern char *gets(char *s);
extern int  putchar(int c);
extern int  puts(char *s);

extern int  fdprintf(int fd, const char *format, ...);
extern int  fdgetc(int fd);
extern char *fdgets(char *buf, int fd);
extern int  fdputc(int c, int fd);
extern int  fdputs(char *s, int fd);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif	/* _STDIO_H */


