/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
typedef struct {
	float	pos[65][65][4] __attribute__((aligned(16)));	
	float	nml[65][65][4] __attribute__((aligned(16)));
	float	stq[65][65][4] __attribute__((aligned(16)));
	float	bblk[8][4] __attribute__((aligned(16)));
	float	cent[4] __attribute__((aligned(16)));
	float	mount_max;
	float	mount_sharp;
	float	tedge;
	int	nmesh;
	int	sdiv;
	int	clip;
	int	cutoff[65][65];
} mesh ;

typedef struct {
	float	pos[10][10][4] __attribute__((aligned(16)));	
	float	nml[10][10][4] __attribute__((aligned(16)));
	float	stq[10][10][4] __attribute__((aligned(16)));
	float	bblk[8][4] __attribute__((aligned(16)));
	float	cent[4] __attribute__((aligned(16)));
	float	mount_max;
	float	mount_sharp;
	float	tedge;
	int	sdiv;
	int	clip;
	int	cutoff[9][9];
} mesh8 ;

typedef struct {
	float	pos[17][17][4] __attribute__((aligned(16)));	
	float	nml[17][17][4] __attribute__((aligned(16)));
	float	stq[17][17][4] __attribute__((aligned(16)));
	float	bblk[8][4] __attribute__((aligned(16)));
	float	cent[4] __attribute__((aligned(16)));
	float	mount_max;
	float	mount_sharp;
	float	tedge;
	int	sdiv;
	int	clip;
	int	cutoff[17][17];
} mesh16 ;

typedef struct {
	float	pos[33][33][4] __attribute__((aligned(16)));	
	float	nml[33][33][4] __attribute__((aligned(16)));
	float	stq[33][33][4] __attribute__((aligned(16)));
	float	bblk[8][4] __attribute__((aligned(16)));
	float	cent[4] __attribute__((aligned(16)));
	float	mount_max;
	float	mount_sharp;
	float	tedge;
	int	sdiv;
	int	clip;
	int	cutoff[33][33];
} mesh32 ;

typedef struct {
	float	cent[8][8][4] __attribute__((aligned(16)));
	float	pos[10][10][4] __attribute__((aligned(16)));	
	float	nml[10][10][4] __attribute__((aligned(16)));
	float	stq[10][10][4] __attribute__((aligned(16)));
	float	bblk[8][4] __attribute__((aligned(16)));
	float	mount_max;
	float	mount_sharp;
	float	tedge;
	int	sdiv;
	int	clip[8][8];
	int	cutoff[9][9];
} bigmesh ;
