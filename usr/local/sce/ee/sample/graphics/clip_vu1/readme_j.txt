[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Clipping(Backface&Viewvolume) �T���v��(VU1)

<�T���v���̉��>
        VU1��p���ă|���S���� Backface Clipping�� View-volume Clipping
	���s���T���v���ł��B�T���v���ł� Triangle�� Triangle strip��
	�����ɑΉ����Ă��܂��B�����͂��ׂă|���S���P�ʂōs���A�V�U�����O
	���͎������Ă��܂���B���������āA�N���b�s���O�̈���꒸�_�ł�
	�͂ݏo���|���S���͌����Ă��܂��܂��B��ʏ�̔������N���b�s���O
	�̈��\���A�R���g�[���[���[�ő���\�ł��B
	�{�����̗̈�́AGS�̃N���b�s���O�̈�Ɠ���(�}2048)�ɂ��Ă����A
	���̗̈���|���S�����͂ݏo�Ȃ��悤�ɂ���̂��ړI�ł��B�܂��A
	���_�̗����ɍs���Ă��܂����|���S�����\�����Ȃ��悤�ɂ��܂��B

<�t�@�C��>
        main.c          : ���C���v���O����
	main.h          : ���C���v���O�����̃v���g�^�C�v�錾
	defines.h	: �e�� .c �t�@�C���ŗp����v���g�^�C�v�錾
        vu1pack.c       : Vu1�]���p�p�P�b�g�쐬�֐�
        cube.dsm        : Vu1�]���p�p�P�b�g(�f�[�^)�t�@�C��(������)
        torus.dsm       : Vu1�]���p�p�P�b�g(�f�[�^)�t�@�C��(�h�[�i�c)
	basic.dsm       : Vu1�}�C�N���R�[�h�����[�h����t�@�C��
	basic.vsm       : �N���b�s���O���s��Ȃ��`��}�C�N���R�[�h
	clip.dsm        : Vu1�}�C�N���R�[�h�����[�h����t�@�C��
        clip.vsm        : Backface&View-volume Clipping����������
			  �`��}�C�N���R�[�h
        clip_opt.vsm    : clip.vsm�̍œK����
        tex128.dsm      : �e�N�X�`���f�[�^
        tex64.dsm	: �e�N�X�`���f�[�^

<�N�����@>
        % make          �F�R���p�C��
        % make run      �F���s

        �R���p�C����A�ȉ��̕��@�ł����s�\
        % dsedb
        > run main.elf

<�R���g���[���̑�����@>
        ����(�����L�[)  : x����](���_)
        ����		: y����](���_)
        L1L2�{�^��      : z���ړ�(���_�C�O��)
        ���~�{�^��      : �N���b�s���O�̈�ύX(y����)
        �����{�^��      : �N���b�s���O�̈�ύX(x����)
        R1R2�{�^��      : �N���b�s���O�̈�ύX(xy����)
        SELECT�{�^��    : ���f���̐؂�ւ��i�����́C�g�[���X�j
	START�{�^��	: �N���b�s���O������ON/OFF

<�����T��>
        �����̊T���́A�ȉ��̒ʂ�ł��B

        ===== main.c =====
        main():

        1. �_�u���o�b�t�@�̐ݒ�A�e�N�X�`���̃��[�h
        2. pad�I�[�v���A�}�C�N���R�[�h�̓]��
        3. �u�����f�B���O�W���̐ݒ�A�ό`�p�m�C�Y���z�̏�����
        4. �p�b�h����̏���ǂݎ��
	5. VU1�]���p�p�P�b�g�쐬
        6. VU1�Ƀp�P�b�g��]�����ĕ`��
	7. 4.�ɖ߂�

	SetViewScreenClipMatrix():
	�e���_���N���b�v���W�n�Ɏˉe���邽�߂̃}�g���N�X(view_clip)
	���v�Z����B�����ɓ����ϊ��p�̃}�g���N�X(view_screen)�����߂�B
	�N���b�v���W�n�Ƃ́A�r���[�{�����[����Ίp�l(-1.0, -1.0, -1.0)
	��(1.0, 1.0, 1.0)�̗����̂Ɏˉe�������W�n�ł���B���������āA
	�ˉe��̍��W���Βl1.0�͈̔͂Ŋe���Ƃ��N���b�s���O���鎖��
	�\�ƂȂ�B

	SetGifClipLine():
	�N���b�s���O�̈�̋��E����`�悷�邽�߂�GIF�p�P�b�g���쐬����֐��B
	�����ł�Vu0�Œ��ړ����ϊ����A���̏��GIF�̃p�P�b�g�f�[�^���쐬
	���Ă���B


        ===== vu1packet.c =====
	vu1�]���p�̃p�P�b�g�𐶐����邽�߂̊֐��Q

	Vu1DmaPacket�\����:
	Vu1�ɓ]������p�P�b�g�𐶐��A�Ǘ�����\���́B
	�e��I�u�W�F�N�g�f�[�^���ꊇ���ĊǗ����A�K�v�ŏ�����DMAKICK��
	�ςނ悤�ȃp�P�b�g�𐶐�����B�܂��A�p�P�b�g�f�[�^���_�u��
	�o�b�t�@�œ���(�쐬)����悤�݌v����Ă���B

	InitVu1DmaPacket():
	Vu1DmaPacket�\���̂̏������֐��B�����ɂ͂Q�̊Ǘ��e�[�u��
	(�p�P�b�g)�ւ̃|�C���^���n�����B����̓p�P�b�g�̃_�u��
	�o�b�t�@�̂��߂ł���B

	GetVu1ObjHead():
	���ݗL���ȃp�P�b�g�̐擪�A�h���X��Ԃ��B

	AddVu1ObjPacket():
	.dsm�t�@�C���ŃR���p�C������A��������ɑ��݂���I�u�W�F�N�g
	�f�[�^(����VU1�p�p�P�b�g�`���ɂȂ��Ă���)��ǉ��o�^����B

	CloseVu1ObjPacket():
	���ݗL���ȃp�P�b�g���I�������A�_�u���o�b�t�@�̃X�C�b�`���s���B
	�Ԃ�l�ɗL���ȃp�P�b�g�̃A�h���X��Ԃ����߁ADMAKICK�Ɏg�p�ł���B

	GetVu1EnvHead():
	VU1�Ƀ}�g���N�X�f�[�^���v���Z�b�g���邽�߂̍\���̂ւ�
	�|�C���^��Ԃ��B

	CloseVu1EnvPacket():
	VU1�Ŏg�p����}�g���N�X�f�[�^(�p�P�b�g)����A�p�P�b�g�ւ�
	�|�C���^��Ԃ��B


        ==== clip.vsm(VU1 micro code) ====

        [clip.vsm]

        1. �e��}�g���N�X�����W�X�^VF01 - VF16�ɐݒ� & precalculate
	2. �`�摮���̔���(Triangle or TriangleStrip)�t���O���Z�b�g
        3. �e��J�E���^�Z�b�g�i�X�g���b�v���_���Ȃǁj
        4. GIF�^�O�̃��[�h�E�X�g�A�ARGBAQ, ST, XYZF�̃X�g�A
           �|�C���^�̐ݒ�
	5. ���_�𓧎��ϊ�
	6. �@���x�N�g���ƃ��C�g�x�N�g���̓��ς��v�Z
	7. ���_���N���b�v���W�Ɏˉe���t���O��ݒ�(VI01)
	   (XYZ�Ƃ���Βl1.0�͈̔́ACLIP���߂ɂ��ߋ�3�_�𔻒�)
	8. BackfaceClip�����t���O��ݒ�(VI12)
	   (VF22-VF23�ɉߋ�3�_�����ϊ����W���Ǘ��B
	    �ʂ̖@���x�N�g�������ߐ����𔻒肷��B
	    TriangleStrip�͐��������݂Ɍ��o����邽�߁A
	    �����𔽓]���Ȃ��猟�o����B)
	9. ��̃t���O�̘a���Ƃ��ĕ���BXYZ2,XYZ3�������
	   �L�b�N���邩����B
	10. �@���x�N�g���Ƃ̓��ς��璸�_�̐F�����߂�B
	11. ���_���������[�v����(5.�ɖ߂�)
	12. �I�u�W�F�N�g�̃u���b�N���������[�v����B
	    (4.�ɖ߂�B�_�u���o�b�t�@�X�C�b�`)
	13. �I�u�W�F�N�g���������[�v����B(2.�ɖ߂�)


