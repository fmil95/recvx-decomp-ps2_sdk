[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�{�C�X�����̃T���v��

<�T���v���̉��>
	libsd�̃T���v���ł��BIOP���SPU2�̃{�C�X������
	�s�Ȃ��܂��B�I�v�V�����ŁA�o�b�`�����ADMA�R�[���o�b�N�A�����IRQ�R
	�[���o�b�N�̎������ł��܂��B

	VAG�t�@�C���̃t�H�[�}�b�g�́A���sPlayStation�ƑS�������Ȃ̂ŁA���s
	�c�[�����g�p���č쐬���Ă��������B

	SPU2�ł̓R�A�̑I���ɒ��ӂ��K�v�ł��B�R�A0�̏o�͂̓R�A1�̃��C���{��
	���[����ʂ�i�܂�R�A1�̃��C���{�����[�����[���̏ꍇ�̓R�A0�̉�
	����������Ȃ��j���Ƃɒ��ӂ��Ă��������B

	BATCH_MODE��1�ɂ���ƁA����܂œƗ���API�ōs�Ȃ��Ă������Ƃ��A�o�b
	�`�����ōs�Ȃ��܂��BEE���������ɏ������ރe�X�g���s���̂ŁAdsedb��
	���グ�āA���������_���v���Ċm�F���Ă��������B

	IRQ_CB_TEST  ��1�ɃZ�b�g����ƁAIRQ�R�[���o�b�N���L���ɂȂ�AVAG��
	�擪����0x1000�Ԓn�ځiIRQ_ADDR_OFST�j���A�N�Z�X���ꂽ�Ƃ��A
	"interrupt detected"�̃��b�Z�[�W���R���\�[���ɏo�͂���܂��B

	DMA_CB_TEST  ��1�ɃZ�b�g����ƁADMA�]���I���R�[���o�b�N���L���ɂ�
	��AVAG�t�@�C���̓]�����I�������Ƃ���"interrupt detected" �̃��b�Z
	�[�W���R���\�[���ɏo�͂���܂��B

<�t�@�C��>
	init_bat.h
	main.c	
	piano.vag

<�N�����@>
	% make		�F�R���p�C��
	% make run	�F���s

	Piano����1�I�N�^�[�u8���̉��K�����������ΐ���B

<�R���g���[���̑�����@>
	�Ȃ�

<���l>
	�Ȃ�
