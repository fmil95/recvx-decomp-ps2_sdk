[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�e�N�X�`���X���b�v�T���v��

<�T���v���̉��>
       �e�N�X�`������ւ��Ȃ���`����s���T���v��

<�t�@�C��>

        buff0.dsm       �F�g���v���o�b�t�@�̓]����ԂO��ݒ肷��p�P�b�g
        buff1.dsm       �F�g���v���o�b�t�@�̓]����ԂP��ݒ肷��p�P�b�g
        buff2.dsm       �F�g���v���o�b�t�@�̓]����ԂQ��ݒ肷��p�P�b�g
        grid.dsm        �F�n�ʂ̃��f��
        m.vsm           �F�`��p�}�C�N���R�[�h �œK���Ȃ�
        m_opt.vsm       �F�`��p�}�C�N���R�[�h �œK��
        main.c          �F���C���֐�
        mat.dsm         �F�n�ʂ̃e�N�X�`��
        mathfunc.c      �F�e�퐔�l���Z�֐�
        mathfunc.h      �F�e��p�����[�^���`
        matrix.dsm      �F�}�g���N�X�p�p�P�b�g�̃e���v���[�g
        no[1-4].dsm     �F�P�Ԃ���S�Ԃ̃e�N�X�`��
        packet.dsm      �Fpath1/path2�ɗ����p�P�b�g�f�[�^
        phys.c          �F�{�[���̈ʒu���v�Z���p�P�b�g���ɃZ�b�g����֐�
        sphere.dsm      �F�{�[���̃��f��
        tex_swap.h      �F�T���v���̃p�����[�^��v���g�^�C�v�錾

<�N�����@>

        % make          �F�e�N�X�`������ւ��T���v���̃R���p�C��
        % make run      �F�e�N�X�`������ւ��T���v���̎��s

        �R���p�C����A�ȉ��̕��@�ł����s�\

        % dsedb
        > run main.elf

<�R���g���[���̑�����@>
        ��������        �F���_�̉�]

        start�{�^��        �F�{�[���̃��Z�b�g

<���l>

        �����̊T���́A�ȉ��̒ʂ�ł��B

        [main.c]

        1. �_�u���o�b�t�@�̐ݒ�A�p�����[�^�̏�����
        2. �e�N�X�`���֘A�̃��W�X�^�̐ݒ�
        3. �_�u���o�b�t�@�̃X���b�v
        4. �{�[���̍��W���v�Z���A�}�g���N�X�ɒl���Z�b�g
        5. �p�b�h����̓��͒l���擾���A���̒l�ɂ���Ď��_���Z�b�g
        6. ch.1��DMA���N�����`����s��
        7. 3�ɖ߂�


        VU1�}�C�N���̒��ł͈ȉ��̂悤�ȏ������s���Ă��܂��B

        [m.vsm]

        1. ��] x �����ϊ��}�g���N�X��ݒ�
        2. �e��p�����[�^�i�}�g���N�X���j��VU1���W�X�^�Ƀ��[�h
        3. �@���x�N�g���A���_���W�AST�l�A�F�����[�h
        4. �����ϊ��A�����v�Z�AQ�l�Ȃǂ��v�Z���Č��ʂ�VU1Mem�ɃX�g�A
        5. 3. �ɖ߂�A���_����J��Ԃ�
        6. �X�g�A���ꂽ�f�[�^(Gif Packet)��GS��XGKICK

        [packet.dsm]
        �p�P�b�g�̗���ɂ��Ĉȉ��Ɏ����܂��B
        1. �}�C�N���R�[�h�̃��[�h
        2. �n�ʂ�`�悷�邽�߂̊��̐ݒ�A�e�N�X�`���o�b�t�@�̐ݒ�
        3. �n�ʂ̃e�N�X�`���̓]��
        4. �n�ʂ̕`��
        5. �{�[���P��`�悷�邽�߂̃}�g���N�X�̓]��
        6. �{�[���P�̃e�N�X�`����]�����邽�߂̃o�b�t�@�̐ݒ�
        7. �{�[���P�̃e�N�X�`����]��
        8. �{�[���P��`��
        9. 5-8���{�[���̐������J��Ԃ��B�{�[���̃e�N�X�`���͂S��ނ�
           ���X�ɓ]�����Ă���(256x256, 32bitRGBA)

