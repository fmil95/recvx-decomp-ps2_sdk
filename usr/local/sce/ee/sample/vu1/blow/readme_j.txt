[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

VU1��p�����p�[�e�B�N���`��T���v��

<�T���v���̉��>
        VU1�ŗ^����ꂽ�R�������W����p�[�e�B�N���Ƀ��[�V�����u���[��
        ���ĕ`�悷��T���v���v���O�����ł��B
        �`�悷��T���v���ɂ͉ԉ�(blow)�Ɖ�(gas)������A�����Ƃ�
        �S������̃}�C�N���R�[�h��p���ĕ`�悵�Ă��܂��B

<�t�@�C��>
        blow.c          �F���C���v���O����
        blow.h          �F�ԉΗp�̃p�[�e�B�N���̐ݒ�p�����[�^
        data.dsm        �F�ԉΗp�̃p�[�e�B�N���A�p�[�e�B�N���̏Ƃ�Ԃ��̃f�[�^
                          �p�P�b�g
        data_gas.dsm    �F���p�̃p�[�e�B�N���A�p�[�e�B�N���̏Ƃ�Ԃ��̃f�[�^
                          �p�P�b�g
        draw.vsm        �F�n�ʂ�`�悷�邽�߂̃}�C�N���R�[�h
        firebit.dsm     �F�p�[�e�B�N���̃e�N�X�`��
        fireref.dsm     �F�p�[�e�B�N���̏Ƃ�Ԃ��̃e�N�X�`��
        gas.h           �F���p�̃p�[�e�B�N���̐ݒ�p�����[�^
        gas_tex.dsm     �F���̃e�N�X�`��
        grid.dsm        �F�n�ʂ̃f�[�^�p�P�b�g
        p.vsm           �F�p�[�e�B�N����`�悷�邽�߂̃}�C�N���R�[�h
        p_opt.vsm       �Fp.vsm���œK����������
        p2.vsm          �F�p�[�e�B�N���̏Ƃ�Ԃ���`�悷�邽�߂̃}�C�N���R�[�h
        part.dat        �F�p�[�e�B�N���̃f�[�^��
        part1.dsm       �Fpart.dat��VIFtag�Ŋ���������
        part10.dsm      �Fpart1.dsm���P�O�܂Ƃ߂�����
        physics.c       �F�p�[�e�B�N���̋O�Ղ��v�Z����֐�
        spart.dat       �F�p�[�e�B�N���Ƃ�Ԃ��̃f�[�^��
        spart1.dsm      �Fspart.dat��VIFtag�Ŋ���������
        spart10.dsm     �Fspart1.dsm���P�O�܂Ƃ߂�����
        src.dsm         �F�p�[�e�B�N���̑��x�x�N�g���A�F�̕ۑ��p�G���A
        srcpart.dat     �Fsrc.dsm�ɃC���N���[�h�����f�[�^��
        wood.dsm        �F�n�ʂ̃e�N�X�`��

<�N�����@>

        % make          �F�ԉ΃T���v���̃R���p�C��
        % make run      �F�ԉ΃T���v���̎��s

        % make -f Makefile.gas          �F���T���v���̃R���p�C��
        % make -f Makefile.gas run      �F���T���v���̎��s

        �R���p�C����A�ȉ��̕��@�ł����s�\
        % dsedb
        > run blow.elf

<�R���g���[���̑�����@>
        �����L�[        �F���_�̈ړ�
        ���{�^��        �F�p�[�e�B�N���̒�~

<���l>
        �����̊T���́A�ȉ��̒ʂ�ł��B

        [blow.c]
        1. �_�u���o�b�t�@�̐ݒ�A�e�N�X�`���̃��[�h
        2. �p�����[�^�A�p�[�e�B�N���̏�����
        3. �����ϊ��}�g���N�X�A�����ɐ����ȃI�t�Z�b�g�x�N�g��
           �i�p�[�e�B�N���W�J�p�j���p�P�b�g���ɃZ�b�g
        4. �n�ʂ�`��
        5. �p�[�e�B�N���ƃp�[�e�B�N���̏Ƃ�Ԃ��̍��W���v�Z
           (SetParticlePosition�A�ԉΗp�Ɖ��p�ɂ��ꂼ�ꏀ��)
        6. �p�[�e�B�N���̏Ƃ�Ԃ���`��
        7. �p�[�e�B�N����`��
        8. 3�ɖ߂�

        VU1�}�C�N���̒��ł͈ȉ��̂悤�ȏ������s���Ă��܂��B

        [draw.vsm]
        1. ��] x �����ϊ��}�g���N�X��ݒ�
        2. �e��p�����[�^�i�}�g���N�X���j��VU1���W�X�^�Ƀ��[�h
        3. �@���x�N�g���A���_���W�AST�l�A�F�����[�h
        4. �����ϊ��A�����v�Z�����Č��ʂ�VU1Mem�ɃX�g�A
        5. 3. �ɖ߂�A���_����J��Ԃ�
        6. �X�g�A���ꂽ�f�[�^(Gif Packet)��GS��XGKICK

        [p.vsm]
        1. �e��p�����[�^�i�I�t�Z�b�g�A�����ϊ��}�g���N�X���AST�j��
           VU1���W�X�^�Ƀ��[�h
        2. ���W�A���x�x�N�g���A�F�����[�h
        3. ���x�x�N�g���ɂ���č��W���ړ��i���[�V�����u���[�̂��߁j
        4. 3.�œ���ꂽ���W�ƃI�t�Z�b�g�����������W�̂Q�_�𓧎��ϊ�
        5. 4.�̂Q�_�����Ԑ�����Ίp���Ƃ���l�p�`�i�S�_�ɂ��
           �g���C�A���O���X�g���b�v�j���쐬
        6. 5.�̌��ʁA�Ή�����ST�A�F��VU1Mem�ɃX�g�A
        7. 2.�֖߂�A�p�[�e�B�N�����J��Ԃ�
        8. �X�g�A���ꂽ�f�[�^(Gif Packet)��GS��XGKICK

        [p2.vsm]
        p.vsm��胂�[�V�����u���[����菜�������̂ƂȂ�܂��BVU1Mem��ł�
        �_�u���o�b�t�@�����O���s���Ă��܂��B
    
        ����
        p2.vsm�͍œK������Ă���܂���B



