/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - bspline -
 *
 *                         Version 0.3.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *          Name : bs.c
 *          Description : packet setting function(to send control points to VU1).
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.3.0          Jun,1,1999     munekis     first version
 */


#include <eekernel.h>
#include <eeregs.h>
#include <math.h>
#include <stdlib.h>
#include <libvu0.h>

extern unsigned int My_bspline_start __attribute__((section(".vudata")));
extern unsigned int My_bspline_end __attribute__((section(".vudata")));
extern unsigned int My_reftag __attribute__((section(".vudata")));


extern u_int *cppkt[2];
extern float *cp_mod;


void CreateControlPointArea(u_int nURow, u_int nVRow, u_int bUClosed, u_int bVClosed, u_int shading) {
	
	u_int length, n_chunk, i;
	u_int *reftag, *data, ui_tmp;

	reftag = (u_int*) &My_reftag;


	length = &My_bspline_end - &My_bspline_start;
	data = (u_int*) &My_bspline_start;

	if(shading == 1) {
		data[3] = 0x300e4000; // shading
	} else {	
		data[3] = 0x30414000; // wire with AA1
	}


	if(bUClosed) 
		n_chunk = (nURow - 1) / 5 + 1;	
	else 
		n_chunk = (nURow - 4) / 5 + 1;

	// --- copy control points chunk ---
	cppkt[0] = (u_int*) 0x70000000;
	for(i = 0; i < n_chunk; i++)	
		memcpy(cppkt[0] + i * length, data, length * sizeof(u_int));


	cppkt[1] = (u_int*) (0x70000000 + n_chunk * length * sizeof(u_int));
	for(i = 0; i < n_chunk; i++)	
		memcpy(cppkt[1] + i * length, data, length * sizeof(u_int));

	// --- set ref addr ---
	ui_tmp = reftag[0] & 0xffff0000;
	length = length >> 2;
	reftag[0] = ui_tmp | (length * n_chunk);
	
	return;

}


void SetControlPoint(u_int v_index_start, 
		     u_int nURow, u_int nVRow, 
                     u_int u_closed, u_int v_closed, u_int *addr) {

	u_int i, j, k, uIndex, index, n_chunk, u_index, v_index;
	u_int n_last, n_gif, *last_gif, ui_tmp;
	float *current;
	float s_width, t_width;
		

	current = (float*) (addr + 8);


	if(u_closed) 
		s_width = 1.0f / nURow;
	else
		s_width = 1.0f / (nURow - 3);
	
	if(v_closed) 
		t_width = 1.0f / nVRow;
	else
		t_width = 1.0f / (nVRow - 3);
	

	
	if(u_closed)
		n_chunk = (nURow - 1) / 5;
	else
		n_chunk = (nURow - 4) / 5;

	uIndex = 0;
	for(k = 0; k < n_chunk; k++) {

		current[0] = uIndex * s_width;
		current[1] = v_index_start * t_width;
		current[2] = s_width;
		current[3] = t_width;
		current += 4;

		for(i = 0; i < 8; i++) {
			for(j = 0; j < 4; j++) {

				u_index = uIndex + i; 
				v_index = v_index_start + j;
				if(u_closed && (u_index >= nURow)) 
					u_index -= nURow; 
				if(v_closed && (v_index >= nVRow)) 
					v_index -= nVRow; 
				

				index = (v_index * nURow + u_index) * 4;	
				current[0] = cp_mod[index    ];
				current[1] = cp_mod[index + 1];
				current[2] = cp_mod[index + 2];

				current += 4;
			}
		}


		uIndex += 5;
		current += 12;

	}


	// ---------- last chunck ------------
	if(u_closed) {	
		n_last = (nURow - 1) % 5 + 1;
	} else {
		n_last = (nURow - 4) % 5 + 1;
	}

	n_gif = n_last * 24;


	last_gif = (u_int*) current;
	last_gif -= 6;
	ui_tmp = *last_gif & 0xffff8000;


	*last_gif = ui_tmp | n_gif;

	if(n_last > 0) {

		current[0] = uIndex * s_width;
		current[1] = v_index_start * t_width;
		current[2] = s_width;
		current[3] = t_width;
		current += 4;

		for(i = 0; i < n_last + 3; i++) {
			for(j = 0; j < 4; j++) {

				u_index = uIndex + i; 
				v_index = v_index_start + j;

				if(u_closed && (u_index >= nURow)) 
					u_index -= nURow; 
				if(v_closed && (v_index >= nVRow)) 
					v_index -= nVRow; 


				index = (v_index * nURow + u_index) * 4;	
				current[0] = cp_mod[index    ];
				current[1] = cp_mod[index + 1];
				current[2] = cp_mod[index + 2];

				current += 4;
			}
		}
	}
	

	return;
}

