[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

                                                          �T���v���̍\��
========================================================================

�T���v���f�B���N�g���\��
------------------------------------------------------------------------
(*)��̃T���v���͍���ǉ����ꂽ�f�B���N�g���ł�

sce/ee/sample/
����advanced
��  ����anti
�b       ����aa1-feather			(*)
�b       ����mountain				(*)
�b       ����pcrtc-blend			(*)
�b       ����refmap-448				(*)
�b       ����refmap-4times			(*)
�b       ����refmap-noAA			(*)
�b       ����refmap-onepass-AAline		(*)
�b       ����texmap-onepass-AAline		(*)
����basic3d
��  ����core
��  ����vu0
��  ����vu1
����cdvd
��  ����smp_ee
��  ����OLD
�b       ����mpgst_cd
����deci2
��  ����mouse
��  ����system
����device
��  ����file
����graphics
��  ����anti
��  ����bumpmap
��  ����capture
��  ����clip_vu0
��  ����clip_vu1
��  ����clut
��  ����clut_csa
��  ����framework
�b  �b  ����general
��  ����jointm
�b  �b  ����intr_key
�b  �b      ����mkdata
�b  �b      ����runtime
��  ����mipmap
��  ����point_l	
��  ����refmap
��  ����scis_vu0
��  ����scis_vu1
��  ����spot_l	
��  ����tex_swap
��  ����textrans
�b      ����4BIT
�b      ����8BIT
�b      ����bitconv
����ilink
����ipu
��  ����ezcube
��  ����ezcube3	
��  ����ezmovie
����kernel
��  ����priority				 (*)
��  ����rotate					 (*)
��  ����semaphore				 (*)
��  ����vsync					 (*)
��  ����thread
����mc
��  ����basic
��  ����icon
����mcx
��  ����basic					 (*)
����mpeg
��  ����ezmpeg
�b  ����mpegstr
�b  �b   ����OLD
�b  ����mpegvu1
�b       ����OLD
����pad
��  ����basic  
��  ����dual2
��  ����gun
��  ����mtap
����pc
��  ����dcache
����pkt
��  ����balls
��  ����cube
��  ����mfifo
����scf						 (*)
����sif
��  ����sifcmd
��  ����sifrpc
����sound
��  ����ezadpcm
��  ����ezbgm
��  ����ezmidi
��  ����rblock
��  ����sqsoft
����OLD
��  ����spu2
��  	 ����rautodma
��	 ����rseqplay
��	 ����rstream
��	 ����rvoice
����usb
��  ����usbkeybd
��  ����usbmouse
����vu0
��  ����tballs
��  ����tdiff
��  ����tfog
��  ����tmip
��  ����tskin
��  ����tsquare
����vu1
�@  ����blow
�@  ����bspline
�@  ����deform
�@  ����hako
�@  ����iga

�T���v���ꗗ
------------------------------------------------------------------------
�T���v���R�[�h�ɂ͈ȉ��̂��̂�����܂��B

advanced:
	advanced/anti/aa1-feather	�A���`�G�C���A�V���O���ʂ̃T���v��
	advanced/anti/mountain		�A���`�G�C���A�V���O���C���㏑����
					���A���`�G�C���A�V���O�̃T���v��
	advanced/anti/pcrtc-blend	PCRTC���g�����A���`�G�C���A�V���O
					���ʂ̃T���v��
	advanced/anti/refmap-448	���t���N�V�����}�b�s���O�E�X�y�L��
					���[�T���v���i�u�����{�𑜓x�j
	advanced/anti/refmap-4times	���t���N�V�����}�b�s���O�E�X�y�L��
					���[�T���v���i�T�u�s�N�Z�����炵��
					���A���`�G�C���A�V���O�j
	advanced/anti/refmap-noAA	���t���N�V�����}�b�s���O�E�X�y�L��
					���[�T���v��
	advanced/anti/refmap-onepass-AAline
					���t���N�V�����}�b�s���O�E�X�y�L��
					���[�T���v�� + LINE�ɂ��A���`�G
					�C���A�V���O�i�P�x�����Łj
	advanced/anti/texmap-onepass-AAline
					�e�N�X�`���}�b�s���O�T���v��+ LINE
					�ɂ��A���`�G�C���A�V���O
					�i�P�x�����Łj

basic3d:
        3D�̃I�u�W�F�N�g��\������܂ł́A��{�I�Ȏ葱���𗝉����邽�߂�
        �T���v��
        basic3d/core    cpu �̃R�A�̖��߂݂̂ŋL�q���ꂽ�A���S���Y��
                        ����p�T���v��
        basic3d/vu0     core �̃v���O���������ƂɃ��C���̏����� VU0 
                        �}�N�����߂ɒu���������T���v��
        basic3d/vu1     core�Avu0 �ōs���Ă��鏈�����A����� VU1 
                        �}�C�N�����߂Ŏ��������T���v��

cdvd:
	cdvd/smp_ee		CD/DVD-ROM Drive �R�}���h�֐����Ăяo��
				�T���v��
        cdvd/OLD/mpgst_cd   	CD/DVD�h���C�u�����MPEG2�X�g���[����ǂ�
                        	�o���A�Đ�����T���v�� 
				���ӁF���̃T���v���̋@�\��mpeg/mpegstr
				�T���v���ɑg�ݍ��܂ꂽ�ׁA����X�V����
				�܂���B

deci2:
	deci2/mouse	DECI2 �v���g�R����p�����v���O�����̃_�E�����[�h�A
                        �y�уz�X�g��̃}�E�X�̓������V�~�����[�g����
			�T���v��
	deci2/system    DECI2 �v���g�R����p�����z�X�g��� system() ��
			���s����T���v��

device:
        device/file     �z�X�g�}�V���̃t�@�C������T���v��

graphics:
	graphics/anti		�A���`�G�C���A�X�T���v��(�\�[�g&
				�|���S���N���b�v)
	graphics/bumpmap	�o���v�}�b�s���O�T���v��
        graphics/capture        �t���[���o�b�t�@���e���z�X�gPC��
                                �t�@�C���ɃZ�[�u����T���v��
	graphics/clip_vu0	Clipping �T���v��(VU0) 
	graphics/clip_vu1	Clipping �T���v��(VU1)
        graphics/clut           4bit/8bit�J���[�̃e�N�X�`����\������
				�T���v��
	graphics/clut_csa	4bit �e�N�X�`��CLUT�̈ꎞ�o�b�t�@�ւ�
				���[�h��csa/cld�t�B�[���h�ɂ���Đ��䂷��
				�T���v��
	graphics/framework/general	
				�����O���t�B�b�N�X���́A��{�R���Z�v�g��
				�e��@�\�𐷂荞�񂾁A�������̍\�z��ڎw
				���T���v��
	graphics/jointm/intr_key/mkdata
				��]�����}�ɂ��L�[�t���[���}�g���N�X
				�A�j���[�V�����̃f�[�^�쐬�T���v��
	graphics/jointm/intr_key/runtime
				��]�����}�ɂ��L�[�t���[���}�g���N�X
				�A�j���[�V�����̃����^�C���T���v��
        graphics/mipmap         mipmap���s���T���v��
	graphics/point_l	�_���������������Ȃ��T���v��
	graphics/refmap		�X�y�L�����[�̂���Ɠx�v�Z�A���t���N
				�V�����}�b�s���O���s���T���v��
	graphics/scis_vu0	�V�U�����O�T���v��(CORE&VU0��)
	graphics/scis_vu1	�V�U�����O�T���v��(VU1��)
	graphics/spot_l		�X�|�b�g���C�g����\������T���v��
	graphics/tex_swap	�e�N�X�`�������ւ��Ȃ���`����s��
				�T���v��
	graphics/textrans/4BIT
				4bit-256x256 �̃e�N�X�`���� 32bit-128x64 
				�̃e�N�X�`���Ƃ��ē]������T���v��
	graphics/textrans/8BIT
				8bit-256x256 �̃e�N�X�`���� 32bit-128x128
				�̃e�N�X�`���Ƃ��ē]������T���v��
	graphics/textrans/bitconv
				PSMT4/8 �� raw-image �� PSMCT32 �œ]���\
				�ȃr�b�g�z�u�ɕϊ�����I�t���C���T���v��

ilink:
	ilink		i.LINK(IEEE1394)�ŒʐM���s�Ȃ��T���v��

ipu:
	ipu/ezcube	3�����|���S���� IPU ��p���ăf�R�[�h����
			�e�N�X�`���𒣂�����T���v��
	ipu/ezcube3	�����̈��k�f�[�^���f�R�[�h����T���v��
        ipu/ezmovie     IPU ���g�p��������̊ȈՍĐ��T���v��

kernel:
	kernel/priority		�X���b�h�D��x�𗝉����邽�߂̃T���v��
	kernel/rotate		�X���b�h���f�B�L���[�̉�]�̃T���v��
	kernel/semaphore	VSync���荞�݂ƃZ�}�t�H���g���ăX���b�h��
				����𐧌䂷��T���v��
	kernel/thread		�}���`�X���b�h��p���ăI�u�W�F�N�g�̕`���
				�s�Ȃ��T���v��
	kernel/vsync		VSyncStart���荞�݃n���h���̃T���v��

mc:
	mc/basic	�������[�J�[�h���C�u�����̊�{�@�\�m�F�T���v��
	mc/icon		3D �A�C�R�������̂��߂� Linux ��œ����c�[��

mcx:
	mcx/basic	PDA���C�u�����̊�{�@�\�m�F�T���v��

mpeg:
	mpeg/ezmpeg	IPU ���g�p���� MPEG2(MPEG1���܂�)�X�g���[��
			�Đ��T���v��
	mpeg/mpegstr	�z�X�g���n�[�h�f�B�X�N�y��CD/DVD����"PlayStation"
			�p�����t��MPEG-2 �t�@�C��(PSS �t�@�C��)���Đ�����
			�T���v���ilibsdr���g�p�����T���v���j
	mpeg/mpegstr/OLD
			�z�X�g���n�[�h�f�B�X�N�y��CD/DVD����"PlayStation"
			�p�����t��MPEG-2 �t�@�C��(PSS �t�@�C��)���Đ�����
			�T���v���ilibrspu2���g�p�����T���v���j
        mpeg/mpegvu1	IPU �ł͂Ȃ� VU1 ���g�p���āA�F�ϊ������s����
                        �T���v���ilibsdr���g�p�����T���v���j
        mpeg/mpegvu1/OLD
			IPU �ł͂Ȃ� VU1 ���g�p���āA�F�ϊ������s����
                        �T���v���ilibrspu2���g�p�����T���v���j

pad:
	pad/basic 	�R���g���[������\������T���v��
        pad/dual2       �A�i���O�R���g���[���iDUALSHOCK 2�j�̊�������
			�\������T���v��
        pad/gun		�K���R���g���[���̍��W�����擾����ʏ�ɕ\������
			�T���v��
        pad/mtap	�}���`�^�b�v�o�R�ŃR���g���[���𐧌䂷��T���v��

pc:
	pc/dcache	Performance Counter ��p���� D Cache miss ��
			�J�E���g����T���v��

pkt:
        pkt/balls       �X�v���C�g�`��T���v��
        pkt/cube        �|���S�����f���`��T���v��
	pkt/mfifo	MFIFO��p���ăI�u�W�F�N�g�̕`����s���T���v��

scf:
	scf		"PlayStation 2" ��̃V�X�e���ݒ�ɐݒ肳��Ă���
			�����擾����T���v��

sif:
	sif/sifcmd	SIF CMD �v���g�R���̃T���v��
	sif/sifrpc	SIF RPC �v���g�R���̃T���v��

sound:
	sound/ezadpcm	�f�B�X�N����� ADPCM �f�[�^�X�g���[�~���O�ɂ�� 
			BGM �Đ�����T���v��
	sound/ezbgm	�f�B�X�N���特���t�@�C���iWAV�j���X�g���[�~���O
			�Đ�����T���v��
	sound/ezmidi	MIDI �ɂ��y�Ȃƌ��ʉ��̉��t�T���v��
	sound/rblock	���o�̓u���b�N�]���i����AutoDMA�]���j�T���v��
	sound/sqsoft	�\�t�g�V���Z�T�C�U�ɂ��MIDI�Đ��T���v��

OLD/spu2:
	�ilibrspu2���g�p�����T���v���j
	spu2/rautodma	AutoDMA �]���@�\���g���āA�X�g���[�gPCM ���͂�
			��锭�����s�Ȃ��T���v��
	spu2/rseqplay	MIDI�V�[�P���X�Đ����s�Ȃ��T���v��
	spu2/rstream	�{�C�X�ɂ��X�g���[���Đ����s�Ȃ��T���v��
	spu2/rvoice	�{�C�X�̔������s�Ȃ��T���v��

usb:
	usb/usbkeybd	EE������IOP����USB�L�[�{�[�h�h���C�o�̏����擾
			����T���v��
	usb/usbmouse	EE������IOP����USB�}�E�X�h���C�o�̏����擾����
			�T���v��

vu0:
        VU0���g�p���ăW�I���g���������s�Ȃ��T���v��
        vu0/tballs      �R�����̋��̂����삷��T���v��
        vu0/tdiff       �|���S���̃N���b�v���s�Ȃ��T���v��
        vu0/tfog        �����Ȃ��_�����ƃI�u�W�F�N�g�N���b�v���s�Ȃ�
                        �T���v��
        vu0/tmip        MiMe�̐��`���}��MIPMODEL���s�Ȃ��T���v��
        vu0/tskin       �P�̃��f�����}�g���N�X���g�p���ĕ����\������
                        �T���v��
        vu0/tsquare     �C�����C���A�Z���u�����g�p����VU0�}�N���̃T���v��

vu1:
        VU1���g�p���ăW�I���g���������s�Ȃ��T���v��
        �}�C�N���R�[�h�̋L�q�ɂ�dvpasm�`�����g�p
        vu1/blow        �p�[�e�B�N���`��T���v��
        vu1/bspline     VU1 B-Spline�Ȗʕ`��T���v��
        vu1/deform      �Q�`���ԃT���v��
        vu1/hako        VU1�����ϊ��T���v��
        vu1/iga         VU1�����ϊ��A�����v�Z�T���v��

