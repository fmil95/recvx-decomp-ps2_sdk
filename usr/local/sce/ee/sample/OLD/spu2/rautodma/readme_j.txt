[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

AutoDMA�]���̃T���v��

<�T���v���̉��>
	�b��T�E���h���C�u���� librspu2 ��AutoDMA�]���@�\���g���āA�X�g��
	�[�gPCM���͂ɂ�锭�����s�Ȃ��܂��B
	�C�ӂ̔g�`���Đ�������ɂ́APCM_FILENAME��PCM�t�@�C���̖��O�A
	PCM_SIZE �Ƀt�@�C���T�C�Y���w�肵�Ă��������B
	���̔g�`�̌`���́A16bit, Little Endian, Signed �̃X�g���[�gPCM�ŁA
	����ɍ��`�����l���ƉE�`�����l����512�o�C�g���ɃC���^�[���[�u����
	�Ă���K�v������܂��B�C���^�[���[�u����c�[���̃T���v�����A���̃h
	�L�������g�̖����ɓY�t���Ă��܂��B
	�f�[�^���C���^�[���[�u����K�v�̂Ȃ�AutoDMA�]��API���A�����I�ɒ�
	����\��ł��B

<�t�@�C��>
	main.c	
	knot.int

<�N�����@>
	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run main.elf

	���y���X�e���I�ōĐ������ΐ���B

<�R���g���[���̑�����@>
	�Ȃ�

<���l>
	�Ȃ�

<�C���^�[���[�u�c�[���̗�>

/*
   WAVE interleave tool for SPU2 AutoDMA

   usage: raw2int <src_left> <src_right> <dst>
 */

#include <stdio.h>
#include <sys/types.h>
#include <string.h>

FILE *fsrc[2];
FILE *fdst;
FILE *ferr;

#define FILE_NAME_LENGTH 1024
char src_name[2][FILE_NAME_LENGTH];
char dst_name[FILE_NAME_LENGTH];
char buff[512];

void convert(void)
{
	int count;

	while( 1 )
	{
		count = fread( buff, 1, 512, fsrc[0] );
		if (count < 512)
		    break;
		fwrite( buff, 1, 512, fdst );
		count = fread( buff, 1, 512, fsrc[1] );
		fwrite( buff, 1, count, fdst );
		if(count < 512) break;
	}
	return;
}

int main ( int argc,  char *argv[] )
{
	short i;

	if( argc < 4){
		printf("usage: raw2int <src_left> <src_right> <dst>\n");
		return -1;
	}
		
	for( i = 0; i < 32; i++ ) {
		src_name[0][i] = 0;
		src_name[1][i] = 0;
	}
	for (i = 0; i < 2; i++) {
	    sprintf( src_name[i], "%s", argv[i+1] );
	    if( (fsrc[i] = fopen( src_name[i], "rb")) == NULL ){
		fprintf( stderr, "Can't open output file:%s\n", src_name[i] );
		return -1;
	    }
	}

	sprintf( dst_name, argv[3] );
	if( (fdst = fopen( dst_name, "wb" )) == NULL ) {
		fprintf( stderr, "Can't open output file:%s\n", dst_name );
		return -1;
	}

	printf( "Processing: %s + %s -> %s ..." 
		, src_name[0], src_name[1], dst_name );

	convert();

	if( fsrc[0] != 0 ) fclose( fsrc[0] );
	if( fsrc[1] != 0 ) fclose( fsrc[1] );
	if( fdst != 0 ) fclose( fdst );
	printf( "done\n" );

	return 0;
}


