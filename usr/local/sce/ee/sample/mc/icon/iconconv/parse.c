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
 *      Name : parse.c
 *      Description : 
 * 
 *      Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      1.0.0          Dec,3,1999     munekis     1st version
 */

#include <ctype.h>

#include "iconconv.h"
#include "parse.h"
#include "error.h"

int SkipLine(parse_data_t *);

// ------------------------------------------------------
#define SPC(x)  ((isspace(x) || x == ',') ? 1 : 0)

int GetToken(parse_data_t *in, char *token) {

    int c;  
    int index;


    // check if data is already parsed.
    if(in->index >= in->size) return -1;

    // skip space
    while(SPC(in->data[in->index])) {
        if(in->index == in->size) {
            return -1; // end of data.
        }
        in->index++;
    }

    index = 0;
    while(in->index < in->size) {

        c = in->data[in->index];
	if(c == ';' || c == '\n') {
            
            // get char without any chars.
            if(index == 0) {
                SkipLine(in);
            } else {
                in->index++;
                token[index] = '\0';
                return index;
            }

	} else if(SPC(c)) {
            
            if(index > 0) {
                token[index] = '\0';
                return index;
            } else {
                                                in->index++;
            }    

        } else {
            token[index++] = c;
            in->index++;
        }
    }
    
    return -1;
}


// ------------------------------------------------------
int SkipLine(parse_data_t *in) {


    while(in->index < in->size) {
        if(in->data[in->index] == '\n') {
            in->index++;
            return 0;
        }
        in->index++;
    }
    
    return -1;
}













