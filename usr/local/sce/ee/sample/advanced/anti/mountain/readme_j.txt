[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

�A���`�G�C���A�V���O���C���㏑���ɂ��
�A���`�G�C���A�V���O�̃T���v���v���O�����i�y�\�[�g�����j

<�T���v���̉��>
	���̃v���O�����ł́AAA1���C���㏑�� �ɂ��A���`�G�C���A�V���O
	���ʂƂ��̎g������������܂��B

	�|���S���G�b�W��AA1���C�����㏑�����邱�Ƃɂ��A���`�G���A�V���O
	���ʂ𓾂邱�Ƃ��ł��܂��B���̃T���v���v���O�����ł͂Q������
	���b�V���f�[�^�𐶐�����ƂƂ��Ƀ��C���f�[�^���������㏑�����܂��B

	����ɂ��V���G�b�g�����̃G�C���A�V���O�̌y���A���b�V����LOD��
	�ω��_�ł�T�������܂̌y�����ł��܂��B

<�t�@�C��>
	
	Makefile	�Fmake file
	clip.c		�F�N���b�v���[�`��
	devinit.c	�F�f�o�C�X�̏��������[�`��
	dma_util.c	�FDMA���[�`��
	gterrain.c	�F�n�`�������[�`��
	gterrain_packet.c�@�F�n�`�p�P�b�g�������[�`��
	main.c		�F���C���v���O����
	matrix_util.c	�F�}�g���N�X�v�Z���[�`��
	misc_packet.c	�F���̑��̃p�P�b�g�������[�`��
	random.c	�F�������[�`��
	texload.c	�F�e�N�X�`�����[�h���[�`��
	view_util.c	�F���_����}�g���N�X�v�Z���[�`��
	terrain.h	�F�\����
	constant.h	�F�萔
	scale.h		�F�X�P�[���Ɉˑ�����萔
	testconst.h	�FVU1�}�C�N���v���O�����p�萔
	*.raw		�F�e�N�X�`���t�@�C��
	work.dsm	�FDMA packet
	fsub.vsm	�FVU1�}�C�N���v���O����

<�N�����@>
	% make		�F�R���p�C��

	�R���p�C����A�ȉ��̕��@�Ŏ��s�\
	% dsedb
	> run main.elf

<�R���g���[���̑�����@>
	���i�����L�[�j	: �O�i
	���@�@		: ��i
	���@�@�@	: �E�i
	���@�@�@	: ���i
	���{�^��	: �E��]
	���{�^��	: ����]
	���{�^��	: ����]
	�~�{�^��	: ���]
	R1�{�^��	: ��������
	R2�{�^��	: �ڋ�
	L1�{�^��	: �オ��
	L2�{�^��	: ������

	SELECT�{�^��+
	R1�{�^��	: �|���S��OFF
	L1�{�^��	: �|���S��ON
	L2�{�^��	: AA1���C��OFF	
	R2�{�^��	: AA1���C��ON
	
<�v���O�����̉��> 
	�v���O�����̃������[�}�b�v���̉���ɂ��܂��Ă� mountain.ppt 
	�t�@�C�����Q�Ƃ��Ă�������
