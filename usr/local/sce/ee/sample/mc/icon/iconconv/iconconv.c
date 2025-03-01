/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                     3DIcon file converter.
 *
 *                         Version 1.0.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *      Name : iconconv.c
 *      Description : 
 * 
 *      Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      1.0.0          Dec,3,1999     munekis     1st version
 */


#include "error.h"
#include "iconconv.h"
#include "parse.h"


u_int g_padding;


// --------------------------------------------------------------

int pad_fprintf(FILE *fp, char *fmt, ...) {
    va_list args;
    int i;
    va_start(args, fmt);
    for(i = 0; i < g_padding; i++) fprintf(fp, "    ");
    vfprintf(fp, fmt, args);
    va_end(args);

    return 0;
}


// --------------------------------------------------------------

int usage() {
    printf("usage : iconconv [-b] <input file> <output file>\n");
    return 0;
}


// --------------------------------------------------------------

int main(int argc, char **argv) {
 
    int fd;
    int size, ret, readcount, b_flag;
    u_char *data, c;
    FILE *fp;


    // initialization
    b_flag = 0;

    // --- read option, open files ---
    // too much arguments.
    argc--;
    if(argc > 3) {
	usage();
        return -1;
    }

    argv++;
    if(argc == 3 && (c = argv[0][0]) == '-') {
        if((c = argv[0][1]) == 'b') {
	    b_flag = 1;
        } else {
	    usage();
	    return -1;
        }
        
	// open files
        fd = open(argv[1], O_RDONLY);
        fp = fopen(argv[2], "w");

    } else if(argc == 2) {

	// open files
        fd = open(argv[0], O_RDONLY);
        fp = fopen(argv[1], "w");

    } else {
	usage();
	return -1;
    }

    printf("parsing file: %s\n", argv[1]);

    // --- error check for files ---
    if(fd < 0) {
        ERRORMSG1("failed to open file : %s\n", argv[1]);
        return -1;
    }

    if(fp == NULL) {
        ERRORMSG1("failed to open file : %s\n", argv[2]);
        return -1;
    }
   

    // --- read input file to buffer ---
    size = lseek(fd, 0, SEEK_END);
    ret = lseek(fd, 0, SEEK_SET);

    data = (u_char*) memalign(64, size);
    readcount = read(fd, data, size);
    close(fd);


    // --- export icon file as ascii/binary ---
    if(b_flag == 0) {
	if(DumpIconFile(fp, data, size) < 0) {
	    fprintf(stderr, "Error : failed to reading binary icon file.\n");
	    return -1;
	}
    } else {
	if(DumpBinaryIconFile(fp, data, size) < 0) {
	    fprintf(stderr, "Error : failed to reading ascii icon file.\n");
	    return -1;
	}
    }

    free(data);
    fclose(fp);
        

    return 0;
}


// --------------------------------------------------------------

int DumpIconFile(FILE *fp, u_char *data, u_int size) {

    int n_animseq, n_framelen, n_keyshape, n_key, imm;
    int n_vtx;
    float imm_f, array_f[4];
    u_int i, j, k,attrib, i1;
    u_char *p1, array_c[4];
    float f1;
    short *pShort;

    // --- Header ---    
    pad_fprintf(fp, ";----------- HEADER SECTION ----------\n");
    p1 = data;
    i1 = *(u_int*) p1;

    if(i1 != VERSION_ID) {
        ERRORMSG0("illegal version id");
        return -1;
    }
    pad_fprintf(fp, "0x%8.8x \t;[magic no]\n", i1);    
    p1 += 4;

    // --- nb of shapes ---
    n_keyshape = *(u_int*) p1;
    if(n_keyshape <= 0) {
        ERRORMSG0("nb of keyshape < 0 is not allowed.\n");
        return -1;
    }
    pad_fprintf(fp, "%d \t\t;[nb of keyshapes]\n", n_keyshape);
    p1 += 4;

    // --- mode attribute ---
    attrib = *((u_int*) p1);
    pad_fprintf(fp, "0x%8.8x \t;[model attribute]\n", attrib);
    p1 += 4;

    // --- coeffient for AA1 ---
    f1 = *((float*) p1);
    pad_fprintf(fp, "%f \t;[AA1 COEFF]\n", f1);
    p1 += 4;


    // ==================== MODEL ====================
    n_vtx = *(u_int*) p1;
    pad_fprintf(fp, "\n;----------- MODEL SECTION -----------\n");
    pad_fprintf(fp, "%d \t;[nb of vertices]\n", n_vtx);
    p1 += 4;

    fprintf(fp, "\n");

    for(i = 0; i < n_vtx; i++) {

        pShort = (short*) p1;

        for(j = 0; j < n_keyshape; j++) {
	    for(k = 0; k < 3; k++) {
                array_f[k] = ITOF10(*pShort);
                pShort++;
            }
            pad_fprintf(fp, "%-8f, %-8f, %-8f", 
                array_f[0], array_f[1], array_f[2]);
            if(j == 0) {
                fprintf(fp, " \t;[position %d]\n", i);
            } else {
                fprintf(fp, "\n");
            }

            pShort++; // to skip padding.
            p1 += 8;
        }

        // --- normal ---
        for(k = 0; k < 3; k++) {
            array_f[k] = ITOF12(*pShort);
            pShort++;
        }
        pad_fprintf(fp, "%-8f, %-8f, %-8f \t;[normal %d]\n", 
                array_f[0], array_f[1], array_f[2], i);
        pShort++;
        p1 += 8;

        // --- ST ---
        array_f[0] = ITOF12(*pShort);
        pShort++;
        array_f[1] = ITOF12(*pShort);
        pShort++;
        pad_fprintf(fp, "%-8f, %-8f \t\t;[st %d]\n", array_f[0], array_f[1], i);
        p1 += 4;

        // --- COLOR ---
        array_c[0] = p1[0];
        array_c[1] = p1[1];
        array_c[2] = p1[2];
        array_c[3] = p1[3];
        pad_fprintf(fp, "%3d, %3d, %3d, %3d \t\t;[rgba %d]\n", array_c[0], array_c[1], array_c[2], array_c[3], i);
        p1 += 4;


        fprintf(fp, "\n");
    }
        





    pad_fprintf(fp, "\n;----------- ANIMATION SECTION ----------\n");
    n_animseq = *(u_int*) p1;
    pad_fprintf(fp, "%d \t; [nb of animation section]\n", n_animseq);
    p1 += 4;

    g_padding++;
    for(i = 0; i < n_animseq; i++) {
        
        pad_fprintf(fp, "\n;---- animation section No.%d ----\n", i);

        n_framelen = *(u_int*) p1;
        pad_fprintf(fp, "%d \t; [animation frame length]\n", n_framelen);
        p1 += 4;

        f1 = *(float*) p1;
        pad_fprintf(fp, "%f \t; [animation speed]\n", f1);
        p1 += 4;
        
        i1 = *(u_int*) p1;
        pad_fprintf(fp, "%d \t; [animation play offset]\n", i1);
        p1 += 4;        

        n_keyshape = *(u_int*) p1;
        pad_fprintf(fp, "%d \t; [nb of keyshape used for this anim-sequence]\n", n_keyshape);
        p1 += 4;

        g_padding++;
        for(j = 0; j < n_keyshape; j++) {

            imm = *(u_int*) p1;
            pad_fprintf(fp, "%d \t; [shape id]\n", imm);
            p1 += 4;

            n_key = *(u_int*) p1;
            pad_fprintf(fp, "%d \t; [nb of keys]\n", n_key);
            p1 += 4;

            g_padding++;
            for(k = 0; k < n_key; k++) {

                imm_f = *(float*) p1;
                pad_fprintf(fp, "%f \t; [time]\n", imm_f);
                p1 += 4;

                imm_f = *(float*) p1;
                pad_fprintf(fp, "%f \t; [value]\n", imm_f);
                p1 += 4;

                fprintf(fp, "\n");
            }
            g_padding--;
        }
        g_padding--;
    }
    g_padding--;




    return 0;
}




// --------------------------------------------------------------


#define ATOI(x) ((u_int) strtol(x, (char**)NULL, 0))
#define ATOF(x) ((float) strtod(x, (char**)NULL))
#define ATOS(x) ((short) strtol(x, (char**)NULL, 0))

int DumpBinaryIconFile(FILE *fp, u_char *data, u_int size) {

    parse_data_t in_data;
    u_char token[1000];
    u_int imm, n_shapes, n_vtx, i, j, k, n_animseq, n_keys, attrib;
    float imf;
    short ims;
    u_char imc;

    in_data.data = data;
    in_data.index = 0;
    in_data.size = size;


    // --- get magic number ---
    GetToken(&in_data, token);
    imm = ATOI(token);

    if(imm != VERSION_ID) {
        ERRORMSG1("illegal version id : %d\n", imm);
        return -1;
    }
    My_fwrite(&imm, sizeof(u_int), 1, fp);


    // --- get n_keyshape ---
    GetToken(&in_data, token);
    n_shapes = ATOI(token);
    My_fwrite(&n_shapes, sizeof(u_int), 1, fp);

    
    // --- get model attribute ---
    GetToken(&in_data, token);
    attrib = ATOI(token);
    My_fwrite(&attrib, sizeof(u_int), 1, fp);


    // --- get aa1 coeff ---
    GetToken(&in_data, token);
    imf = ATOF(token);    
    My_fwrite(&imf, sizeof(float), 1, fp);


    // --- get nb of vertices ---
    GetToken(&in_data, token);
    n_vtx = ATOI(token);
    My_fwrite(&n_vtx, sizeof(u_int), 1, fp);

    for(i = 0; i < n_vtx; i++) {
        
        // --- positions ---
        for(j = 0; j < n_shapes; j++) {
            for(k = 0; k < 3; k++) {
	        GetToken(&in_data, token);
                imf = ATOF(token);
                ims = FTOI10(imf);
                My_fwrite(&ims, sizeof(short), 1, fp);
	    }
            imf = 1.0f;
            ims = FTOI10(imf);
            My_fwrite(&ims, sizeof(short), 1, fp);
        }
        

        // --- normal ---
        for(k = 0; k < 3; k++) {
            GetToken(&in_data, token);
            imf = ATOF(token);
            ims = FTOI12(imf);
            My_fwrite(&ims, sizeof(short), 1, fp);
        }
        imf = 1.0f;
        ims = FTOI10(imf);
        My_fwrite(&ims, sizeof(short), 1, fp);

        // --- st ---
        for(k = 0; k < 2; k++) {
            GetToken(&in_data, token);
            imf = ATOF(token);
            ims = FTOI12(imf);
            My_fwrite(&ims, sizeof(short), 1, fp);
        }
        
        // --- rgba ---
        for(k = 0; k < 4; k++) {
            GetToken(&in_data, token);
            imf = ATOF(token);
            imc = (u_char) imf;
            My_fwrite(&imc, sizeof(u_char), 1, fp);
        }

    }


    // ---- ANIMATION SECTION ----
    GetToken(&in_data, token);
    n_animseq = ATOI(token);
    My_fwrite(&n_animseq, sizeof(u_int), 1, fp);
    
    for(i = 0; i < n_animseq; i++) {

        // frame length
        GetToken(&in_data, token);
        imm = ATOI(token);
        My_fwrite(&imm, sizeof(u_int), 1, fp);
   
        // animation speed
        GetToken(&in_data, token);
        imf = ATOF(token);
        My_fwrite(&imf, sizeof(float), 1, fp);
   
        // animation play offset
        GetToken(&in_data, token);
        imm = ATOI(token);
        My_fwrite(&imm, sizeof(u_int), 1, fp);

        // nb of keyshapes used in this animation sequence.
        GetToken(&in_data, token);
        n_shapes = ATOI(token);
        My_fwrite(&n_shapes, sizeof(u_int), 1, fp);

        for(j = 0; j < n_shapes; j++) {
        
            // shape id
            GetToken(&in_data, token);
            imm = ATOI(token);
            My_fwrite(&imm, sizeof(u_int), 1, fp);

            // nb of keys
            GetToken(&in_data, token);
            n_keys = ATOI(token);
            My_fwrite(&n_keys, sizeof(u_int), 1, fp);

            for(k = 0; k < n_keys; k++) {
                
                // time
                GetToken(&in_data, token);
                imf = ATOF(token);
                My_fwrite(&imf, sizeof(float), 1, fp);

                // value
                GetToken(&in_data, token);
                imf = ATOF(token);
                My_fwrite(&imf, sizeof(float), 1, fp);

            }

        }
                


    }





    return 0;

}





// --------------------------------------------------------------

int My_fwrite(void *data, int size, int num, FILE *fp) {


    fwrite(data, size, num, fp);

    return size;
}










