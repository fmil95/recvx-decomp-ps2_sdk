[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

VU1��p���������ϊ��A�����v�Z�̃T���v��

<�T���v���̉��>
	���̃v���O�����́AVU1�œ����ϊ��ƌ����v�Z�������ɍs�����߂̃T
	���v���v���O�����ł��B

<�t�@�C��>
	sample.c	�F���C���v���O����
	devinit.c	�F�f�o�C�X�̏��������[�`��
	tex.h		�F�e�N�X�`���f�[�^
	work.dsm	�F�I�u�W�F�N�g�f�[�^(DMA packet)
	work.vsm	�FVU1�}�C�N���v���O����

<�N�����@>
	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run sample.elf

<�R���g���[���̑�����@>
	���{�^��	�F�O������
	���{�^��	�F�������
	select�{�^��    �F��~�i�����Ă���ԁj 

<���l>
	�����̊T���́A�ȉ��̒ʂ�ł��B

	1. path1 DMA�p�P�b�g�̈�Q�p��(dpmem[2])
	2. �I�u�W�F�N�g�`��ɕK�v�ȃ}�g���b�N�X���ASPRAM��ɗp��
           (SetMatrix())�B
	3. DMA�p�P�b�g�̈��2�̃}�g���b�N�X��]��(SetMatrix())�B
	4. �O�t���[���̕`��y��VSync��҂�(sceGsSyncPath(), sceGsSyncV())
	5. �������ł���DMA�p�P�b�g��path1��DMA�]��
	6. 2�ɖ߂�

