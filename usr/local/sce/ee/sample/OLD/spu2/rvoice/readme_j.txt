[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�{�C�X�����̃T���v��

<�T���v���̉��>
	�b��T�E���h���C�u���� librspu2 ���g���āAEE�ォ��SPU2�̃{�C�X����
	���s�Ȃ��܂��B�I�v�V�����ŁADMA�R�[���o�b�N�A�����IRQ�R�[���o�b�N
	�̎������ł��܂��B

	���F�f�[�^������������ɂ́Amain.c ��define�s�� VAG_FILENAME ��
	VAG�t�@�C�����AVAG_SIZE�Ƀt�@�C���T�C�Y���w�肵�Ă��������BVAG�t�@
	�C���̃t�H�[�}�b�g�́A���sPlayStation�ƑS�������Ȃ̂ŁA���s�c�[��
	���g�p���č쐬���Ă��������B

	API�̎g�����́A�قڌ��sPlayStation�Ɠ��l�ł����ASPU�Q�ł̓R�A�̑I
	�����K�v�ł��B�R�A0�̏o�͂̓R�A1�̃��C���{�����[����ʂ�i�܂�R
	�A1�̃��C���{�����[�����[���̏ꍇ�̓R�A0�̉�����������Ȃ��j���Ƃ�
	���ӂ��Ă��������B

	IRQ_CB_TEST  ��1�ɃZ�b�g����ƁAIRQ�R�[���o�b�N���L���ɂȂ�AVAG��
	�擪����0x1000�Ԓn�ځiIRQ_ADDR_OFST�j���A�N�Z�X���ꂽ�Ƃ��A
	"IRQ interrupt detected"�̃��b�Z�[�W���R���\�[���ɏo�͂���܂��B

	DMA_CB_TEST  ��1�ɃZ�b�g����ƁADMA�]���I���R�[���o�b�N���L���ɂ�
	��AVAG�t�@�C���̓]�����I�������Ƃ���"DMA interrupt detected" �̃�
	�b�Z�[�W���R���\�[���ɏo�͂���܂��B

<�t�@�C��>
	main.c	
	piano.vag

<�N�����@>
	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run main.elf

	Piano��1�I�N�^�[�u8���̉��K�����������ΐ���B

<�R���g���[���̑�����@>
	�Ȃ�

<���l>
	�Ȃ�
