[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

EzBGM - �f�B�X�N����̃X�g���[�~���O�ɂ��BGM�Đ�


<�T���v���̉��>
	wav�t�@�C�����f�B�X�N����X�g���[�~���O���čĐ����܂��B2��wav�t�@
	�C���𓯎����񓯊��ɍĐ��ł��܂��B�e�X�g���[���̍Đ��̓R���g���[��
	�ő���ł��܂��B

	���̃T���v���ł̓z�X�g�̃n�[�h�f�B�X�N�܂��̓��[�J����CD/DVD�f�B�X�N
	����X�g���[�~���O���s�Ȃ��܂��B�l�b�g���[�N�ɐڑ����ꂽDTL-T10000��
	�g�p����ꍇ�A�l�b�g���[�N�̒x���ɂ���ăX���[�Y�ɍĐ�����Ȃ����Ƃ�
	�����悤�ł��B

	�g�p�ł���wav�t�@�C����Windows�`���ŁA16bit, 48KHz, �����k, �X�e���I
	���̓��m���� �ł��B�X�e���I�����m�������̓w�b�_��񂩂画�ʂ��āA�Đ�
	���[�h��؂�ւ��Ă��܂��B

	�X�e���I��wav�t�@�C���́A1�T���v������L/R���C���^�[���[�u����Ă��܂�
	���ASPU2�Ŏg�p����ɂ͂�512byte��L/R�C���^�[���[�u����Ă���K�v������
	���߁A���̂܂܂ł͍Đ��ł��܂���B���̃T���v���ł�IOP���g�p���āAwav��
	PCM��SPU2�`����PCM�ɕ��בւ��Ă܂��BIOP�̕��ׂ�1�t�@�C���ɂ�2%�ق�
	�ł��B

	���̃T���v���́AIOP���̃T���v���ƑΉ����Ă���A����������ċ@�\���܂��B
	EE���̃v���O�����������I��IOP�������[�h���܂��̂ŁA�O������IOP���̃�
	�W���[���iiop/sample/sound/ezbgm)���r���h���Ă����Ă��������B

<�t�@�C��>
	main.c	
	bgm_rpc.c
	bgm_i.h
	m_stereo.wav
	ps_mono.wav

<�N�����@>
	% make		�FIOP���̃R���p�C���iiop/sample/sound/ezbgm/)
	% cd  ???	�FEE���̃f�B���N�g���Ɉړ��iee/sample/sound/ezbgm/)
	% make		�FEE���̃R���p�C��
	% make run	�F���s

	�ȏ�ŁA�z�X�g��HD����WAV�t�@�C����ǂ�ŉ��t���܂��B

	���[�J����CD/DVD�h���C�u��WAV�t�@�C��������ꍇ�́A�ȉ��̂悤���s���܂��B
	% dsreset 0 0
	$ dsedb
	dsedb S> run main.elf 'cdrom0:\\M_STEREO.WAV;1' 'cdrom0:\\PS_MONO.WAV;1'

	dsedb�̃R�}���h���C���ł́A�ȉ��̂悤�Ɏ��s���܂��B�i�z�X�gHD�̏ꍇ�j
	
	dsedb S> run main.elf host0:m_stereo.wav host0:ps_mono.wav

	�P�܂��͂Q��wav�t�@�C���������Ɏw�肵�܂��B


<�R���g���[���̑�����@>
        ���{�^��     :   1�Ԗڂ�wav�t�@�C���̉��t�J�n
        ���{�^��     :   1�Ԗڂ�wav�t�@�C���̉��t��~
        �~�{�^��     :   2�Ԗڂ�wav�t�@�C���̉��t�J�n
        ���{�^���@�@ :   2�Ԗڂ�wav�t�@�C���̉��t��~
        START�{�^��  :   ���t���̃t�@�C���̈ꎞ��~�E�ĊJ
	SELECT�{�^�� :   �v���O�����I��

<���l>
	�Ȃ�

