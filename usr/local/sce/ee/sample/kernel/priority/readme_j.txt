[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�X���b�h�D��x�̃T���v��

<�T���v���̉��>

���̃v���O�����́A�X���b�h�̗D��x�𗝉����邽�߂̃T���v���ł��B

<�e�X���b�h����>

main �v���C�I���e�B0:
    ReadThread�APrintThread���쐬���ċN�����A���ꂼ���READY��Ԃɂ��āA
    �D��x��10�ɉ�����READY��Ԃֈڍs���܂��B

ReadThread �v���C�I���e�B1:
    dsedb �R���\�[���փX���b�hID���o�͂��A�t�@�C����ǂ݂Â��܂��B

PrintThread �v���C�I���e�B2:
    dsedb �R���\�[���փX���b�hID���o�͂��Â��܂��B

<�Z�}�t�H>

ReadEndSema -- ReadThread �t�@�C���̓ǂݍ��݂̏I���������Z�}�t�H

<�v���O�����̓���>

main����A�N�����ꂽReadThread�APrintThread��READY��Ԃɂ���܂��B

main�̗D��x��ReadThread��PrintThread���Ⴍ����ƁA�D��x������ReadThread
��RUN��ԂɂȂ�܂��BReadThread�́A�t�@�C���̃I�[�v���܂��͓ǂݍ��݂��s�Ȃ�
�ƁA�ꎞ�I��WAIT��ԂɂȂ�܂��BReadThread���AWAIT��ԂɑJ�ڂ���ƁA�D��x
�̒ႢPrintThread��RUN��ԂɈڍs���܂��B

ReadThread�̓t�@�C���̓ǂݍ��݂��I������ƁAReadEndSema��������A
PrintThread���I�������܂��BReadThread���I��������Amain���I�����܂��B


<�t�@�C��>

	priority.c

<�N�����@>

	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run priority.elf

<�R���g���[���̑�����@>

	�Ȃ�
