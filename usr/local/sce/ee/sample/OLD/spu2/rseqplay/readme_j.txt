[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

MIDI�V�[�P���X�Đ��̃T���v��

<�T���v���̉��>
	�b��T�E���h���C�u���� librspu2 ���g���āAEE�ォ��MIDI�V�[�P���X��
	�Đ����s�Ȃ��܂��B

	main.c ��define�s��
		SEQ_FILENAME ��SEQ�t�@�C�����A
		VH_FILENAME  ��vh�t�@�C�����A
		VB_FILENAME  ��vb�t�@�C�����w�肵�Ă��������B

	�����̃t�@�C���t�H�[�}�b�g�́A���sPlayStation�ƑS�������Ȃ̂ŁA
	���s�c�[�����g�p���Đ������Ă��������B

<�t�@�C��>
	main.c	
	fuga.seq
	simple.vh
	simple.vb

<�N�����@>
	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run main.elf

	fuga���s�A�m�̉��ŉ��t�����ΐ���B

<�R���g���[���̑�����@>
	�Ȃ�

<���l>
	�Ȃ�
