[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

"PlayStation" �p�����t�� MPEG2 �X�g���[��(PSS)�̍Đ�


<�T���v���̉��>
---------------

    == �T�v ==

    mpegstr �́A"PlayStation" �p�����t�� MPEG2 �X�g���[��(PSS)
    �̍Đ��T���v���v���O�����ł��Bmpegstr �ł́A���̗v���𖞂���
    �f���y�щ������Đ����܂��B

    - �f�� -

	Main Profile at Main Level(MP@ML)
	Simple Profile at Main Level(SP@ML)
	�ő�T�C�Y               :720x576
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


    == GS �̃������ɂ��� ==
	
    �{�v���O�����ł͍ŏ��� GS �̃������̏��������s�Ȃ��A���̌�
    �ʏ�̃_�u���o�b�t�@�̐ݒ���s�Ȃ��Ă��܂��B

    (a) clearGsMem �֐�
	�܂� GS �̃������̏��������s���܂��B
	�������S�̂� 1 �̑傫�ȕ`��̈�Ƃ��Đݒ肵�A1 ���̃X�v���C�g��
	�`�悷�邱�Ƃł�����������Ă��܂��B

		 -----------------------
		|			|
		|			|
		|			|
		|			|
		|			|
		|			|
		|			|
		|			|
		|	Frame		|
		|	 Buffer		|
		|			|
		|			|
		|			|
		|			|
		|			|
		|			|
		|			|
		|			|
		|			|
		 -----------------------
    (b) sceGsSetDefDBuff �֐�
	���� sceGsSetDefDBuff �֐���p���āA���߂� GS �̃�������
	�t���[���o�b�t�@�A�e�N�X�`���o�b�t�@�Ɛݒ肵�Ă��܂��B
	�{�T���v���ł� Z �o�b�t�@�͎g�p���Ă���܂���B

		 -----------------------
		|			|
		|	Frame		|
		|	 Buffer0	|
		|			|
		 -----------------------
		|			|
		|	Frame		|
		|	 Buffer1	|
		|	 		|
		 -----------------------
		|			|
		|	Texture		|
		|	 Buffer		|
		|			|
		|			|
		 -----------------------
		|			|
		|			|
		|			|
		 -----------------------

    == �X���b�h�\�� ==

    �v���O������ 3 �̃X���b�h����ł��Ă��܂��B

    (a) main thread 
    main thread �́Amain() �����s�����X���b�h�ł��Bmain thread ��
    ����������ё��̃X���b�h���쐬/�N��������A���̏������s���܂��B

	+ �f�[�^�����f�B�A���� readBuf �ɓǂݍ��ށB
	+ readBuf �̃f�[�^��񑽏d�����A�G�� videoDec �� viBuf �ցA
	  ������ audioDec ���̃o�b�t�@�֓]������B
	+ DMA �g���ĊG�� IPU �ɑ���B
	+ ������ IOP ���ɑ���B

    (b) video decode thread 
    video decode thread �́AIPU ���g���ĊG�̃f�R�[�h���s���܂��B

	+ IPU ���g���ĊG�̃f�R�[�h������B
	+ �f�R�[�h���ʂ� voBuf �ɒ~����B

    (c) default thread 
    default thread �̓f�o�b�O�ړI�Ŏg�p���܂��B���̃X���b�h�Ɠ��l��
    ���䂪����Ă��܂����ʏ�͉������܂���B


    == ���荞�� ==

    �{�v���O�����̓��[�U���� 2 �̊��荞�݂��g�p���Ă��܂��B

    (a) vblankHandler
	vblank �̊J�n���ɂ��̊֐����Ă΂�܂��B�֐����ł́A

	- sceGsSetHalfOffset ���g�p���� odd �t�B�[���h�� even �t�B�[���h��
	  �Ԃɐ�����n�[�t�s�N�Z���̂���𒲐߁B
	- sceGsSwapDBuff ���g�p���ă_�u���o�b�t�@�̐ؑւ��B
	- sceDmaSend ���g�p���ĉ摜�f�[�^�ƕ`��ɕK�v�ȃf�[�^��GS �ɓ]���B

	�Ƃ����������s�Ȃ��Ă��܂��Bvblank �� 2 �񔭐�����Ԃ� 1 �x����
	�摜�f�[�^�� GS �ɓ]�����邽�� odd �t�B�[���h�� even �t�B�[���h
	�ł͓����摜���e�N�X�`���Ƃ��ĎQ�Ƃ���܂��B�����������
	vblank ���Ƀ_�u���o�b�t�@����ꊷ���Ă��邽�ߓ���� 60 frames/s
	�ɂȂ��Ă��܂��B

    (b) hadler_endimage
	path3 �o�R�� DMA �]�����I���������Ɋ��荞�݂��������A���̊֐���
	�Ă΂�܂��B���C�����[�v�̒��ł͉摜�f�[�^�̓]���ƁAsceGsSwapDBuff
	�̒��� path3 �o�R�� DMA ���g�p����Ă��܂����A�t���O�ɂ����
	��ʂ����Ă��邽�߁AsceGsSwapDBuff �ɂ�� DMA ���I�������ۂɂ́A
	���̊֐��͉������Ȃ��Ŕ����Ă��܂��܂��B�摜�f�[�^�̓]����
	�I�������ۂɂ́A�t���O�������A���̊֐��́A

	- VoBuf ���ɂ���摜�̐��������J�E���^�[�� 1 ���炷�B
	- �t���[���̕`�悪�I���������Ƃ������t���O�𗧂Ă�B

	�Ƃ����������s�Ȃ��܂��B����ɂ���� VoBuf ���ɂ���
	�摜�f�[�^�̐����Ǘ����Ă��܂��B


    == �v���O�����̑�܂��ȗ��� ==

    �{�v���O������ 3 �̃X���b�h�y�� 2 �̊��荞�݃n���h����
    �g�p���Ă��邽�߁A�v���O�����̗��ꂪ���݂ɂ����ł����A
    �ȒP�Ɏ����ƈȉ��̂悤�ɂȂ�܂��B���L�u�f�[�^�̗���y�уo�b�t�@�\���v
    �Ƌ��Ɍ�Q�Ɖ������B

    (a) SIF �̏���������ѕK�v�ȃ��W���[���� IOP ��������Ƀ��[�h�B

    (b) GS �̃������̏�������A�t���[���o�b�t�@�A�e�N�X�`���o�b�t�@��ݒ�B

    (c) �f�B�X�N�������Ă����f�[�^( ���� + �摜 )���i�[����o�b�t�@
	readBuf ���m�ہB
	
    (d) �����y�щ摜�̃f�R�[�_( videoDec, audioDec )���쐬�B

    (e) IPU ���g���ăf�R�[�h������̉摜�f�[�^���i�[����o�b�t�@
	VoBuf ���m�ہB

    (f) 2 �̃X���b�h( default, video decode �ڂ����͏�L���Q�� )���쐬�B

    (g) �X�g���[�~���O���s�Ȃ��t�@�C�����J���B

    (h) 2 �̊��荞�݃n���h����o�^�B
	( vblankHandler, handler_endimage �ڂ����͏�L���Q�� )

    (i)	readBuf �� �f�B�X�N����f�[�^��]���B

    (j) sceMpegDemuxPssRing ��p���ăf�[�^�������Ɖ摜�ɔ񑽏d���B

    (k) �����f�[�^�� IOP �ɑ���Ԃ��B

    (l) sceMpegGetPicture ��p���ĉ摜�f�[�^���f�R�[�h�B

    (m) �f�R�[�h�����摜�f�[�^�ƕ`��f�[�^��GS �ɑ��邽�߂̃p�P�b�g���쐬�B

    (n) �t���O�𗧂Ă� vblankHandler ���������s�Ȃ��悤�ɐݒ�B

    (o) �����f�[�^�̃f�R�[�h�J�n�B

    (p) vblank ���Ƃɕ`��A�\���o�b�t�@��ؑւ��āA�p�P�b�g�� GS �ɓ]���B

    (q) �f�[�^���Ȃ��Ȃ�܂� (i) ���� (p) ���J��Ԃ��B


    == �f�[�^�̗��ꂨ��уo�b�t�@�\�� ==

    �f�[�^�� CD/DVD ����ǂݍ��݂�����Đ�����܂ł̃f�[�^�̗���͉��}��
    �ʂ�ł��B


            audioDec  readBuf   viBuf           voBuf
            +---+     +---+     +---+           +---+
            | A |     | V |---->| V |  Video    |   |
            +---+     +---+     +---+  decode   | V |
            | A |<----| A |     | V |--> IPU -->|   |--> GS
            +---+     +---+     +---+           +---+
              |       | V |     | V |           |   |
              |       +---+     +---+           | V |
              |       | A |     |-------------| |   |
              |       +---+       videoDec      +---+
              |       | V |                    
              |       +---+                                        EE 
              |         ^
            --|---------|-----------------------------------------------
              V         |
            +---+       |                                          IOP
     SPU2<--| A |     CD/DVD/HD
            +---+
            | A |
            +---+

        (a) CD/DVD/HD ����ǂݍ��܂ꂽ�f�[�^�́A�ŏ��� EE ��̃o�b�t�@
            readBuf �ɒu����܂��B
        (b) readBuf �̃f�[�^�́A�񑽏d������A���ƊG�ɕ�������܂��B
        (c) �G�� videoDec ���W���[�����̃r�f�I���̓o�b�t�@ viBuf �ɑ�
	    ���܂��B
        (d) viBuf �̃f�[�^�� IPU ���g���ăf�R�[�h����A�f�R�[�h��̊G
	    �̓r�f�I�o�̓o�b�t�@ voBuf �ɒu����܂��B
        (e) voBuf ��̊G�� vblank ���荞�݂ɓ������� GS �ɑ����A
	    ���j�^�ɕ\������܂��B
        (f) (b) �Ŕ񑽏d�����ꂽ���́AaudioDec ���W���[�����̃o�b�t�@��
            �ۑ�����܂��B
        (g) audioDec �ɕۑ����ꂽ���́AIOP ��(SMEM)�̃o�b�t�@�ɓ]����
	    ��܂��B
        (h) SMEM ��̉��� SPU2 �̃��[�J���������� DMA �]������A������
	    �Đ�����܂��B


    == ���̑� ==

    �{�T���v���v���O�����ł́A�f��/�����̃^�C���X�^���v��
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
    sceMpegGetPicture() �̊Ԃ� SPR ���J������K�v������܂��B

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
	% make help     : help���\��(�t�@�C�����̎w����@���m�F�ł��܂�)


	�z�X�g�̃n�[�h�f�B�X�N�h���C�u�� PSS �t�@�C��������ꍇ�́A�Ⴆ��
	�ȉ��̂悤�Ɏ��s���܂��B

	% dsreset 0 0
	% dsedb
	dsedb S> run main.elf 'host0:XXX.pss�f
'
	�܂��ADTL-T10000 �� CD/DVD �h���C�u�� PSS �t�@�C��������ꍇ�́A
	�Ⴆ�Έȉ��̂悤�Ɏ��s���܂��B

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


