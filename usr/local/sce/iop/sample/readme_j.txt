[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

                                                          �T���v���̍\��
========================================================================

�T���v���f�B���N�g���\��
------------------------------------------------------------------------
(*)��̃T���v���͍���ǉ����ꂽ�f�B���N�g���ł�

sce/iop/sample/
����cdvd
��  ����smp_iop
��  ����stmadpcm
��  ����stmread
��  ����stmspcm
����hello
����ilink
����kernel
�b  ����module
��  ����thread
����sif
�b  ����sifcmd
��  ����sifrpc
����sound
�b  ����ezadpcm
�b  ����ezbgm
�b  ����ezmidi
�b  ����sqhard
�b  ����sqsoft
��  ����voice
����OLD
��  ����spu2
��  	 ����autodma
��	 ����seqplay
��	 ����stream
��	 ����voice
����usb
    ����usbdesc				(*)
    ����usbkeybd
    ����usbmouse


�T���v���ꗗ
------------------------------------------------------------------------
�T���v���R�[�h�ɂ͈ȉ��̂��̂�����܂��B

cdvd:
	cdvd/smp_iop	IOP���� CD/DVD-ROM Drive �R�}���h�֐���
			�Ăяo���T���v��
        cdvd/stmadpcm	ADPCM�̃X�g���[�~���O�Đ��T���v��
        cdvd/stmread	CD/DVD�h���C�u����X�g���[���֐��𗘗p����
			�t�@�C����ǂݍ��ރT���v��
        cdvd/stmspcm	�X�g���[�gPCM �̃X�g���[�~���O�Đ��T���v��

hello:
	hello		"hello !" ��\������T���v��

ilink:
	ilink		i.LINK(IEEE1394)�ŒʐM���s�Ȃ��T���v��

kernel:
	kernel/module	�풓���C�u�������W���[���̍쐬���@�T���v��
	kernel/thread	�X���b�h�̐����ƋN���A�X���b�h�̃v���C�I���e�B��
			����A�X���b�h�Ԃ̓������s���T���v��

sif:
	sif/sifcmd	SIF CMD �v���g�R���̃T���v��
	sif/sifrpc	SIF RPC �v���g�R���̃T���v��

sound:
	sound/ezadpcm	�f�B�X�N����� ADPCM �f�[�^�X�g���[�~���O�ɂ�� 
			BGM �Đ�����T���v��
	sound/ezbgm	�f�B�X�N���特���t�@�C���iWAV�j���X�g���[�~���O
			�Đ�����T���v��
	sound/ezmidi	MIDI �ɂ��y�Ȃƌ��ʉ��̉��t�T���v��
	sound/sqhard	�n�[�h�E�F�A�E�V���Z�T�C�U�ɂ��MIDI�Đ��T���v��
	sound/sqsoft	�\�t�g�E�F�A�E�V���Z�T�C�U�ɂ��MIDI�Đ��T���v��
	sound/voice	�{�C�X�����T���v��

OLD/spu2:
	�ilibspu2���g�p�����T���v���j
	spu2/autodma	�b��T�E���h���C�u�����ilibspu2�j ��AutoDMA�]��
			�@�\���g���āA�X�g���[�gPCM���͂ɂ�锭�����s��
			�T���v��
	spu2/seqplay	�b��T�E���h���C�u�����ilibspu2, libsnd2�j��
			�g���āAIOP���MIDI�V�[�P���X�̍Đ����s���T���v��
	spu2/stream	�b��T�E���h���C�u�����ilibspu2�j���g���āAIOP���
			SPU2�̃{�C�X�ɂ��X�g���[���Đ����s���T���v��
	spu2/voice	�b��T�E���h���C�u�����ilibspu2�j���g���āAIOP���
			SPU2�̃{�C�X�������s���T���v��

usb:
	usb/usbdesc     USB�f�o�C�X�̐ÓI�f�B�X�N���v�^���_���v����T���v��
	usb/usbkeybd	USB�L�[�{�[�h�̃T���v���h���C�o
	usb/usbmouse	USB�h���C�o(USBD)�̎g�����̃T���v��


�T���v���R���p�C���̑O�ɍs���Ă�������
------------------------------------------------------------------------
���ɂ���܂���B
