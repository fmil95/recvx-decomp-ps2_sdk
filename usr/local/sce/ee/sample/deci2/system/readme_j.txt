[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

DECI2 �v���g�R����p�����z�X�g��� system() �֐������s����T���v��


<�T���v���̉��>
	���̃v���O�����́ADECI2 �v���g�R����p���ă^�[�Q�b�g���
	�v���O��������z�X�g��� system() �֐������s���A���̌��ʂ�
	�\������T���v���v���O�����ł��B

<�t�@�C��>
	main.c        : �^�[�Q�b�g���̃��C���v���O����
	pdriver.c     : �T���v���v���g�R���h���C�o�v���O����
	pdriver.h     : �T���v���v���g�R���h���C�o�w�b�_
	linux/main.c  : �z�X�g���̃��C���v���O����
	linux/deci2.h : DECI2 �v���g�R���w�b�_
	linux/dcmp.h  : DCMP �v���g�R���w�b�_
	linux/netmp.h : NETMP �v���g�R���w�b�_

<�N�����@>
	$ make		�F�R���p�C��
	
	�R���p�C����A
	$ cd linux
	$ ./hsys -d �^�[�Q�b�g���i�܂��́AIP�A�h���X�j
	�����s���A���̃E�B���h�E���J����
	
	$ make run
	
	���邢��
	$ dsedb
	> run main.elf

<�R���g���[���̑�����@>
	�Ȃ�

<���l>
	�����`�����T���v���v���g�R���́A�v���g�R���ԍ� = 0xe000 �Ƃ��A
	DECI2 �w�b�_�Ɉ��������ȉ��̂悤�Ȍ`���̃p�P�b�g�ƂȂ�܂��B

       3                   2                   1                  
     1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                            Result                             |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
                                                          Data ...  |
                                                       -+-+-+-+-+-+-+

	Result 
	  �^�[�Q�b�g�v���O��������̃R�}���h���s�v���̂Ƃ��� 0 ���A
	  �z�X�g����̉����̎��ɂ� system() �̖߂�l��ݒ肵�܂��B

	Data
	  null-terminated ���ꂽ�C�ӂ̃f�[�^�ł��B
	  �^�[�Q�b�g�v���O��������̃R�}���h���s�v���̂Ƃ��̓R�}���h������
	  ���A�z�X�g����̉����̎��ɂ� system() �Ŏ��s���ꂽ�R�}���h�̕W��
	  �o�͌��ʂ�ݒ肵�܂��B

<�����T��>
	�����̊T���́A�ȉ��̒ʂ�ł��B

	�z�X�g��:
	1. dsnetm �Ƃ� TCP/IP ���x���̃R�l�N�V�������m������
	2. netmp �v���g�R���� CONNECT ���b�Z�[�W��p���āA
	   deci2 ���x���̃R�l�N�V�������m������
	3. �^�[�Q�b�g����̗v��������܂Ŗ����ɑ҂�
	4. �^�[�Q�b�g����̃R�}���h���s�v����������A�R�}���h�����s���A
	   ���̌��ʂ��e���|�����t�@�C���Ɉ�U�Z�[�u���Ă���A�^�[�Q�b�g��
	   �ԐM����

	�^�[�Q�b�g��:
	1. �v���g�R���h���C�o��o�^����
	2. �R�}���h���z�X�g�ɑ���
	3. �R�}���h�̌��ʂ��Ԃ��Ă���܂Ŗ����ɑ҂�
	4. ���ʂ��Ԃ��Ă�����\������ 

