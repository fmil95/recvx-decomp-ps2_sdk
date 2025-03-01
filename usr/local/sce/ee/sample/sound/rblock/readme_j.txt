[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

入出力ブロック転送（旧称AutoDMA転送）のサンプル

<サンプルの解説>

	SPU2の入力ブロックへの転送によって、IOPメモリ上のストレートPCMデー
	タを発音させます。
	libsdrを使用して、EEからlibsd（IOPライブラリ）をリモート実行する例
	にもなっています。

	任意の波形を再生させるには、PCM_FILENAMEにPCMファイルの名前、
	PCM_SIZE にファイルサイズを指定してください。
	この波形の形式は、16bit, Little Endian, Signed のストレートPCMで、
	さらに左チャンネルと右チャンネルが512バイト毎にインターリーブされ
	ている必要があります。インターリーブするツールのサンプルを、このド
	キュメントの末尾に添付しています。

	IOP側で、libsd.irx, sdrdrv.irx が動作している必要があります。
 	ロード状況はdsiconsに表示されるので、うまく動作しない時には
	これらがロードされているかどうか確認してください。

<ファイル>
	main.c	
	knot.int

<起動方法>
	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run main.elf

	音楽がステレオで再生されれば正常。

<コントローラの操作方法>
	なし

<備考>
	なし

<インターリーブツールの例>

        /*
           WAVE interleave tool for SPU2 I/O Block Trans

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
	        	printf("usage: raw2int <src_left> <src_right> 
                                <dst>\n");
	        	return -1;
	        }
		
	        for( i = 0; i < 32; i++ ) {
		        src_name[0][i] = 0;
		        src_name[1][i] = 0;
	        }
	        for (i = 0; i < 2; i++) {
	                sprintf( src_name[i], "%s", argv[i+1] );
	                if( (fsrc[i] = fopen( src_name[i], "rb")) == NULL ){
		            fprintf( stderr, "Can't open output file:%s\n", 
                                     src_name[i] );
	                    return -1;
	                 }
	        }

	        sprintf( dst_name, argv[3] );
        	if( (fdst = fopen( dst_name, "wb" )) == NULL ) {
		        fprintf( stderr, "Can't open output file:%s\n", 
                                 dst_name );
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




