[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved
                                                               Aug. 2000

graphics framework general sample

<�T���v���̉��>

	�{�T���v���� sample/graphics/framework/ �J�e�S���ɂ����铝��
	�O���t�B�b�N�X���́A��{�R���Z�v�g�Ɗe��@�\�𐷂荞�񂾁A��������
	�\�z��ڎw���܂��B
	���� sample/graphics/framwork/ ���̍\���̏[���ɔ����A�����o�[�W����
	�A�b�v���Ă����\��ł��B

	graphics framework �̊T�v�ɂ��ẮA�ʃt�@�C���̃h�L�������g
	over_j.txt ���������������B

<�t�@�C��>
	*.h             : �v���g�^�C�v�錾�փb�_�[

	anim.c		: �A�j���[�V�����p�֐��Q
	camera.c	: �J�����p�����[�^��ݒ肷��֐��Q
	eval.c		: ��Ԍv�Z�̊֐��Q
	framebuf.c	: �e�N�X�`���p�t���[���o�b�t�@�̈�Ǘ��̊֐��Q
	gmain.c		: �f�[�^�̏������E�ǂݍ��݁E�X�V���s���֐��Q
	hier.c		: ���f���̊K�w�\������ы��L�|���S�������̊֐��Q
	main.c		: ���C���v���O����
	packbuf.c	: �p�P�b�e�C���O�̃o�b�t�@�̈���m�ۂ��邽�߂̊֐��Q
	vu1frame.c	: ���f���f�[�^�Q���f�[�^�x�[�X�Ǘ����邽�߂̊֐��Q

	micro.dsm	: �}�C�N���R�[�h�̃��x�������Ǘ�����t�@�C��
	vu1base.vsm	: ���s����*3 ��VU1�}�C�N���R�[�h
	vu1baseo.vsm	: ���s����*3 ��VU1�}�C�N���R�[�h(�œK����)
	vu1cull.vsm	: ���s����*3 �� Culling ���s�Ȃ�VU1�}�C�N���R�[�h
	vu1cullo.vsm	: ���s����*3 �� Culling ���s�Ȃ�VU1�}�C�N���R�[�h
			  (�œK����)

	dino.s		: raw�t�H�[�}�b�g�f�[�^
	logo.s		: raw�t�H�[�}�b�g�f�[�^
	APEWALK39.s	: �T���v���f�[�^�i�K�w�E�A�j���[�V�����j

	dino.hmd	: HMD�f�[�^
	logo.hmd	: HMD�f�[�^
	APEWALK39.hmd	: LightWave 3D �`���[�g���A���f�[�^��HMD�f�[�^

	�{�f�B���N�g���Ɋ܂܂��A���f���f�[�^����уV�[���f�[�^�́A�č� NewTek
	�Ђ�LightWave 3D 5.6 �ɂ��z�z����Ă���f�[�^��������Ѓf�B�E�X�g�[��
	�ЊJ����Animation Saver v1.8 �ŃG�N�X�|�[�g���܂����B

<�N�����@>

	% make		  �F�R���p�C��
	% make run	  �F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run main.elf


<�R���g���[���̑�����@>

	���� (�����L�[ �j	:	�J����x����]
	���� (�����L�[ �j	:	�J����y����]
	L1L2 �{�^��   		:	�J����z����]
	���~ �{�^��   		:	�J�����㉺�ړ�(y����)
	���� �{�^��   		:	�J�������E�ړ�(x����)
	R1R2 �{�^��   		:	�J�����O��ړ�(z����)
	START�{�^��   		:	VU1�}�C�N���R�[�h�؂�ւ�
	SELECT�{�^��		:	�J�������Z�b�g

<�����T��>
	�����̊T���́A�ȉ��̒ʂ�ł��B

	g_init();	������

			1. GS�_�u���o�b�t�@������
			2. DMA�p�P�b�g�o�b�t�@������
			3. �J�����p�����[�^������
			4. VU1�}�C�N���R�[�h�}�l�[�W��������

	g_read_model();	�f�[�^�ǂݍ���

			1. �f�[�^�̏�����
			2. �f�[�^�̓ǂݍ���
			3. �f�[�^�o�b�t�@����
			4. DMA�p�P�b�g����
			5. �o�b�t�@�I�[�o�t���[�`�F�b�N

	g_main();	���C�����[�v

			1. PAD���̓ǂݍ���
			2. �O�����v�Z(animation,hierarchy,share)
			3. DMA�f�[�^�t���[���X�g�쐬
			4. DMA�]��
			5. �X�e�[�^�X�\��
			6. 1 �ɖ߂�

	g_finish();	�I������

			1. DMA�p�P�b�g�o�b�t�@���
			2. �f�[�^�o�b�t�@���
			3. �ϐ��̏�����
			4. PAD�̏�����

<���l>
	�f�[�^�t�H�[�}�b�g�̏ڍׂɂ��Ă�
	eS �p�b�P�[�W���� doc/es2raw_j.txt���Q�Ƃ��Ă��������B

<���������E���ӎ���>

	�풓�E��풓�e�N�X�`���̐؂�ւ��̓��[�U�[���g���t���O��ݒ肷��
	�K�v������܂��B
	�f�t�H���g�͔�풓�e�N�X�`���A�t���O0�ɂȂ��Ă��܂��B
	�풓�e�N�X�`���t���O��ݒ肷��ꍇ��es2raw_j.txt���Q�Ƃ��ăt���O��1
	�ɐݒ肵�ăf�[�^�ɔ��f�����Ă��������B
	�܂��A���ꂼ��̃x�[�X�|�C���^��#define�ŌŒ肵�Ă��܂��B
	�x�[�X�|�C���^��ύX����ꍇ��framebuf.h���Q�Ƃ��Ă��ꂼ��̃x�[�X
	�|�C���^��ݒ肵�Ă��������B

	�K�w�\���̃}�g���N�X�X�^�b�N�T�C�Y��30�܂łƂȂ��Ă��܂��B
	�X�^�b�N�T�C�Y��ύX����ꍇ��hier.c���Q�Ƃ��ăX�^�b�N�T�C�Y��ݒ肵��
	���������B

	�K�w�\������уA�j���[�V�����̉�]������ LightWave3D�ɍ�����YXZ�̏���
	�Ȃ��Ă��܂��B
	�ύX����ꍇ��hier.c���Q�Ƃ��ĉ�]������ݒ肵�Ă��������B
	����͉�]�����̓f�[�^�ɔ��f����\��ł��B

	�^�C�����O�e�N�X�`���ɂ͑Ή����Ă���܂���B

-----------------------------------------------------------------------
���W�Ɋւ��钍�ӏ���
-----------------------------------------------------------------------
LightWave 3D(TM)�́A�č�NewTek�Ђ̏��W�ł��B

