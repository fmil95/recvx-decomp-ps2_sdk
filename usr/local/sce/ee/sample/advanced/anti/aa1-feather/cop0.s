/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                       - feather aa1 sample  -
 *
 *                         Version 0.2.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : cop0.s
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.1.0           Oct,07,1999     sasaki    modified from feather
 *      0.2.0           Jul,11,2000     sasaki    
 */
/*
 * cache control functions
 */
	.set at
	.set noreorder
/*
 * u_int BranchPredictOn(void)
 */
	.global	BranchPredictOn
BranchPredictOn:
	li	$8, 0x00001000

	mfc0	$9, $16
	por	$10, $8, $9
	mtc0	$10, $16

	j	$31
	nop

/*
 * u_int BranchPredictOff(void)
 */
	.global	BranchPredictOff
BranchPredictOff:
	li	$8, 0xffffefff

	mfc0	$9, $16
	pand	$10, $8, $9
	mtc0	$10, $16

	j	$31
	nop

/*
 * u_int DoubleIssueOn(void)
 */
	.global	DoubleIssueOn
DoubleIssueOn:
	li	$8, 0x00040000

	mfc0	$9, $16
	por	$10, $8, $9
	mtc0	$10, $16

	j	$31
	nop

/*
 * u_int DoubleIssueOff(void)
 */
	.global	DoubleIssueOff
DoubleIssueOff:
	li	$8, 0xfffbffff

	mfc0	$9, $16
	pand	$10, $8, $9
	mtc0	$10, $16

	j	$31
	nop

