[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�z�X�g�}�V���̃t�@�C������T���v��

<�T���v���̉��>
	�z�X�g�}�V���̃t�@�C�����A�N�Z�X����T���v���v���O�����ł��B

	1. �t�@�C�� main.c �̓��e���R�s�[���� �t�@�C�� copy.c ���쐬
	2. 4�o�C�g���̃C���N�������^���f�[�^���쐬���A����� incdata ��
	   �����t�@�C���ɏ������ށB�ēx�Aincdata ��ǂݏo���Đ�����������
	3. indata ��񓯊����[�h�œǂݏo���Đ������ǂ߂Ă��邩�����؁B

<�t�@�C��>
	main.c

<�N�����@>
	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run main.elf

	sample end.
	�ƕ\�������ΐ���I���B
	�t�@�C�� copy.c �� �t�@�C�� main.c �Ɠ��t�@�C�������m�F�B
	% diff main.c copy.c

<�R���g���[���̑�����@>
	�Ȃ�

<���l>
	�Ȃ�
