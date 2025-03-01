/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 1.6
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                       - mpeg csc on vu1 -
 *
 *                         Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            util.c
 *                      utility functions
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.10           12.17.1999      umemura     the first version
 *       0.20           01.17.2000      umemura     vu1
 */
#include <stdio.h>
#include <string.h>
#include <sifdev.h>
#include "defs.h"
#include "vobuf.h"

// ///////////////////////////////////////////////////////////////
//
// Load modules
//
void loadModule(char *moduleName)
{
    while (sceSifLoadModule(moduleName, 0, "") < 0) {
        printf("Cannot load '%s'\n", moduleName);
    }
    printf("load '%s'\n", moduleName);
}

// ////////////////////////////////////////////////////////////////
//
// Open a file and get its size
//
int openFile(char *filename, int *filesize)
{
    int fd;
    int ret;

    // //////////////////////////////////////
    //
    //  open file to read and check its size
    //
    fd = sceOpen(filename, SCE_RDONLY);
    if (fd < 0) {
    	printf("Cannot open '%s'\n", filename);
	return fd;
    }
    *filesize = sceLseek(fd, 0, SCE_SEEK_END);
    if (*filesize < 0) {
    	printf("sceLseek() fails (%s): %d\n", filename, *filesize);
	sceClose(fd);
	return -1;
    }

    ret = sceLseek(fd, 0, SCE_SEEK_SET);
    if (ret < 0) {
    	printf("sceLseek() fails (%s)\n", filename);
	sceClose(fd);
	return -1;
    }

    return fd;
}

