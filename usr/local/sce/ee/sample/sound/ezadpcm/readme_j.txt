[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

EzADPCM - �f�B�X�N����� ADPCM �f�[�^�X�g���[�~���O�ɂ�� BGM �Đ�


<�T���v���̉��>

	.VB (ADPCM) �t�@�C�����f�B�X�N����X�g���[�~���O���čĐ����܂��B
	2 �� .VB �t�@�C���𓯎��ɍĐ����A�X�e���I�� BGM ���\�����܂��B

	���̃T���v���ł̓z�X�g�̃n�[�h�f�B�X�N�܂��̓��[�J���� CD/DVD 
	�f�B�X�N����X�g���[�~���O���s���܂��B�l�b�g���[�N�ɐڑ����ꂽ
	DTL-T10000 ���g�p����ꍇ�A�l�b�g���[�N�̒x���ɂ���Ă̓X���[
	�Y�ɍĐ�����Ȃ����Ƃ�����悤�ł��B

	�g�p�ł��� .VB (ADPCM) �t�@�C���� 1 �̔g�`�f�[�^�̂݊܂܂��
	���B���̐擪�Ɩ����Ń��[�v���Ă���K�v������܂��B�t�@�C���T�C
	�Y�� SPU2 ���[�J�����������ɐݒ肷��o�b�t�@�T�C�Y�̐����{�ɂȂ�
	�Ă��Ȃ���΂����܂���B

	���̃T���v���́AIOP ���̃T���v���ƑΉ����Ă���A����������ċ@
	�\���܂��BEE ���̃v���O�����������I�� IOP �������[�h���܂��̂ŁA
	�O������ IOP ���̃��W���[�� (iop/sample/sound/ezadpcm) �� make 
	���Ă����Ă��������B

<�t�@�C��>
	main.c	
	rpc.c
	ezadpcm.h

<�N�����@>
	% make		: IOP ���̃R���p�C�� (iop/sample/sound/ezadpcm/)
	% cd ???	: EE ���̃f�B���N�g���Ɉړ� 
			  (ee/sample/sound/ezadpcm/)
	% make		: EE ���̃R���p�C��
	% make run	: ���s

	make run ��ASTART �{�^�����������Ƃɂ��A�z�X�g�̃n�[�h�f�B
	�X�N���� .VB �t�@�C�� (�T���v���� 
	ee/sample/spu2/rstream/tr1l(r)_pad.vb) ��ǂ�� ADPCM �f�[�^����
	�����܂��B

	���[�J���� CD/DVD �h���C�u�� .VB �t�@�C��������ꍇ�́A�Ⴆ��
	�ȉ��̂悤���s���܂��B

	% dsreset 0 0
	% dsedb
	dsedb S> run main.elf 'cdrom0:\\TR1L_PAD.VB;1' 'cdrom0:\\TR1R_PAD.VB;1'

	dsedb�̃R�}���h���C���ł́A�ȉ��̂悤�Ɏ��s���܂� (�z�X�g���n�[
	�h�f�B�X�N�̏ꍇ)�B
	
	dsedb S> run main.elf host0:tr1l_pad.vb host0:tr1r_pad.vb

	�K�� 2 �� .VB (ADPCM) �t�@�C���������Ɏw�肵�܂��B

<�R���g���[���̑�����@>
        ���i�����L�[�j:   �{�����[�����グ��
        ���@�@�@�@�@  :   �{�����[����������
        START�{�^��   :   ���t���̃t�@�C���̊J�n�i�ĊJ�j�E��~
	SELECT�{�^��  :   �v���O�����I��

<���l>
	�Ȃ�

