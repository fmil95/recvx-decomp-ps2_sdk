[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

"PlayStation" �p�����t�� MPEG2 �X�g���[��(PSS)�̍Đ�


<�T���v���̉��>
	mpegstr �́A"PlayStation" �p�����t�� MPEG2 �X�g���[��(PSS)
	�̍Đ��T���v���v���O�����ł��Bmpegstr �ł́A���̗v���𖞂���
	�f���y�щ������Đ����܂��B

	- �f�� -

	    Main Profile at Main Level(MP@ML)
	    Simple Profile at Main Level(SP@ML)
	    �ő�T�C�Y               :720x480
	    �s�N�`���\��             :�t���[���\��(�v���O���b�V�u
	                                           �t���[�����]�܂���)
	    
	- ���� -
	    
	    �`��                      :�X�g���[�gPCM
	    �T���v����                :16bit
	    �G���f�B�A��              :���g���G���f�B�A��
	    �T���v�����O���[�g        :48.0KHz
	    �`���l����                :2
	    (L/R)�C���^�[���[�u�T�C�Y :512

	mpegstr �̓z�X�g�̃n�[�h�f�B�N�h���C�u�܂��́ADTL-T10000
	�� CD/DVD �h���C�u�ɒu���ꂽ PSS �t�@�C�����Đ����܂��B�Đ�
	���Ƃ̃f�o�C�X�̓t�@�C�����̐擪�̃f�o�C�X��(host0:/cdrom0:)
	�Ŏw�肵�܂��B

	�f���Ɖ����̑��d���ɂ́A�X�g���[���R���o�[�^ ps2str ���g�p����
	���B�{�v���O�����̍Đ�����X�g���[���̃X�g���[���ԍ���
	�f��/�������� 0 �ł���K�v������܂��B���d���̂Ƃ��ɂ́A�X�g
	���[���ԍ��� 0 ���w�肵�܂��B

	�v���O�����́A�ǂݍ��݂ƃf�R�[�h���ʃX���b�h�ɂȂ��Ă��܂��B
	���C���X���b�h�� sceRead()/sceCdStRead() �œǂݍ��݂��s����
	�ŁA�����ЂƂ̃f�R�[�h�X���b�h�� MPEG2 �̃f�R�[�h���s���܂��B

	�����͉f���Ƌ��� EE �ɓǂݍ��݁A�񑽏d��������� IOP ���ɖ߂�
	�܂��B�{�T���v���v���O�����ł́A�f��/�����̃^�C���X�^���v��
	�`�F�b�N���Ă��܂���B���̂��߁A���������ꂽ�ꍇ�̕��A�����
	�g�ݍ��܂�Ă��܂���B�{�v���O�����ł́A�f��/�����̐擪������
	������A�Đ����J�n���܂��B�f���̍Đ����x�� vblank �����߂܂��B
	�����̍Đ����x�́ASPU2 �̍Đ����x�����߂܂��B���҂ɂ͋͂��Ȍ�
	�������邱�Ƃ�����܂����A�ʏ�̍Đ��ɂ����Ă͖��ɂȂ�Ȃ���
	�x�ł��B

	pause/resume ���s�����ꍇ�A�����Ɋւ��Ă͍ő�� 512 �T���v��
	(48KHz �� 0.0107 sec) ��������\��������܂��B

	read ���Ԃɍ���Ȃ��A�܂��́A�f�R�[�h���Ԃɍ���Ȃ����̗��R��
	���r���̃o�b�t�@���A���_�[�t���[�����ꍇ�ɂ́A����܂łɃf
	�R�[�h���ꂽ�f�[�^���J��Ԃ��Đ�����܂��B���Ȃ킿�A�f����
	�ւ��ẮA�Ō�Ƀf�R�[�h���ꂽ�G�����̂܂ܕ\������܂��B����
	�Ɋւ��Ă̓o�b�t�@�Ɏc���Ă���f�[�^���J��Ԃ��Đ�����܂��B
	���̂Ƃ��A�����Ɖf���̓����������\��������܂��B�܂��A�o�b
	�t�@����t�ł������A�V���ȃf�[�^�̎�荞�݂����܂���̂ŁA
	�o�b�t�@���I�[�o�[�t���[���邱�Ƃ͂���܂���B

	�֐� sceMpegGetPicture() �́A������ SPR(Scratch Pad RAM) ��
	�g�p���܂��B���̂��߁A���� SPR ���g���ꍇ�ɂ�
	sceMpegGetPicture() �̊Ԃ����� SPR ���J������K�v������܂��B

<�t�@�C��>
	audiodec.c
	audiodec.h
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

<�N�����@>
	% make		: �R���p�C��
	% make run	: ���s(�����t��)
	% make noaudio  : ���s(�����Ȃ�)
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
	���{�^���@: ���߂���Đ�
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


