[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

4bit �e�N�X�`��clut�̈ꎞ�o�b�t�@�ւ̃��[�h�� csa/cld�t�B�[���h�ɂ����
���䂷��T���v��

<�T���v���̉��>
        ���̃v���O�����́A4bit �e�N�X�`��clut�̈ꎞ�o�b�t�@�ւ̃��[�h��
        csa/cld�t�B�[���h�ɂ���Đ��䂷��T���v���ł��B
        �S�r�b�gCLUT�͂P�U���Ɉꎞ�o�b�t�@�Ƀ��[�h����A���ꂪ�R�g
        ���炩���ߗp�ӂ���Ă��܂��B

<�t�@�C��>
        clut_csa.c      �F���C���v���O����
        tex4.dsm        �F4 bit �e�N�X�`���f�[�^�ACLUT (48�j�f�[�^

<�N�����@>
        % make          �F�R���p�C��
        % make run      �F���s

        �R���p�C����A�ȉ��̕��@�ł����s�\
        % dsedb
        > run clut_csa.elf

<�R���g���[���̑�����@>
	�Ȃ�

<�����T��>
        �����̊T���́A�ȉ��̒ʂ�ł��B

        1. �`����ADMA�̏�����
        2. 4�C���f�b�N�X�C���[�W��GS�t���[���o�b�t�@�ւ̃��[�h(My_indeximg4	      )
        3. clut�̃t���[���o�b�t�@�ւ̃��[�h(My_clut4_1, 4_2, 4_3)
        4. �`��p�P�b�g�̏�����
        5. �_�u���o�b�t�@�̃X���b�v
        6. �t���[�����̃p�P�b�g�̐ݒ�
        6-1. �P�U�̂S�r�b�gCLUT���P�̂W�r�b�gCLUT�Ƃ��Ĉ�x�Ɉꎞ�o�b�t	        �@�Ƀ��[�h���邽�߂̐ݒ�B�O�̃t���[���Ɠ����S�r�b�gCLUT��		     �g�ł���΁i����CBP�ł���΁j���[�h����Ȃ��B
        6-2. CLUT�o�b�t�@����CLUT�����[�h���邱�ƂȂ��Acsa�t�B�[���h�̎���              �I�t�Z�b�g�̒l�ɂ���ăJ�����g�t���[���̕`��ɗp����4bitCLUT��             �w�肷��(0-15)�B
        7. �X�v���C�g��`�悷��
        8. 5.�ɂ��ǂ�

