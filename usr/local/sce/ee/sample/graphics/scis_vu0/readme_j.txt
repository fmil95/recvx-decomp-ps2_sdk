[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�V�U�����O�T���v��(CORE&VU0��)

<�T���v���̉��>

        CORE&VU0�}�N�����[�h��p���āC�|���S�����N���b�v���E�Őؒf���C
        ���E���͂ݏo���Ȃ��V���ȃ|���S���𐶐����ĕ\������T���v���ł��D
        �T���v���� Triangle�� Triangle strip�̗����ɑΉ����Ă��܂��D
        ��ʏ�̉��F�̐����N���b�s���O�̈��\���C�R���g�[���[���[
        �ő���\�ł��D�ʏ킱�̗̈��(x, y)=(�}2048)�̘g�ɐݒ肵�C
        �|���S����GS�̃N���b�s���O�̈���͂ݏo�Ȃ��悤���䂵�܂��D
        �܂��CZ�o�b�t�@���L���Ȕ͈�(z����near��,far��)����`�����
        ����͂��ł�����C������͂ݏo���Ȃ��悤�ؒf���܂��D
        ��O�̖ʂŐ؂�ꂽ�I�u�W�F�N�g�́C���傤�ǒf�ʐ}��������
        �i�D�ƂȂ�܂��D

        ��ʓI�ɃV�U�����O�́C���_�ɋߕt�����|���S�����s���R�Ɍ�����
        �\������Ȃ��悤�ɂ��邽�߂Ɏg�p����܂��D

        (��) �n�ʁC���H�C��


<�t�@�C��>

        main.c          :���C���v���O����
        main.h          :���C���v���O�����̃v���g�^�C�v�錾
        defines.h       :�e�� .c �t�@�C���ŗp����v���g�^�C�v�錾
        cube.s          :�I�u�W�F�N�g�f�[�^(������)
        torus1.s        :�I�u�W�F�N�g�f�[�^(�h�[�i�c)
        tex128.dsm      :�e�N�X�`���f�[�^
        remake.c        :�N���b�v�ʂŐؒf�����|���S������������
                         ���邽�߂̊֐��Q

<�N�����@>

        % make          �F�R���p�C��
        % make run      �F���s

        �R���p�C����A�ȉ��̕��@�ł����s�\
        % dsedb
        > run main.elf

<�R���g���[���̑�����@>

        ����    :       x����](���_)
        ����    :       y����](���_)
        L1L2    :       z���ړ�(���_�C�O��)
        ���~    :       �N���b�s���O�̈�ύX(y����)
        ����    :       �N���b�s���O�̈�ύX(x����)
        R1R2    :       �N���b�s���O�̈�ύX(xy����)
        select�{�^��     :���f���̐؂�ւ��i�����́C�g�[���X�j
        start�{�^��      :�V�U�����O�|���S���̕\���̐؂�ւ�
                          (TriangleFan or LineStrip)

<���l>

    ���ӎ���


        1.�r���[�{�����[���ɂ�����nearZ,farZ�̋��E�ʂƁCZbuffer�l�Ƃ�
        �Ή��t���̍ۂɒ��ӂ��Ȃ���΂Ȃ�Ȃ��_������܂��D
        nearZ,farZ�ʂ̉��Z�͒ʏ핂�������_���Z�ōs���܂��D������
	Zbuffer�l�͐����ŕ\������邽�߁CFTOI���̖��߂ɂ�肱��𐮐���
	���邱�ƂɂȂ�܂��D�ʏ�ł́C�������������Ɋ��҂���Z�l�������
        ��悤�C���肬��̒l�őΉ��Â����s���܂��D
        (�Ⴆ��24bit Zbuffer�̏ꍇ�C�����������Z��nearZ��(��Z=1.0)���
        �_��16777215.0�ɂȂ�悤�ݒ�(16777215.0->FTOI->0xffffff)�D)
        ������32bit�̕��������\���ł͉�������23bit�ł���C�M���ł���
        �L�������͂�������6�`7�����x�ƂȂ�܂��D���̂��߁C�v�Z�덷
        �ɂ�� 1.0->16777215.0 �̉��Z���ʂ�16777215.0��傫������
        �\�����\���ɂ���܂��D���̎�Zbuffer�l�͔j�]���C�[���ƂȂ���
        ���܂��܂��D
        ������m���ɉ�����邽�߂ɂ́CZbuffer�������X�]���ɂ���nearZ��
        �̑Ή��l��16777000.0�ɂ���Ȃǂ̏��u���K�v�ł��D
        �{�T���v���̂悤�ɁC����nearZ�ʂɃN���e�B�J���ȃv���O�����ł́C
        ����Zbuffer�ɂ�����Z�l�̌v�Z���x�̖��ɗJ�����ׂ��ł��傤�D

        2.�f�ʂ��v�Z����Ƃ��C�ؒf�ʂƃ|���S���̊e�ӂƂ̌�_�����߂�
        �K�v������܂��D���̍ہC���_���(vertex,normal,color,textureUV)
        �Ȃǂ��Ԃ���K�v������킯�ł����Cnormal�̕�Ԃɂ��Ă͒���
        ���K�v�ł��D
        �|���S�����O�[���[�V�F�[�f�B���O�ŕ\�����Ă���ꍇ�C�|���S����
	�ӏ�̓_�̐F�́C�[�_�ł���u���_�̐F�̐��`��ԁv�ɂ���ċ��߂�
	��܂��D���������āC���Ղ�normal�̐��`��Ԃ������Ȃ�lighting�F
        ���v�Z����ƌ��ʂ��قȂ��Ă��܂��܂��D
        �����������邽�߂ɂ́C���_��lighting�F�v�Z��CGS�ɓn��color��
        ���`��Ԃ���K�v������܂��D�܂�C��Ԃ��K�v�ȗv�f��(vertex,
        GScolor, textureUV)�Ƃ������ƂɂȂ�܂��D


    ��@�̊T��

        �ؒf����Ăł���|���S���́C�ʑ��p�`�ł���Ƃ�������̉���
        TriangleFan�ɂ���ĐV���ȃ|���S���𐶐��E�`�悵�܂��D
        
        �O�p�|���S���͂R�̕ӂ������Ă��܂��D���ꂪ����ʂɂ����
        �ؒf�����ƁC�Q�̕ӂ���_�������C���̂Q�̌�_���Ȃ���
        �V�������p�`����邱�Ƃ��ł��܂��D�����TriangleFan�ŕ\��
        �\�ȑ��p�`�ł��D���ɂ��̑��p�`���C�܂��V���Ȗʂɂ����
        �ؒf���܂��D����Ƃ�����Q�̌�_�������C������Ȃ���
        ���l�ɐV���ȑ��p�`�����܂��D
        ���̂悤�ɁC�ċA�I�ɐؒf�����������Ȃ��Ă����΁C�ŏI�I��
        �K�v�ȗ̈��(�f�ʌQ��)�Ɏ��܂�|���S���𐶐��ł��邱��
        �ɂȂ�܂��D

        �{�T���v���ł͂U���̃N���b�v��(+x,-x, +y,-y, +z,-z)
        ���ꂼ��ɂ��Đؒf�������s���C�ŏI�I�ȃV�U�����O�|���S����
        �������Ă��܂��D

        (�Q�l) Computer Graphics --princeples and practice--
                            SECOND EDITION
                 authors:  James Foley, Andries van Dam,
                         Steven Feiner, John Hughes
                 publisher: ADDISON-WESLEY PUBLISHING COMPANEY 
             
                 Section 3.14 Clipping Polygons(pp124-127)


    �����̊T���́A�ȉ��̒ʂ�ł��B

        ===== main.c =====
        main():

        1. �_�u���o�b�t�@�̐ݒ�A�e�N�X�`���̃��[�h,pad�I�[�v��
        2. �p�b�h����̏���ǂݎ��
        3. �I�u�W�F�N�g�f�[�^����GS�]���p�p�P�b�g�쐬(with�N���b�s���O)
        4. �N���b�s���O���ꂽ�|���S�����V�U�����O����
        5. �V�U�����O�|���S���̃p�P�b�g���쐬��GS�ɓ]��
        6. �N���b�s���O���ꂽ�I�u�W�F�N�g�f�[�^�̃p�P�b�g��GS�ɓ]��
        7. 2.�ɖ߂�


    �֐��̐���  

        InitNodeArraySet():
        TriangleFan�p�ɗp�ӂ������_�z����Ǘ�����\����NodeArraySet�^
        �����������邽�߂̊֐��D
        
        InitScissorPlane();
        �N���b�v�ʂ̏���ێ�����\����ScissorPlaneArray�^������������D

        ResetNodeArraySet();
        �ċA�I�ȃV�U�����O�����Ŏg�p�����C���_�z��\���̂�����������֐�

        FlipNodeArray();
        TriangleFan�p�̒��_�z����_�u���o�b�t�@�����ɂ����ʓI��
        �g�p���邽�߂́C�t���b�v�������Ȃ��֐�

        PushTriangleNodeArray();
        �V�U�����O�����ɓn���O�p�|���S���̏���o�^���C�ێ�����֐�
        
        ClipCheck();
        �V�U�����O�Œf�ʂ����t����ہCTriangleFan���`������ڑ����_
        �ԂŃN���b�s���O���`�F�b�N���C�ؒf�ʂ��܂����ł��邩�ǂ�����
        �m�F����֐��D

        InterpNode();
        �f�ʂ𔻒肵����C�ʂƓ_�̐ڑ����̊ԂɌ�_�����߁C�V����
        �ڑ����_�𐶐�����֐�

        ScissorTriangle();
        �O�p�|���S���ɃV�U�����O�������{���֐�

        DrawScissorPolygon();
        �V�U�����O�����|���S��(TriangleFan)�𓧎��ϊ����CGS�]���p��
        �p�P�b�g���쐬���]������֐��D

        MakePacket();
        �ʏ�̃I�u�W�F�N�g�f�[�^���N���b�v�����t���œ����ϊ����C
        GS�]���p�̃p�P�b�g���쐬����D
        �N���b�s���O���ꂽ�O�p�|���S���̓V�U�����O�����ɓn�����D


