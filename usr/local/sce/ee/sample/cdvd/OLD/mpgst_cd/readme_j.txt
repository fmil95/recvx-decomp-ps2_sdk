[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

"PlayStation" �p�����t�� MPEG2 �X�g���[��(PSS)�̍Đ�
				CD/DVD�h���C�u����̃X�g���[�~���O�d�l


<�T���v���̉��>
	���̃T���v�� �́A"PlayStation" �p�����t�� MPEG2 �X�g���[��(PSS)
	�̍Đ��T���v���v���O�����ł��Bmpgst_cd �ł́A���̗v���𖞂���
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
	    �`�����l����              :2
	    (L/R)�C���^�[���[�u�T�C�Y :512

	�f���Ɖ����̑��d���ɂ́A�X�g���[���R���o�[�^ ps2str ���g�p����
	���B�{�v���O�����̍Đ�����X�g���[���̃X�g���[���ԍ���
	�f��/�������� 0 �ł���K�v������܂��B���d���̂Ƃ��ɂ́A�X�g
	���[���ԍ��� 0 ���w�肵�Ă��������B

	�v���O�����́A�ǂݍ��݂ƃf�R�[�h���ʃX���b�h�ɂȂ��Ă��܂��B
	���C���̃X���b�h�� sceRead() �œǂݍ��݂��s�����ŁA�����ЂƂ�
	�̃X���b�h�� MPEG2 �̃f�R�[�h���s���܂��B

	�����͉f���Ƌ��� EE �ɓǂ݁A�񑽏d��������� IOP ���ɖ߂��܂��B
	�{�T���v���v���O�����ł́A�f��/�����̃^�C���X�^���v���`�F�b�N
	���Ă��܂���B���̂��߁A���������ꂽ�ꍇ�̕��A����͑g�ݍ��܂�
	�Ă��܂���B�{�v���O�����ł́A�f��/�����̐擪�����킹����A��
	�����J�n���܂��B�f���̍Đ����x�� vblank �����߂܂��B�����̍Đ�
	���x�́ASPU2 �̍Đ����x�����߂܂��B���҂͋͂��Ȍ덷�����邱��
	������܂����A�ʏ�̍Đ��ɂ����Ă͖��ɂȂ�Ȃ����x�ł��B

	read ���Ԃɍ���Ȃ��A�܂��́A�f�R�[�h���Ԃɍ���Ȃ����̗��R��
	���r���̃o�b�t�@���A���_�[�t���[�����ꍇ�ɂ́A����܂łɃf
	�R�[�h���ꂽ�f�[�^���J��Ԃ��Đ�����܂��B���Ȃ킿�A�f����
	�ւ��ẮA�Ō�Ƀf�R�[�h���ꂽ�G�����̂܂ܕ\������܂��B����
	�Ɋւ��Ă̓o�b�t�@�Ɏc���Ă���f�[�^���J��Ԃ��Đ�����܂��B
	���̂Ƃ��A�����Ɖf���̓����������\��������܂��B�܂��A�o�b
	�t�@����t�ō݂����A�V���ȃf�[�^�̎�荞�݂����܂���̂ŁA
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
	util.c
	vibuf.c
	vibuf.h
	videodec.c
	videodec.h
	vobuf.c
	vobuf.h

<�N�����@>
	% make		�F�R���p�C��

        ���[�J����CD/DVD�h���C�u��MPG�t�@�C���̑��݂���CD/DVD-ROM��}�����܂��B
	�R���\�[�����ȉ��̎菇�ɂĎ��s���܂��B
        % dsreset 0 0
        $ dsedb
        dsedb S> run main.elf '\\MPG�t�@�C����;1'

<�R���g���[���̑�����@>

	���{�^�� : ���߂���Đ�
	�~�{�^�� : �I�� 

<���l>
	(a) ps2str ver1.00 �ł́A��r�b�g���[�g(CBR)�ŃG���R�[�h
	    ���ꂽ�f���݂̂̑��d���ɑΉ����Ă��܂��B�σr�b�g
	    ���[�g(VBR)�ŃG���R�[�h���ꂽ�f���͐��������d���ł�
	    �܂���BVBR �ɂ́A�ȍ~�̃o�[�W�����őΉ����܂��B

