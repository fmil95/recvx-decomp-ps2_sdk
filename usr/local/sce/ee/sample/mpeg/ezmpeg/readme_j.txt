[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

IPU ���g�p���� MPEG2 �X�g���[���Đ��T���v��


<�T���v���̉��>
	IPU ���g�p���� MPEG2(MPEG1���܂�)�X�g���[���̍Đ��T���v��
	�v���O�����ł��B�Đ��ł��� MPEG2 �X�g���[����

	    Main Profile at Main Level(MP@ML)
	    Simple Profile at Main Level(SP@ML)

	�ł���A���t���[���\���̃s�N�`�������X�g���[���݂̂ł��B
	�t�B�[���h�\���̃s�N�`����������̍Đ��ɂ͌��ݑΉ����Ă���
	�܂���B�܂��AMPEG2 �̃V�X�e�����̕t�����X�g���[���ɂ��Ή�
	���Ă���܂���B

	�{�T���v���v���O�����ł́AMPEG2 �f�[�^����U���ׂă��������
	�ǂݍ��݂܂��B�ǂݍ��񂾃f�[�^���֐� sceMpegAddBs() ��p����
	IPU �֑���A�֐� sceMpegGetPicture() ��p���� 1 �s�N�`���Â�
	�f�R�[�h���܂��B�f�R�[�h���ʂ́A���C����������ɓW�J����܂�
	�̂ŁA����� GS �� DMA �]�����ĉ�ʂɕ\�����܂��B

	�t���[���\�����Ƃ�s�N�`���́A��̃t���[���̒��ɋ������
	��������ъ�t�B�[���h�������܂��B�e�t�B�[���h���؂�ւ�
	��̂� 1/60 sec �ł��̂ŁA�s�N�`�����̂� 1/30 sec �Ő؂�ւ��
	�K�v������܂��B�����ŁA�v���O�������ł� 2 vblank �ň��
	�s�N�`�����؂�ւ��悤�Ɏw�肵�Ă��܂��B

	�֐� sceMpegGetPicture() �́A������ SPR(Scratch Pad RAM) ��
	�g�p���܂��B���̂��߁A���� SPR ���g���ꍇ�ɂ�
	sceMpegGetPicture() �̊Ԃ����� SPR ���J������K�v������܂��B

<�t�@�C��>
        ezmeg.h
        display.c
        ldimage.c
        main.c
	ez.m2v

<�N�����@>
	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run ezmpeg.elf ez.m2v

<�R���g���[���̑�����@>
	�Ȃ�

<���l>
	�Ȃ�
