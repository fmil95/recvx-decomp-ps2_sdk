[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�V�U�����O�T���v��(VU1�� 2nd version)

<�T���v���̉��>

        VU1��p���ă|���S�����N���b�v���E�Őؒf���C���E���͂ݏo��
        �Ȃ��V���ȃ|���S���𐶐����ĕ\������T���v���ł��D
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

        VU1�ł́CCORE&VU0�łɔ�׃p�t�H�[�}���X�͑傫�����サ�Ă��܂��D
        �܂��CVU1code�T�C�Y�I�ɂ����ɑ傫�Ȃ��̂ɂȂ��Ă��܂��D
        ���W�X�^�l�̃X�^�b�N�ޔ��ɂ��֐�����̏����܂ł��Ă��܂��̂ŁC
        ��K�͂�VU1�v���O�����̈�̎�{�ɂȂ邩������܂���D


<�t�@�C��>

        main.c          :���C���v���O����
        main.h          :���C���v���O�����̃v���g�^�C�v�錾
        defines.h       :�e�� .c �t�@�C���ŗp����v���g�^�C�v�錾
        cube.dsm        :�I�u�W�F�N�g�f�[�^(������)
        torus51.dsm     :�I�u�W�F�N�g�f�[�^(�h�[�i�c)
	grid1800.dsm    :�I�u�W�F�N�g�f�[�^(���ʕ���)
        tex128.dsm      :�e�N�X�`���f�[�^
	scissor.dsm     :VU1�}�C�N���R�[�h�]���p�p�P�b�g
	scissor.vsm     :VU1�}�C�N���R�[�h
	sci_opt.vsm     :VU1�}�C�N���R�[�h(�œK����)
	sci_tri.vsm     :VU1�}�C�N���R�[�h(�œK����Triangle���f����p)
	sci_str.vsm	:VU1�}�C�N���R�[�h(�œK����Strip���f����p)
	vu1pack.c	:VU1�]���p�p�P�b�g�쐬�֐�

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
        SELECT�{�^��     :���f���̐؂�ւ��i�����́C�g�[���X�j
        START�{�^��      :�V�U�����O�|���S���̕\���̐؂�ւ�
                          (TriangleFan or LineStrip)

<���l>

    2nd version �̉��Ǔ_

	�ȑO�̃o�[�W�����ł͌v�Z�덷�̂��߁C�V�U�����O�����|���S�����m��
	�Ԃɔ����Ȍ����ł����肪����܂����D
	���̃o�[�W�����ł́C�������������@�𓱓����Ă��܂��D
	�\�[�X scissor.vsm ���̃��x�� PUSHTRIANGLE: �̒���ɁC�V�U�����O
	�|���S��������Ɋg�傷��R�[�h��}�����܂����D�g�嗦1.01�ɂ���
	�͗p�r�ɂ���Ē������K�v��������܂���D�����ӂ��������D


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

        �ؒf����Ăł���|���S���͓ʑ��p�`�ł���C�Ƃ�������̉���
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

        1. �_�u���o�b�t�@�̐ݒ�A�e�N�X�`���̃��[�h
        2. pad�I�[�v���A�}�C�N���R�[�h�̓]��
        3. �p�b�h����̏���ǂݎ��
	4. VU1�]���p�p�P�b�g�쐬
        5. VU1�Ƀp�P�b�g��]�����ĕ`��
	6. 3.�ɖ߂�

	SetViewScreenClipMatrix():
	�e���_���N���b�v���W�n�Ɏˉe���邽�߂̃}�g���N�X(view_clip)
	���v�Z���܂��D�����ɓ����ϊ��p�̃}�g���N�X(view_screen)��
	���߂Ă��܂��D�܂��C�N���b�v���W����X�N���[�����W�ɕϊ�����
	�}�g���N�X���v�Z���܂��D

	SetGifClipLine():
	�N���b�s���O�̈�̋��E����`�悷�邽�߂�GIF�p�P�b�g���쐬����֐��D
	�����ł�Vu0�Œ��ړ����ϊ����C���̏��GIF�̃p�P�b�g�f�[�^���쐬
	���Ă��܂��D
	
      ===== vu1packet.c =====
        VU1�]���p�̃p�P�b�g�𐶐����邽�߂̊֐��Q

        Vu1DmaPacket�\����:
        VU1�ɓ]������p�P�b�g�𐶐��C�Ǘ�����\���́D

        InitVu1DmaPacket():
        Vu1DmaPacket�\���̂̏������֐��D�����ɂ͂Q�̊Ǘ��e�[�u��
        (�p�P�b�g)�ւ̃|�C���^���n����܂��D����̓p�P�b�g�̃_�u��
        �o�b�t�@�̂��߂ł��D

        GetVu1ObjHead():
        ���ݗL���ȃp�P�b�g�̐擪�A�h���X��Ԃ��܂��D

        AddVu1ObjPacket():
        .dsm�t�@�C���ŃR���p�C������C��������ɑ��݂���I�u�W�F�N�g
        �f�[�^(����VU1�p�p�P�b�g�`���ɂȂ��Ă���)��ǉ��o�^���܂��D

        CloseVu1ObjPacket():
        ���ݗL���ȃp�P�b�g���I�������C�_�u���o�b�t�@�̃X�C�b�`��
        �s���܂��D�Ԃ�l�ɗL���ȃp�P�b�g�̃A�h���X��Ԃ����߁C
        DMAKICK�Ɏg�p�ł��܂��D

	GetVu1EnvHead():
        VU1�Ƀ}�g���N�X�f�[�^���v���Z�b�g���邽�߂̍\���̂ւ�
        �|�C���^��Ԃ��܂��D

        CloseVu1EnvPacket():
        VU1�Ŏg�p����}�g���N�X�f�[�^(�p�P�b�g)����C�p�P�b�g�ւ�
        �|�C���^��Ԃ��܂��D


       ==== scissor.vsm(VU1 micro code) ====

        [scissor.vsm]

        1. �e��}�g���N�X�����W�X�^VF01 - VF16�ɐݒ� & pre-calculate
        2. �`�摮���̔���(Triangle or TriangleStrip)�t���O���Z�b�g
        3. �e��J�E���^�Z�b�g�i�X�g���b�v���_���Ȃǁj
        4. GIF�^�O�̃��[�h�E�X�g�A�ARGBAQ, ST, XYZF�̃X�g�A
           �|�C���^�̐ݒ�
        5. ���_�𓧎��ϊ�
        6. �@���x�N�g���ƃ��C�g�x�N�g���̓��ς��v�Z
        7. �@���x�N�g���Ƃ̓��ς��璸�_�̐F�����߂�
	8. �e�N�X�`�����W(S,T,Q)�̌v�Z
        9. ���_���N���b�v���W�Ɏˉe���t���O��ݒ�(VI01)
           (XYZ�Ƃ���Βl|W|�͈̔́CCLIP���߂ɂ��ߋ�3�_�𔻒�)
	10. ���Y�|���S�����ؒf�ʂ��܂����ł�����C
            �V�U�����O���[�`��(S1)�ɕ���
        11. XYZ2,XYZ3��������L�b�N���邩����D�X�g�A�D
        12. ���_���������[�v����(5.�ɖ߂�)
	13. XGKICK�CGSrendering
        14. �I�u�W�F�N�g�̃u���b�N���������[�v����D
            (4.�ɖ߂�D�_�u���o�b�t�@�X�C�b�`)
	
	S1. VF���W�X�^�CVI���W�X�^���X�^�b�N�o�b�t�@�ɑޔ�
	S2. �^����ꂽ�|���S��(TriangleFan�ɂ݂��Ă�)���C
            �ؒf���[�`��(T1)�ɓn���ĕ���
        S3. �ؒf�ʂU��(+x,-x,+y,-y,+z,-z)�̏������I��܂ŌJ��Ԃ��D
	S4. �ŏI�I�ɂł���TriangleFan�𓧎��ϊ���XGKICK
	S5. ���C�����[�`��11.�ɖ߂�

        T1. �e���_���g���[�X���C���̊Ԃɐؒf�ʂƂ̌�_�����邩����
	T2. �ӂ̏�Ԃ���C�[�_��������TriangleFan���X�g�ɓo�^
            �E�ӂ̒[�_A�CB�Ƃ��O���Ȃ�o�^���Ȃ��D
            �E�ӂ̒[�_A�CB�Ƃ������Ȃ�A��o�^�D
            �E�ӂ̒[�_A���O�CB�����Ȃ�C��_��o�^
            �E�ӂ̒[�_A�����CB���O�Ȃ�CA�ƌ�_�̂Q��o�^
        T3. �ӂ�������邾���J��Ԃ��C�V����TriangleFan���X�g��
            �쐬���ꂽ�Ƃ����S2�ɖ߂�D

