[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�t���[���o�b�t�@�̓��e���Z�[�u����T���v��

<�T���v���̉��>
        ���̃v���O�����́A�`�悳�ꂽ�t���[���o�b�t�@���z�X�g�}�V����
        �t���[���o�b�t�@�ɃZ�[�u������̂ł��B
        �`�掩�̂� sample/vu1/deform �̃T���v�����g�p���Ă��܂��B

<�t�@�C��>
        capture.c       �F���C���v���O����
        metal.dsm       �F�e�N�X�`���f�[�^
        sphere.dsm      �F�I�u�W�F�N�g�f�[�^(DMA packet)
        mime.vsm        �FVU1�}�C�N���v���O����

<�N�����@>
        % make          �F�R���p�C��
        % make run      �F���s

        �R���p�C����A�ȉ��̕��@�ł����s�\
        % dsedb
        > run capture.elf

<�R���g���[���̑�����@>
        ���{�^��        �F�ό`�͑������܂܁A��]�̒�~
        �~�{�^��        �F�L���v�`�����ăv���O�������I��

<�����T��>
        �L���v�`�������̊T���́A�ȉ��̒ʂ�ł��B
        �i�`�揈�����̂�deform�̃T���v�����Q�Ɓj

        1. �~�{�^���������B
        2. �A�j���[�V�����̂��߂̃}�g���N�X�X�V���s�����ƂȂ�
           ���̃t���[������������̃t���[���o�b�t�@�ɕ`��
        3. ���[�J���֐� StoreImage()���Ăяo��
        4. �z�X�g�t�@�C���̃I�[�v��
        5. odd/even�̃t���[���o�b�t�@�̓��e�𒙂��郁�������m��
        6. even�t�B�[���h�̓��e�����[�J��-->�z�X�g�]������
           (sceGsExecStoreImage())
        7. odd�t�B�[���h�̓��e�����[�J��-->�z�X�g�]������
        8. �]�����ꂽ�C���[�W��odd/even���݂Ƀz�X�g��̃t�@�C���ɂ����o��
           (sceWrite())
        9. �m�ۂ������������J��
        10. �t�@�C���L�q�q���J��
        11. �v���O�������I������

<���l>
        �o�͂����t�@�C����32bit-rgba��raw data�ł��B
        
