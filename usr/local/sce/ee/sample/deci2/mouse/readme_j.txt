[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

DECI2 �v���g�R����p�����v���O�����̃_�E�����[�h�A
                      �y�уz�X�g��̃}�E�X�̓������V�~�����[�g����T���v��


<�T���v���̉��>
	���̃v���O�����́ADECI2 �v���g�R����p���ăv���O�����̃_�E�����[�h
	���s���A�_�E�����[�h�����v���O�����Ƀz�X�g��̃}�E�X�̓��������W�l
	�Ƃ��Ă������ă}�E�X�̓������[���I�ɃV�~�����[�g����T���v���v���O
	�����ł��B

<�t�@�C��>
	main.c        : �^�[�Q�b�g���̃��C���v���O����
	vu0.c         : �s�񉉎Z�֐�
	pdriver.c     : �T���v���v���g�R���h���C�o�v���O����
	pdriver.h     : �T���v���v���g�R���h���C�o�w�b�_
	linux/main.c  : �z�X�g���̃��C���v���O����
	linux/main.h  : �w�b�_�t�@�C��
	linux/child.c : �q�v���Z�X�v���O����
	linux/deci2.h : DECI2 �v���g�R���w�b�_
	linux/dbgp.h  : DBGP �v���g�R���w�b�_
	linux/dbgp.c  : DBGP �v���O����
	linux/dcmp.h  : DCMP �v���g�R���w�b�_
	linux/dcmp.c  : DCMP �v���O����
	linux/elf.c   : ELF ��̓��[�`��
	linux/list.h  : ���X�g����w�b�_
	linux/list.c  : ���X�g���샋�[�`��
	linux/netmp.h : NETMP �v���g�R���w�b�_
	linux/netmp.c : NETMP �v���O����

<�N�����@>
	$ make		: �R���p�C��
	$ make run	: ���s
	
	���邢�́A�R���p�C����A
	$ linux/mmouse main.elf

<�R���g���[���̑�����@>
	�Ȃ�

<���l>
	�����`�����T���v���v���g�R���́A�v���g�R���ԍ� = 0xe000 �Ƃ��A
	DECI2 �w�b�_�Ɉ��������ȉ��̂悤�Ȍ`���̃p�P�b�g�ƂȂ�܂��B
	���̃T���v�������삷��ɂ�X-Window ���N������Ă���K�v������܂��B         

       3                   2                   1                  
     1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               x                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               y                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               b                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	x 
	  x ���W�l

	y
	  y ���W�l

	b
	  button ���

<�����T��>
	�����̊T���́A�ȉ��̒ʂ�ł��B

	�z�X�g��:
	1. dsnetm �Ƃ� TCP/IP ���x���̃R�l�N�V�������m������
	2. netmp �v���g�R���� CONNECT ���b�Z�[�W��p���āA
	   deci2 ���x���̃R�l�N�V�������m������
	3. �^�[�Q�b�g�̃��Z�b�g
	4. EE �Ƃ̃R�l�N�V�����̊m����҂�
	5. �v���O�����̃_�E�����[�h
	6. �^�[�Q�b�g���̃v���O�������V�����v���g�R�����I�[�v������̂�҂� 	   7. �}�E�X�C�x���g������������A��L�p�P�b�g�𑗐M


	�^�[�Q�b�g��:
	1. �v���g�R���h���C�o��o�^����
	2. �z�X�g����̃}�E�X�̏������ɕ`��

