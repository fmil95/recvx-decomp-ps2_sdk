[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�~�b�v�}�b�v�T���v��

<�T���v���̉��>
       �~�b�v�}�b�v��p�����`����s���T���v��

<�t�@�C��>
        256.dsm
        128.dsm
        64.dsm
        32.dsm
        16.dsm
        8.dsm
        4.dsm           �F�e�~�b�v���x���p�̃e�N�X�`��
        main.c          �F���C���֐�
        mathfunc.c      �F�e�퐔�l���Z�֐�
        rect.dsm        �F��`�悷��p�P�b�g
        basic.vsm       �F�`��p�}�C�N���R�[�h
        mathfunc.h      �F�e��p�����[�^���`

<�N�����@>

        % make          �F�~�b�v�}�b�v�T���v���̃R���p�C��
        % make run      �F�~�b�v�}�b�v�T���v���̎��s

        �R���p�C����A�ȉ��̕��@�ł����s�\

        % dsedb
        > run main.elf

<�R���g���[���̑�����@>

        ���{�^��        �F�̑O��^���̒�~

<���l>
        �����̊T���́A�ȉ��̒ʂ�ł��B

        [main.c]

        1. �_�u���o�b�t�@�̐ݒ�A�e�~�b�v���x���̃e�N�X�`���̃��[�h
        2. �e�N�X�`���̃��W�X�^��ݒ�(TEX0_1, TEX2_1, MIPTBP1_1, MIPTBP2_1���j
        3. �p�b�h����̓��͒l���擾���A���̒l�ɂ���ăp�����[�^���Z�b�g
        4. MIPMAP on �ݒ�(TEX1_1)
        5. �}�g���N�X��ݒ�
        6. �~�b�v�}�b�v���ꂽ�i�E�j��`��
        7. MIPMAP off �ݒ�(TEX1_1)
        8. �}�g���N�X��ݒ�
        9. �~�b�v�}�b�v����Ă��Ȃ��i���j��`��
        10. 3�ɖ߂�

        VU1�}�C�N���̒��ł͈ȉ��̂悤�ȏ������s���Ă��܂��B

        [basic.vsm]

        1. ��] x �����ϊ��}�g���N�X��ݒ�
        2. �e��p�����[�^�i�}�g���N�X���j��VU1���W�X�^�Ƀ��[�h
        3. �@���x�N�g���A���_���W�AST�l�A�F�����[�h
        4. �����ϊ��A�����v�Z�AQ�l�Ȃǂ��v�Z���Č��ʂ�VU1Mem�ɃX�g�A
        5. 3. �ɖ߂�A���_����J��Ԃ�
        6. �X�g�A���ꂽ�f�[�^(Gif Packet)��GS��XGKICK


