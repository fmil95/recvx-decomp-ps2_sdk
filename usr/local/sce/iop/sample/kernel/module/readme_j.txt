[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�풓���W���[���T���v��

<�T���v���̉��>

�풓���C�u�������W���[���̍쐬���@�̃T���v���ł��B

<�t�@�C��>

	mylib.c			�풓���C�u�����̃T���v���\�[�X
	mylib.tbl		�풓���C�u�����̃G���g���e�[�u���̃T���v��
	client.c		�풓���C�u�����𗘗p����v���O�����̃T���v��

<�N�����@>

	% make						�F�R���p�C��
	% dsreset 0 2
	% dsistart mylib.irx; dsistart client.irx	�F���s

	�ȉ��̕��@�ł����s�\
	% dsidb
	> reset 0 2 ; mstart mylib.irx; mstart client.irx
