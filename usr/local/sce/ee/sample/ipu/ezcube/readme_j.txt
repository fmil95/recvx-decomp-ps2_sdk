[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

3�����|���S����IPU��p���ăf�R�[�h�����e�N�X�`���𒣂����T���v��

<�T���v���̉��>
	sample �f�B���N�g���̒��� pkt/cube �̃T���v����
	ipu/ezmovie �̃T���v�����e�N�X�`���Ƃ��Ē��������̂ł��B
	����Ɋւ��Ă�cube�̃T���v�����A����Ɋւ��Ă�ezmovie��
	�T���v�����Q�Ƃ��Ă��������B
	�{�T���v���ł�IPU�Ńf�R�[�h�����f�[�^���e�N�X�`���o�b�t�@��
	DMA�]�����A�e�N�X�`���Ƃ��ĕ\�����Ă��܂��B
		
<�t�@�C��>
	main.c
	ldimage.c
	ezcube.c
	ezcube.h
	1_6.ipu
	ezmovie.h

<�N�����@>
	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run main.elf

<�R���g���[���̑�����@>
	��,���{�^��	�Fx����]
	��,���{�^��	�Fy����]
	L1,L2�{�^��	�Fz����]
	R1,R2�{�^��	�F�����̂�z�������̈ړ�
	��,�~�{�^��	�F�J������x����]
	��,���{�^��	�F�J������y����]
	START�{�^��	�F���Z�b�g

