[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

VU1��p���������ϊ��̃T���v��

<�T���v���̉��>
	���̃v���O�����́AVU1�œ����ϊ��������s���T���v���v���O�����ł��B

<�t�@�C��>
	sample.c	���C���v���O����
	work.dsm	�I�u�W�F�N�g�f�[�^(DMA packet)
	work.vsm	VU1�}�C�N���v���O����

<�N�����@>
	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run sample.elf

<�R���g���[���̑�����@>
	�Ȃ�

<���l>
	�����̊T���́A�ȉ��̗l�ɂȂ��Ă��܂��B

	1. �\���������I�u�W�F�N�g�̉�]�}�g���N�X��p�ӂ���(RotMatirx())
	2. DMA�p�P�b�g(work.dsm)�̉�]�}�g���N�X������������(SetMatrix())
	3. �������ł���DMA�p�P�b�g��path1��DMA�]������B
	4. �O�t���[���̕`��y��VSync��҂�(sceGsSyncPath(), sceGsSyncV())
	5. 2�ɖ߂�

	����
	work.vsm�͉ǐ��ׁ̈A�œK������Ă���܂���B
