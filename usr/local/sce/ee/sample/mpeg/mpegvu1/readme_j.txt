[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

"PlayStation" �p MPEG2 �X�g���[��(PSS)�̃f�R�[�h�� VU1 ���g�����F�ϊ�


<�T���v���̉��>
	mpegvu1 �́A"PlayStation" �p MPEG2 �X�g���[��(PSS) �̍Đ��T��
	�v���v���O�����ł��Bmpegvu1 �́AIPU �ŐF�ϊ� (YCbCr->RGB)
	���s�킸�� VU1 ��p���ĐF�ϊ����s���܂��B

	IPU �̐F�ϊ��@�\�͐F��(CbCr)�����Ɋւ��ẮA�ׂ荇�� 4 �_��
	�����l�Ƃ��Ĉ����܂��Bprogressive frame �̏ꍇ�A�c�����A
	�������Ƃ��ɓ������ԂɃT���v�����O���ꂽ�l�������߁A����
	�����ł��قƂ�ǖ��͂���܂���B

	�Ƃ��낪�Ainterlace frame ����� field �\���̃s�N�`���̏ꍇ��
	�\�����ꂽ���A���Ԃ̈قȂ�t�B�[���h���c�����Ɍ��݂ɔz�u�����
	���B���̂Ƃ��ɓ��l�̐F���ϊ�������ƊG�ɂ���Ăׂ͍����F����/
	�Ȗ͗l�Ƃ��������ۂ������N�����܂��B

	mpegvu1 �́A�F�ϊ��� IPU ��p�����AVU1 �ō����x�̐F�ϊ�
	�����������T���v���v���O�����ł��BMPEG2 �̃f�R�[�h�̍ۂɁA
	VU1 ���󂢂Ă���ꍇ�́A���̕��@�ɂ��������� MEPG2 �f�R�[�h
	�摜�𓾂邱�Ƃ��o���܂��B

	mpegvu1 �ł́A���̗v���𖞂����f�����Đ����܂��B

	- �f�� -

	    Main Profile at Main Level(MP@ML)
	    Simple Profile at Main Level(SP@ML)
	    �ő�T�C�Y               :720x480

	mpegvu1 �ł̓s�N�`���\�������̂悤�ɕ��ނ��A���ꂼ��ʂ�
	VU1 �}�C�N���R�[�h�ŐF�ϊ������s���܂��B

	    (1) progressive frame structure
	    (2) interlace frame structure
	    (3) field structure

	�r�b�g�X�g���[������̂R�̂����ǂ�ɊY�����邩�́A
	�f�R�[�h�� sceMpeg �\���̂̃����o���Q�Ƃ��邱�Ƃɂ��
	�m�邱�Ƃ��o���܂�(videodec.c �ɂ����� csct ���Z�o����
	�������Q��)�B
	
	mpegvu1 �̓z�X�g�̃n�[�h�f�B�N�h���C�u�܂��́ADTL-T10000
	�� CD/DVD �h���C�u�ɒu���ꂽ PSS �t�@�C�����Đ����܂��B�Đ�
	���Ƃ̃f�o�C�X�̓t�@�C�����̐擪�̃f�o�C�X��(host0:/cdrom0:)
	�Ŏw�肵�܂��B

	pause/resume ���s�����ꍇ�A�����Ɋւ��Ă͍ő�� 512 �T���v��
	(48KHz �� 0.0107 sec) ��������\��������܂��B

	�v���O�����̍\���̓T���v���v���O���� mpegstr �Ɠ��l�ł��B
	�ڂ����� mpegstr ���Q�Ƃ��Ă��������B
	    
<�t�@�C��>
	audiodec.c
	audiodec.h
	cscvu1.c
	cscvu1.h
	defs.h
	disp.c
	disp.h
	main.c
	read.c
	readbuf.c
	readbuf.h
	strfile.c
	strfile.h
	util.c
	vibuf.c
	vibuf.h
	videodec.c
	videodec.h
	vobuf.c
	vobuf.h
	vu1.dsm
	yuvfl.dsm
	yuvfl.vsm
	yuvfrfl.dsm
	yuvfrfl0.vsm
	yuvfrfl1.vsm
	yuvprg.dsm
	yuvprg0.vsm
	yuvprg1.vsm

<�N�����@>
	% make		�F�R���p�C��
	% make run	�F���s(�����t��)
	% make noaudio	�F���s(�����Ȃ�)
	% make help     : help ���\��(�t�@�C�����̎w����@���m�F�ł��܂�)

	�z�X�g�̃n�[�h�f�B�X�N�h���C�u�� PSS �t�@�C��������ꍇ�́A�Ⴆ��
	�ȉ��̂悤�Ɏ��s���܂��B

	% dsreset 0 0
	% dsedb
	dsedb S> run main.elf 'host0:XXX.pss�f
'
	�܂��ADTL-T10000 �� CD/DVD �h���C�u�� PSS �t�@�C��������ꍇ�́A�Ⴆ��
	�ȉ��̂悤�Ɏ��s���܂��B

	% dsreset 0 0
	% dsedb
	dsedb S> run main.elf 'cdrom0:\\XXX.PSS;1�f

<�R���g���[���̑�����@>

	���{�^��  : ���߂���Đ�
	�~�{�^��  : �I�� 
	���{�^��  : pause/resume

<���l>
	(a) DTL-T10000 ��p���ăz�X�g������X�g���[�~���O����ۂ�
	    �́A�l�b�g���[�N�̕��ׂɂ��Đ�������邱�Ƃ�����܂��B
	    �l�b�g���[�N����ăX�g���[�~���O����ۂɂ́A
	    ���̓_�ɂ����ӂ��������B

		100Base-T �ڑ��ɂ���
		�n�u������Ȃ�

	(b) /usr/local/sce/data/movie/sample.pss �̃T���v���f�[�^�́A
	    �摜�̈ꕔ�ɗ��ꂪ����܂��B


