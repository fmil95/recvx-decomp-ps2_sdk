[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

SIF RPC �v���g�R���T���v��

<�T���v���̉��>
	SIF RPC �v���g�R���̃T���v���v���O�����ł��B

	EE/IOP �ŃT�[�o�E�N���C�A���g�̓�������܂��B�Е���
	#define SERVER �𐶂����Ă��������B�����炪�T�[�o�ɂȂ�܂��B

	�N���C�A���g�ŁA�f�[�^���쐬���T�[�o�ɑ���܂��B
	�T�[�o�́A�󂯎�����f�[�^�����H���ăN���C�A���g�ɑ���Ԃ��܂��B

	�������̃��[�h�ŁA�T�[�o���R�[�����܂��B

<�t�@�C��>
	iopmain.c

<�N�����@>
	% dsreset 0 0
	% make		�F�R���p�C��

	�R���p�C����Adsidb ���N�����܂��B
	% dsidb

	���ɁA�ʂ̃E�B���h�E���J���AEE ���T���v��
        (ee/sample/sif/sifrpc) ��� dsedb ���N�����܂�

	IOP�T���v�����E�B���h�E�ɂ�����
	> mstart iopmain.irx
	�����s���܂�

	EE�T���v�����E�B���h�E�ɂ�����
	> run main.elf
	�����s���܂�

	�T�[�o���A�������f�[�^�� 0xff000000 ���}�X�N����
	�N���C�A���g�֑���Ԃ��Ă���ΐ���I���ł��B

<�R���g���[���̑�����@>
	�Ȃ�

<���l>
	����̃v���O�����́AEE ���Ƃقړ���ɂ��Ă���܂��B
