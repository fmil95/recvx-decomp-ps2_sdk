[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

VU1��p����B-Spline�`��T���v��

<�T���v���̉��>
        VU1��p���ė^����ꂽ�R���g���[���|�C���g����
        B-Spline�T�[�t�F�X��`�悷��T���v���ł��B

<�t�@�C��>
        main.c          �F���C���v���O����
        bs.c            �F�p�P�b�g�쐬�֐�
        noise.h         �F�m�C�Y�֐��̃v���g�^�C�v�錾
        noise.c         �F�m�C�Y�֐�
        b_data.dsm      �F�p�P�b�g�i�}�g���N�X�A�u�����f�B���O�t�@�N�^�A
			  �p�P�b�g���j
        b.vsm           �FB-Spline�`��}�C�N���R�[�h
        grid.h          �F�O���b�h�̃R���g���[���|�C���g�f�[�^
        torus.h         �F�g�[���X�̃R���g���[���|�C���g�f�[�^
        flower.dsm      �F�e�N�X�`���f�[�^
        b_opt.vsm       �Fb.vsm�̍œK����

<�N�����@>
        % make          �F�R���p�C��
        % make run      �F���s

        �R���p�C����A�ȉ��̕��@�ł����s�\
        % dsedb
        > run main.elf

<�R���g���[���̑�����@>
        �����L�[        �F���_�̈ړ�
        select�{�^��    �F���f���̐؂�ւ��i�O���b�h�A�g�[���X�j
        R1�{�^��        �F�e�N�X�`���t���E���C���t���[���̐؂�ւ�

<���l>
	�p�P�b�g�̍\���A�}�C�N���R�[�h�ɂ��Ă� default.htm ���Q�Ƃ���
	���������B
	
        �����̊T���́A�ȉ��̒ʂ�ł��B

        ===== main.c =====
        main():

        1. �_�u���o�b�t�@�̐ݒ�A�e�N�X�`���̃��[�h
        2. pad�I�[�v���A�}�C�N���R�[�h�̓]��
        3. �u�����f�B���O�W���̐ݒ�A�ό`�p�m�C�Y���z�̏�����
        4. �p�b�h����̏���ǂݎ��A�`��p�P�b�g�A�e��}�g���N�X��ݒ�
        5. �R���g���[���|�C���g�̍��W���p�P�b�g�ɃZ�b�g
        6. VU1�Ƀp�P�b�g��]�����ĕ`��
        7. ���̃t���[���̃R���g���[���|�C���g��ݒ�
        8. 4.�ɖ߂�

        SetBlendingCoeff():
        1. �S���������Ƃ���t�̒l�ɂ�����u�����f�B���O�֐��A
           �u�����f�B���O�֐��̓��֐��̒l���v�Z
        2. �}�C�N���R�[�h�ł́A4x4�̃R���g���[���|�C���g����ł���36�̎O�p�`��
           �R�̃g���C�A���O���X�g���b�v�ɕ������\������B
           ���̃g���C�A���O���X�g���b�v�̊e���_�ɑΉ�����1.�Ōv�Z���ꂽ
           �l�̑g�ݍ��킹�����炩���߃p�P�b�g�Ƃ��ăX�g�A����
        
        CreateViewingMatrix():
        1. �X�N���[���ϊ����s���}�g���N�X�̐ݒ�

        SetViewPosition():
        1. ���_�̐ݒ�

        MoveLight():
        1. ���������̐ݒ�

        ===== bs.c =====
        CreateControlPointArea():
        1. b_data.dsm�ɐݒ肳�ꂽ�p�P�b�g�e���v���[�g��`�悷��f�[�^��
           U, V�̃R���g���[���|�C���g���AU, V�����ꂼ����Ă��邩�ǂ���
           �ɂ���āA�X�N���b�`�p�b�h��ɓK���Ȑ��R�s�[����
        
        SetControlPoint():
        1. CreateControlPointArea()�ɂ���ăX�N���b�`�p�b�h��ɍ��ꂽ
           �p�P�b�g���Ɏ��ۂɕ`�悷��R���g���[���|�C���g�̍��W���Z�b�g����

        ==== noise.c ====
        1. �m�C�Y���z�������A��Ԋ֐�

        ==== b.vsm(VU1 micro code) ====

        [b.vsm]
        1. �����ϊ��}�g���N�X�����W�X�^VF28 - VF31�ɐݒ�
        2. �e��J�E���^�Z�b�g�i�X�g���b�v���_���Ȃǁj
        3. �v�Z���悤�Ƃ���4x4�R���g���[���|�C���g�Z�b�g��
           VF01-VF16�֐ݒ�
        4. GIF�^�O�̃��[�h�E�X�g�A�ARGBAQ, ST, XYZF�̃X�g�A
           �|�C���^�̐ݒ�
        5. �v�Z�Ώۂ̃u�����f�B���O�W���̃��[�h
        6. �v�Z�_�ɑ΂���ڐ��x�N�g�����v�Z���A
           ���̊O�ς��@���x�N�g�������߂�
        7. �@���x�N�g���ƕ��s�����̌����x�N�g���Ƃ̓��ς����߁A
           �Ɠx�v�Z���s��
        8. �u�����f�B���O�W�������[�h���A�R�p�`������̕������W��
           ���߂�
        9. ���炩���ߗ^����ꂽST�̒l�����[�h���A�ɃI�t�Z�b�g������
           ���ۂ�ST�l�����߂�
        10. FTOI�Ȃǂ��s���AVUmem�ɃX�g�A�|�C���^�̎����ʒu��
            RGBAQ, ST, XYZF���X�g�A����
        11. ���_���񃋁[�v����i5.�ɖ߂�j
        12. �p�P�b�g���񃋁[�v����(2.�ɖ߂�)
        13. �X�g���b�v�̐؂�߂���邽�߂ɁA�؂�ڂƂȂ�p�P�b�g��
            Z�̕����𔽓]����
        14. GIF�ɑ΂���XGKICK����

