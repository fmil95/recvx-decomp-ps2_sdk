[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

IOP���X�g���[���֐��̌Ăяo���T���v��

<�T���v���̉��>
	libcdvd�̃X�g���[���֐��e�X�g�v���O�����ł��B

	�P�ECD/DVD-ROM�֌W�̃C�j�V�����C�Y
	�Q�E�X�g���[���֐��̌Ăяo��
	�R�E�X�g���[�gPCM�̉��t

	�����ɍs���܂��B

<�t�@�C��>
	main.c
        wav2int.c 	Wavfile��PS2_PCM_Raw_Format�ɕϊ����s��linux�T���v���v
		        ���O����

<�N�����@>
	% dsreset 0 0
	% make		�F�R���p�C��

          CD/DVD-ROM Drive�ɃT���v���f�[�^"M_STEREO.INT"���������񂾃��f�B�A��
        �Z�b�g���܂��B
	�E�C���h�E���I�[�v�����܂��B
	  ��̃E�C���h�E��dsidb�𗧂��グ������̃E�C���h�E�ɂ�
        ���L�̗v�̂ɂăv���O���������s���܂��B
	% make run �v���O���������s���܂��B

	dsidb�̃E�C���h�E��

	sample end.

	�ƕ\������X�g���[�gPCM�̉��t���s���Ă���ΐ���I���ł��B


<�R���g���[���̑�����@>
	�Ȃ�

<���l>	
	"PlayStation 2"�̃X�g���[�gPCM�f�[�^�ւ̃f�[�^�R���o�[�^�̃T���v��
	�Ƃ���wav2int.c��Y�t�������܂��B

