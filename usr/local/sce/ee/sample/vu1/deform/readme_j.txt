[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

VU1��p�����Q�`���ԃT���v��

<�T���v���̉��>
        ���̃v���O�����́AVU1�œ����ϊ��ƌ����v�Z�A�Q�̊􉽌`��̕�Ԃ�
	�s�Ȃ��T���v���v���O�����ł��B

<�t�@�C��>
        deform.c	�F���C���v���O����
        metal.dsm	�F�e�N�X�`���f�[�^
        sphere.dsm	�F�I�u�W�F�N�g�f�[�^(DMA packet)
        mime.vsm	�FVU1�}�C�N���v���O����

<�N�����@>
	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run deform.elf

<�R���g���[���̑�����@>
        ���{�^��        �F�ό`�͑������܂܁A��]�̒�~

<���l>
        �����̊T���́A�ȉ��̒ʂ�ł��B

        1. �e�N�X�`�������[�h
        2. �R���g���[���I�[�v��
        3. �}�g���N�X�A��ԏd�݁A�����}�g���N�X��VU1�֓n���p�P�b�g����
	   �Z�b�g
        4. VU1�ւ�DMA kick
        5. 2�ɖ߂�

        VU1�}�C�N���̒��ł͈ȉ��̂悤�ȏ������s���Ă��܂��B
        1. ��] x �����ϊ��}�g���N�X��ݒ�
        2. �e��p�����[�^�i�}�g���N�X���j��VU1���W�X�^�Ƀ��[�h
        3. �@���x�N�g���A��Ԃ���Q�̍��W�AST�l�A�F�����[�h
        4. ���W��ԁA�����ϊ��A�����v�Z�����Č��ʂ�VU1Mem�ɃX�g�A
        5. 3. �ɖ߂�A���_����J��Ԃ�
        6. �X�g�A���ꂽ�f�[�^(Gif Packet)��GS��XGKICK

        �ƂȂ�܂��BVU1Mem��ł̃_�u���o�b�t�@�����O���s���Ă��܂��B

