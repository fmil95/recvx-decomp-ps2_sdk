[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

MFIFO �T���v��

<�T���v���̉��>

	���̃v���O������MFIFO��p���ăI�u�W�F�N�g�̕`����s���T���v��
	�ł��BSPR��Memory �y�� Memory��GIF �Ƃ��� DMA �]�����s���ASPR
	�� DoubleBuffer ���AMemory �� RingBuffer ���s�Ȃ��Ă��܂��B
	�܂��AMFIFO�̈��.cmd�t�@�C����ҏW����MFIFO�̈�p��section��
	�m�ۂ��Ă��܂��B

<�t�@�C��>
	mfifo.cmd

<�N�����@>

	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run mfifo.elf

<�R���g���[���̑�����@>

	��,�~�{�^��		�F�I�u�W�F�N�g�̑���
	��,���{�^��		�F�I�u�W�F�N�g�T�C�Y�̑���
	R1,R2�{�^��		�F�I�u�W�F�N�g�Ԋu�ύX
	��,���{�^��		�F�I�u�W�F�N�g�ʒu�w���ړ�
	��,���{�^��		�F�I�u�W�F�N�g�ʒu�x���ړ�
	L1,L2�{�^��		�F�I�u�W�F�N�g�ʒu�y���ړ�
	start�{�^��		�F�|�[�Y
	select+��,�~�{�^��	�F�I�u�W�F�N�g���̏�����
	select+R1�{�^��		�F�I�u�W�F�N�g�Ԋu���O�ɐݒ�
	select+R2�{�^��		�F�I�u�W�F�N�g�Ԋu������
	select+��,���{�^��	�F�I�u�W�F�N�g�ʒu�w��������
	select+��,���{�^��	�F�I�u�W�F�N�g�ʒu�x��������
	select+L1,L2�{�^��	�F�I�u�W�F�N�g�ʒu�y��������
	select+start�{�^��	�F�I�u�W�F�N�g�ʒu������
				�@�i�w�A�x�A�y���j

<���l>
	��MFIFO���g�p�����ł̒��ӎ���

 -	�����O�o�b�t�@�̃T�C�Y(MFIFO_SIZE)�� 2��n�� qword �łȂ���
	�΂Ȃ炸�AD_RBSR�ɂ�MFIFO_SIZE-0x10���w�肵�܂��B���ۂɂ́A
	MFIFO_SIZE��0x00200000��0x00008000�̂悤�ȃT�C�Y�ŁAD_RBSR
	�ɂ�0x001FFFF0��0x00007FF0��ݒ肵�܂��B

 -	�����O�o�b�t�@�̈��MFIFO_SIZE�A���C�����g�ɑ����܂��B����́A
	(�`)global�ϐ��Ƃ��ė̈���m�ۂ���ۂɃA���C�����g���w�肷��
	(�a)kernel�Ń��[�U�[�ɗ��p��������Ă���̈���g�p����
	(�b).cmd�t�@�C����ҏW���ăo�b�t�@�̈�p��section���m�ۂ���
	�Ȃǂ̕��@������܂��B

	(�`)	char base[SIZE] __attribute__ ((aligned(SIZE)));
	��L�̂悤��aligned�Ŏw�肵�܂����Aaligned�̐����Ƃ���SIZE��
	32KB(0x8000)���ő�l�ƂȂ�܂��B�]����32KB�ȏ�̃����O�o�b�t�@
	���g�p����ꍇ�͈ȉ��̕��@��p���܂��B

	(�a)	char* base = (char*)0x00100000;
	��L�̂悤�ɁA���[�U�[���p�\�̈�(0x00100000�`0x00200000)
	���|�C���^�w�肵�ė̈���g�p���܂��B���[�U�[�̓��[�U�[���p
	�\�̈���Ǘ�����K�v������܂��B

	(�b)	.mfifo		ALIGN(0x40000): { *(.mfifo) }
	��L�̂悤�ɁA.cmd�t�@�C���ɃZ�N�V������ǉ����A
		char base[0x40000] __attribute__ ((section(".mfifo")));
	�̂悤�ɃZ�N�V�������w�肵�ė̈���m�ۂ��܂��B

 -	D_RBOR��D_RBSR��&��������l�͕K��0�ɂȂ�Ȃ���΂Ȃ�܂���B
	����͏�L�̏����𖞂����ΕK�R�I�ɖ�������܂��B���ۂɂ́A
	MFIFO_SIZE��0x00040000�̂Ƃ��AD_RBOR��0x00240000�AD_RBSR��
	0x0003FFF0�̂悤�ɂȂ�܂��B

 -	drain�`���l����STR bit�́AD_RBOR�AD_RBSR�A�y��D_CTRL.MFD��
	�ݒ��Asource�`���l��(DMA8)���M�̑O�ɗ��ĂĂ����܂��B

 -	drain�`���l����STR bit�́AENDtag������ꂽ�痎���܂��B

 -	MFIFO�֘A�̃��W�X�^��QWC�ȊO��byte�P�ʂł����AQWC��qword�P��
	�Ȃ̂Œ��ӂ��K�v�ł��B

 -	source�`���l����MFIFO�ɓ]���\���̔���́A
	remain = (tadr==madr) ? MFIFO_SIZE
		: (tadr+MFIFO_SIZE-madr)&(MFIFO_SIZE-0x10);
	�ɂ���ď����݉\�̈�(remain)�̗e�ʂ����߁Aremain���]������
	���Ƃ��Ă���source�`���l���̃f�[�^�ʂ����傫���Ȃ������ł��B

	��drain�`���l����GIF�o�R(path3)�Ƃ���ꍇ�̒��ӎ���

 -	sceGsSwapDBuff��GIF�o�R��DoubleBuffer�̐؂�ւ����s�Ȃ��̂�
	D_CTRL.MFD��0�ɖ߂�(MFIFO���I������)�K�v������܂��B�����
	�����čēxMFIFO���g�p����O��D8_MADR�AD2_TADR�AD2_CHCR�Ȃ�
	�̓��e���Đݒ肷��K�v������܂��BVIF1�o�R(path2)�̏ꍇ�́A
	MFIFO�̏�Ԃɂ����܂܂ɏo���܂��B

	�����̑��̒��ӎ���

 -	sceGsSwapDBuff��GIF�o�R��DoubleBuffer�̐؂�ւ����s�Ȃ��̂ŁA
	sceGsDBuff�ϐ���Cache�ɏ��ꍇ�͒l��ύX�������ƁASyncDCache
	�Ȃǂ�Cache��WriteBack����K�v������܂��B

