[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�����̈��k�f�[�^���f�R�[�h����T���v��
<�T���v���̉��>
	�{�T���v���� sample �f�B���N�g���̒��� ipu/ezcube �̃T���v����
	�g���������̂ł��B
	�����̈��k���ꂽ�f�[�^�� IPU��1��1�f�R�[�h���A�e�N�X�`���o�b
	�t�@��DMA�]�����A�e�N�X�`���Ƃ��ĕ\�����Ă��܂��B
	�܂�IPU�����DMA�I�����荞�݂�p���āA1�̃f�[�^�̃f�R�[�h��
	�I����������Ɏ��̃f�R�[�h���s���Ă���A�����ǂ�IPU�𗘗p����
	���܂��B
	
<�t�@�C��>
	main.c
	ldimage.c
	ezcube.c
	ezcube.h
	ezmovie.h
	ez.ipu
	rugby.ipu
	456.ipu
	1_6.ipu
	wada.ipu
	yama.ipu

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

