[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

�e�N�X�`���}�b�s���O�T���v��
+ LINE�ɂ��A���`�G�C���A�V���O�i�P�x�����Łj


<�T���v���̉��>
        �X�y�L�����[�̂���Ɠx�v�Z�A�e�N�X�`���}�b�s���O��
        �s���T���v���ɁAAA1 LINE �ɂ��A���`�G�C���A�V���O�@�\��
	�t�������B
	���炩�Ŗ��� CUBE ���ł����ꂢ�ɃX���[�W���O��������悤��
        AA1 LINE �\���� Backface Clip �������������B

<�t�@�C��>

        buff0.dsm       �F�e�N�X�`���]���̐ݒ���s���p�P�b�g
        buff1.dsm       �FTEX0_1���W�X�^�̐ݒ���s���p�P�b�g
        r.vsm           �F�X�y�L�����[�E���t���N�V�����}�b�s���O�p
                          �}�C�N���R�[�h�i�œK���Ȃ��j
        r_opt.vsm       �F�X�y�L�����[�E���t���N�V�����}�b�s���O�p
                          �}�C�N���R�[�h�i�œK���j
        main.c          �F���C���֐�
        mathfunc.c      �F�e�퐔�l���Z�֐�
        mathfunc.h      �F�e��p�����[�^���`
	add_object.c	�F�Ȗʃf�[�^�����v���O����
        packet.dsm      �Fpath1/path2�ɗ����p�P�b�g�f�[�^
        matrix.c        �F�e��}�g���N�X���Z�b�g����֐�
        reftex.dsm      �F�e�N�X�`���}�b�s���O�̂��߂̃e�N�X�`��

<�N�����@>

        % make          �F�T���v���̃R���p�C��
        % make run      �F�T���v���̎��s

        �R���p�C����A�ȉ��̕��@�ł����s�\
        % dsedb
        > run main.elf

<�R���g���[���̑�����@>
	��,��,��,���i�����L�[�j	:�J�����̏㉺���E��]
	��,�~,��,�� �{�^��	:���C�g�̏㉺���E��]
	L1�{�^��		:�A���`�G�C���A�V���O ON-OFF
	L2�{�^��		:�s�e�w�g�O�� Modulate -> Decal 
				  -> Highlight -> Highlight2
				�iHighlight �� Highlight2 �͂��̏ꍇ�����j
	R1�{�^��		:�J�����O�i
	R2�{�^��		:�J�������
	START�{�^��		:Texture Mapping ON<->OFF
	SELECT�{�^��		:AA1 LINE �̂ݕ`�� <-> Shading ���`��

<���l>
	Triangle, Triangle Strip �� AA1 off �ŕ`������A�O�`����
	�O�`������ AA1 LINE �ŏ㏑�����邱�Ƃɂ��A�j�]�̏��Ȃ�
	�A���`�G�C���A�V���O���o����B

	Backface Clip �����́A�ʂɂ��@�����قȂ鑽�ʑ̌`���A���ɍׂ�
        ���Ȗʃ|���S���ɂ͗L���ł��邪�A�|���S�����̏��Ȃ��A�@�����e���_��
	���L����Ă���ꍇ�́A�t�ɑ��ʑ̌`���ڗ������Ă��܂����Ƃ�����B

	�Ώۂɉ����āABackface Clip ��p���邩�ǂ������߂�K�v������B

