[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�o���v�}�b�s���O�T���v��

<�T���v���̉��>
       �[���o���v�}�b�s���O�`����s���T���v��

<�t�@�C��>
        grid.dsm        : �O���b�h�̃��f���p�P�b�g
        m.vsm           : st�ɗ^����ꂽ�I�t�Z�b�g��^���Čv�Z����}�C�N��                           �R�[�h
        m_opt.vsm       : m.vsm�̍œK����
        main.c          : ���C��
        mat.dsm         : �}�b�g�̕��̃e�N�X�`���f�[�^
        packet.dsm      : �����ϊ��}�g���N�X��GS�̐ݒ�̂��߂̃p�P�b�g
        textbump.dsm    : ����"BUMP MAP"�̃o���v�e�N�X�`��
        wavebump.dsm    : �g�̕��̃o���v�e�N�X�`��

<�N�����@>

        % make          �F�o���v�}�b�v�T���v���̃R���p�C��
        % make run      �F�o���v�}�b�v�T���v���̎��s

        �R���p�C����A�ȉ��̕��@�ł����s�\

        % dsedb
        > run main.elf

<�R���g���[���̑�����@>
        �����L�[        : ���_�̈ړ�
        �����{�^��      : ���C�g�̕����̕ω�
        START�{�^��     : �o���v��ON/OFF
        SELECT�{�^��    : �o���v�e�N�X�`���̐؂�ւ�

<���l>
        ���̃T���v���ł̃o���v�}�b�s���O�̏����͈ȉ��̂悤�ɂȂ�܂��B

        1. ���̃��f����}���̃e�N�X�`���t���ŕ`�悷��B
        2. �`�悳�ꂽ�t���[���o�b�t�@����o���v�e�N�X�`���t����
           �������f����`�悵�A�t���[���o�b�t�@�̐F��������B
           ���̂Ƃ��A���f���̃e�N�X�`�����Wst���A�����̕�����
           ���X�̂��̂��班�����炷�B
        3. st�����炳���ɁA�o���v�e�N�X�`���t���̃��f����`�悷��B
           ���x�̓t���[���o�b�t�@�ɐF��������B

        2, 3��st�̒l�����ꂽ���������̐}���ɔZ�W������A����ɂ����
        �o���v�̂悤�Ȍ��ʂ𓾂邱�Ƃ��ł��܂��B

<�����T��>
        �����̊T���́A�ȉ��̒ʂ�ł��B

        [main.c]
        1. �O���t�B�b�N�X�̃p�����[�^�̃��Z�b�g
        2. �_�u���o�b�t�@�̐ݒ�
        3. �e�N�X�`���̃p�����[�^�̐ݒ�
        4. �}�C�N���R�[�h�A�e�N�X�`���̃��[�h
        5. �p�b�h����̓��͂Ŋe��p�����[�^�ݒ�
        6. �`��DMA�̃L�b�N

        VU1�}�C�N���̒��ł͈ȉ��̂悤�ȏ������s���Ă��܂��B

        [m.vsm]
        1. ��] x �����ϊ��}�g���N�X��ݒ�
        2. �e��p�����[�^�i�}�g���N�X���j��VU1���W�X�^�Ƀ��[�h
        3. �@���x�N�g���A���_���W�AST�l�A�F�����[�h
        4. �����ϊ��A�����v�Z�AQ�l�Ȃǂ��v�Z���Č��ʂ�VU1Mem�ɃX�g�A
           ���̍ہAst�l�ɐݒ肳�ꂽ�I�t�Z�b�g��������B
        5. 3. �ɖ߂�A���_����J��Ԃ�
        6. �X�g�A���ꂽ�f�[�^(Gif Packet)��GS��XGKICK


