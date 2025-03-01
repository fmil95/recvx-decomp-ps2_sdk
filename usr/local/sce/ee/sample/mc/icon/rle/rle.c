/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                 Run-Length encode/decode(2bytes based)
 *
 *                         Version 1.0.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *      Name : rle.c
 *      Description : main procedures
 * 
 *      Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      1.0.0          Dec,3,1999     munekis     1st version
 */



#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


#include "rle.h"


// --- prototypes ---
int My_fwrite(void *, int, int, FILE *);

// -----------------------------------------------------------

int usage() {
    printf("usage : rle [-x] <input> <output>\n");
    return 0;
}

// -----------------------------------------------------------

int main(int argc, char **argv)
{
 

    u_int fd_in, size, ret, out_size;
    u_char *in_data, *out_data, c;

    FILE *fpout;

    argc--;
    if(argc > 3) {
        usage();
        return -1;
    }

    argv++;
    if(argc == 3 && (c = argv[0][0]) == '-') {

        if((c = argv[0][1]) != 'x') {
            usage();
            return -1;   
        }

        // --- Decode  ---
        fd_in = open(argv[1], O_RDONLY);
        size = lseek(fd_in, 0, SEEK_END);
        ret = lseek(fd_in, 0, SEEK_SET);
        in_data = (u_char*) memalign(64, size * sizeof(u_char));
        out_data = (u_char*) memalign(64, 2 * size * sizeof(u_char));
        read(fd_in, in_data, size * sizeof(u_char));

        size = *(u_int*) in_data;
        RleDecode(in_data + 4, size, out_data, &out_size);

        fpout = fopen(argv[2], "w");
        My_fwrite(out_data, sizeof(u_char), out_size, fpout);
        close(fd_in);
        fclose(fpout);

        free(in_data);
        free(out_data);


    } else if(argc == 2) {


        // --- ENCODE ---
        fd_in = open(argv[0], O_RDONLY);
        size = lseek(fd_in, 0, SEEK_END);
        ret = lseek(fd_in, 0, SEEK_SET);

        in_data = (u_char*) memalign(64, size * sizeof(u_char));
        out_data = (u_char*) memalign(64, 2 * size * sizeof(u_char));
        read(fd_in, in_data, size * sizeof(u_char));
        close(fd_in);
        RleEncode(in_data, size, out_data, &out_size);

        fpout = fopen(argv[1], "w");
        My_fwrite(&out_size, sizeof(u_int), 1, fpout);
        My_fwrite(out_data, sizeof(u_char), out_size, fpout);
        fclose(fpout);   

        free(in_data);
        free(out_data);

    } else {
        usage();
        return -1;
    }
    

    return 0;

}

// ----------------------------------------------------------------

int RleEncode(u_char *in_data, u_int size, 
              u_char *out_data, u_int *out_size) {

    u_short s1, prev_s1 = 0, *pInput, *pOutput, cnt = 0;
    u_int index, index_buff, flag;
    u_short buffer[0xffff];

    // --- set start ---
    pInput = (u_short*) in_data;
    pOutput = (u_short*) out_data;
    index_buff = 0;
    flag = 3;
    index = 0;
    size = size >> 1;

    while(index < size) {

        // --- read char ---        
        s1 = *pInput++;
        index++;
            
        if(flag == 0) { // same chars

	    if(prev_s1 == s1) {
                cnt++;
                if(cnt == 0x7fff) {
                    *pOutput++ = 0x7fff;
                    *pOutput++ = s1;
                    cnt = 0;
                    flag = 3;
                    prev_s1 = s1;
                    index_buff = 0;
                    continue;
                } else {
                    continue;
                }
	    } else {
                *pOutput++ = cnt;
                *pOutput++ = prev_s1;
                cnt = 1;
                flag = 2;
                prev_s1 = s1;
                continue;
            }
        } else if(flag == 1) { // different chars

            if(prev_s1 != s1) {
                cnt++;            
                if(cnt == 0x7fff) {
                    *pOutput++ = - (cnt-1);
                    memcpy(pOutput, buffer, sizeof(short)*(cnt-1));
                    cnt = 1;
                    flag = 2;
                    prev_s1 = s1;
                    index_buff = 0;
                    pOutput += (cnt-1);
                    continue;
                } else {
                    buffer[index_buff++] = s1;
                    prev_s1 = s1;
                    continue;
                }
	    } else if(prev_s1 == s1) {
                *pOutput++ = -(cnt-1);
                memcpy(pOutput, buffer, sizeof(u_short)*(cnt-1));         
                pOutput += (cnt-1);
                index_buff = 0;
                cnt = 2;
                flag = 0;
                continue;
            }

        } else if(flag == 2) { 

            if(prev_s1 == s1) {
                cnt++;
                flag = 0;
                continue;
	    } else {
                cnt++;
                flag = 1;
                buffer[0] = prev_s1;
                buffer[1] = s1;
                index_buff = 2;
                prev_s1 = s1;
            }


        } else {  // init state.
            prev_s1 = s1;
            flag = 2;
            cnt = 1;
            continue;            
        }
    }
        

    if(flag == 0) {
        *pOutput++ = cnt;
        *pOutput++ = prev_s1;
    } else if(flag == 1) {
        *pOutput++ = -cnt;
        memcpy(pOutput, buffer, cnt * sizeof(u_short));
        pOutput += cnt;
    } else if(flag == 2) {
        *pOutput++ = cnt;
        *pOutput++ = prev_s1;
    } 

    
    *out_size = (u_int) ((u_char*)pOutput - out_data);

    return 0;
}

// ----------------------------------------------------------------

int RleDecode(u_char *in_data, u_int size, 
              u_char *out_data, u_int *out_size) {

    short *pInput, *pOutput;
    int cnt;
    u_int index, i;

    // --- set start ---
    pInput = (short*) in_data;
    pOutput = (short*) out_data;
    index = 0;
    size = size >> 1;

    while(index < size) {

        // --- read char ---        
        cnt = (int) *pInput++;
        index++;
        
        if(cnt > 0) { // same char.
            for(i = 0 ; i < cnt; i++) 
                *pOutput++ = *pInput;
            pInput++;
            index++;
        } else if(cnt < 0) {
            cnt = -cnt;
            memcpy(pOutput, pInput, cnt*sizeof(short));
            pOutput += cnt;
            pInput += cnt;
            index += cnt;
        }
    }
    
    *out_size = (u_int) ((u_char*)pOutput - out_data);


    return 0;
}



// ----------------------------------------------------------------

int My_fwrite(void *data, int size, int num, FILE *fp) {


    fwrite(data, size, num, fp);
    return size;

}












