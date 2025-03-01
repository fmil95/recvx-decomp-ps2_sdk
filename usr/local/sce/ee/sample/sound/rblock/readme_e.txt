[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The Sample Program for I/O Block Transfer (former AutoDMA Transfer)

<Description of the Sample>

	This sample program allows to sound out the straight PCM data
	in the IOP memory by transfering it to the SPU2 input block.
	The program also samples the remote execution of libsd 
	
	Specify the PCM file name in PCM_FILENAME and the file size
	in PCM_SIZE in order to play  arbitrary waveforms.
	The format of the waveform must be the straight PCM of 16-bit, 
	little endian and signed.  Also, both left and right channels 
        must be interleaved every 512-byte. 
        The sample program of the tool for interleaving is attached to
        the end of the document.

	libsd.irx and sdrdrv.irx must be running on the IOP. 
	The loading status is displayed on dsicons.  
	Check the dsicons whether to these are loaded if not functioning
        well.
	
<File>
	main.c	
	knot.int

<Activating the Program>
	% make		:Compiles the program
	% make run	:Executes the program

	After compilation, also executable with the following:
	% dsedb
	> run main.elf

	The operation is normal if the music is played in stereo.

<Using the Controller>
	None

<Note>
	None

<Example of Interleave Tool>

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




