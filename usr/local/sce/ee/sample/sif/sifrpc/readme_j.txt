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
	main.c

<�N�����@>
	% dsreset 0 0
	% make		�F�R���p�C��

	�R���p�C����Adsedb ���N�����܂��B
	% dsedb

	���ɁA�ʂ̃E�B���h�E���J���AIOP ���T���v��
        (iop/sample/sif/sifrpc) ��� dsidb ���N�����܂�

	EE�T���v�����E�B���h�E�ɂ�����
	> run main.elf
	�����s���܂�

	IOP�T���v�����E�B���h�E�ɂ�����
	> mstart iopmain.irx
	�����s���܂�

	�T�[�o���A�������f�[�^�� 0xff000000 ���}�X�N����
	�N���C�A���g�֑���Ԃ��Ă���ΐ���I���ł��B

<�R���g���[���̑�����@>
	�Ȃ�

<���l>


