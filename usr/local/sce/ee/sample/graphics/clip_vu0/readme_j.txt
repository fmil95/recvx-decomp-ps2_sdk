[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Clipping �T���v�� (VU0)

<�T���v���̉��>
	���̃v���O�����́A27�̗����̂�\�����A�o�E���_���[�{�b�N�X��
	�p���ĕ\���̈�ɂ��I�u�W�F�N�g�P�ʂ̃N���b�s���O�A�f�r�̕`��
	�̈�ɂ��|���S���P�ʂ̃N���b�s���O���s�����Ƃ��ł��܂��B
	�o�E���_���[�{�b�N�X�̃N���b�s���O�𗝉����₷���悤�ɕ\���̈�
	��苷���͈͂ɃN���b�s���O�̈��ݒ肵�Ă��܂��B
	�t�@�[�N���b�v�܂��̓j�A�N���b�v�ʂ����u�����f�B���O�ɂ��\
	�����A���̐ؑւ����s�����Ƃ��\�ł��B

<�t�@�C��>
        main.c          : ���C���v���O����
        vu0.c 	        : ���W�ϊ��A�����ϊ����̍s�񉉎Z�֐�
        cube.s          : �I�u�W�F�N�g�f�[�^(������)
        torus1.s        : �I�u�W�F�N�g�f�[�^(�g�[���X �f�[�^����)
        flower.dsm      : �e�N�X�`���f�[�^

<�N�����@>
	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����́A�ȉ��̕��@�ł����s�\
	% dsedb
	> run main.elf

<�R���g���[���̑�����@>
	����(�����L�[)	: x����](�I�u�W�F�N�g)
	����		: y����](�I�u�W�F�N�g)
	L1L2�{�^��	: z����](�I�u�W�F�N�g)

	���~�{�^��	: x����](���_)
	�����{�^��	: y����](���_)
	R1R2�{�^��	: z���ړ�(���_)
	���X�e�B�b�N    : x��,y���ړ�(���_)

	SELECT�{�^��	: �I�u�W�F�N�g�̐ؑւ�(CUBE/TORUS)
	START�{�^��	: �I�u�W�F�N�g�N���b�v��ON/OFF
	���X�e�C�b�N    : ���X�e�B�b�N���������ނ��ƂŁAGS�`��̈�ɂ��
			  �N���b�v��ON/OFF �̐ؑւ����s�Ȃ��܂�(*1) 
	�E�X�e�B�b�N    : �E�X�e�B�b�N���������ނ��ƂŁA�I�u�W�F�N�g�N���b				  �v�ʂ̐ؑւ����s���܂�(NEAR/FAR)

<���l>
	���̃T���v���� ANALOG���[�h�X�C�b�`���AON�ɂ�����ԂŎ��s����
	���������B
	(*1) �I�u�W�F�N�g�N���b�v OFF �̏�ԂŁAR2 �{�^�����g���ăI�u
	�W�F�N�g����ʂ���啝�ɂ͂ݏo���AGS �̕`��̈�𒴂��邭�炢��
	���܂��B���̏�Ԃō��X�e�B�b�N�� ON �ɂ���ƁA�͂ݏo�����|���S��
	���N���b�s���O����܂��B

