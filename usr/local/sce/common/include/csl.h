/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *                     I/O Processor Library
 *                          Version 0.60
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                            csl.h
 *                     Component Sound Library
 *
 *     Version   Date          Design     Log
 *  --------------------------------------------------------------------
 *     0.60      Oct.12.1999   katayama   first checked in.
 */
#ifndef _csl_h_
#define _csl_h_
typedef struct {
    int  	sema;	
    void	*buff;
} sceCslBuffCtx;

typedef struct {
	int				buffNum;
	sceCslBuffCtx	*buffCtx;
} sceCslBuffGrp;

typedef struct {
	int			buffGrpNum;	//�o�b�t�@�O���[�v���@�@�@�@�@�F�K�{
	sceCslBuffGrp*		buffGrp;	//�o�b�t�@�O���[�v�ւ̃|�C���^�F�K�{
	void*			conf;		//���W���[���ݒ�\���́@�@�@�@�F�C��
	void*			callBack;	//�R�[���o�b�N�֐��@�@�@�@�@�@�F�C��
	char**			extmod;		//�O�����W���[���̃G���g���@�@�F�C��
} sceCslCtx;

#endif //!_csl_h_
