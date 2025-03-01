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

#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <eeregs.h>
#include "pdriver.h"

#define COMMAND	"ls -l"

int main(void) {
	int  nbytes;
	char buffer[BUFSZ];

	printf("DECI2 Sample Start !!!\n");

	memset(buffer, 0, BUFSZ);
	if (InitProtocolDriver() < 0) {
		printf("InitProtocolDriver() failed\n");
		return 1;
	}

	if ((nbytes = SystemOnHost(COMMAND, buffer)) < 0) {
		printf("SystemOnHost() failed\n");
		return 1;
	}

	printf("Result of \'%s\':\n%s\n", COMMAND, buffer);
	return 0;
}

// Local variables:
// tab-width: 4
// End:
// vi:set tabstop=4:
