/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                     - mug with AA1 line -
 *
 *                        Version 0.4.0
 *
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : add_object.c
 *                     Description : add object
 *
 *       Version   Date           Design              Log
 *  --------------------------------------------------------------------
 *       0.1.0     May, 18th ,2000 sasaki           first version
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sifdev.h>
#include <libvifpk.h>

// --- added by sasaki ---
extern sceVif1Packet	MyVif1Packet;
extern sceVif1Packet	MyVif1Packet1;
extern u_long128	PacketData1[0x10000] __attribute__((aligned (64))); // 1.6MB
extern sceVif1Packet	MyVif1Packet2;
extern u_long128	PacketData2[0x10000] __attribute__((aligned (64))); // 1.6MB
extern sceVif1Packet	MyVif1Packet3;
extern u_long128	PacketData3[0x10000] __attribute__((aligned (64))); // 1.6MB
extern sceVif1Packet	MyVif1Packet4;
extern u_long128	PacketData4[0x10000] __attribute__((aligned (64))); // 1.6MB
extern sceVif1Packet	MyVif1Packet5;
extern u_long128	PacketData5[0x10000] __attribute__((aligned (64))); // 1.6MB
extern sceVif1Packet	MyVif1Packet6;
extern u_long128	PacketData6[0x10000] __attribute__((aligned (64))); // 1.6MB
extern u_int		pData[4];

extern int TME_FLAG;

extern unsigned int GetCOUNT(void);

int detail_level = 0;

void initialize_packet_buffer1()
{
	// --- setup object to draw ---
	sceVif1PkInit(&MyVif1Packet, PacketData1);
	sceVif1PkReset(&MyVif1Packet);

	sceVif1PkCnt(&MyVif1Packet, 0);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	MyVif1Packet1 = MyVif1Packet;
}

void close_packet_buffer1()
{
	int n_packet;
	n_packet = sceVif1PkSize(&MyVif1Packet);

//	printf("n_packet: %08x\n",(u_int)n_packet);

	sceVif1PkTerminate(&MyVif1Packet);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	sceVif1PkEnd(&MyVif1Packet, 0);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	MyVif1Packet1 = MyVif1Packet;
}

void initialize_packet_buffer2()
{
	// --- setup object to draw ---
	sceVif1PkInit(&MyVif1Packet, PacketData2);
	sceVif1PkReset(&MyVif1Packet);

	sceVif1PkCnt(&MyVif1Packet, 0);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	MyVif1Packet2 = MyVif1Packet;
}

void close_packet_buffer2()
{
	int n_packet;
	n_packet = sceVif1PkSize(&MyVif1Packet);

	printf("n_packet: %08x\n",(u_int)n_packet);

	sceVif1PkTerminate(&MyVif1Packet);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	sceVif1PkEnd(&MyVif1Packet, 0);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	MyVif1Packet2 = MyVif1Packet;
}

void initialize_packet_buffer3()
{
	// --- setup object to draw ---
	sceVif1PkInit(&MyVif1Packet, PacketData3);
	sceVif1PkReset(&MyVif1Packet);

	sceVif1PkCnt(&MyVif1Packet, 0);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	MyVif1Packet3 = MyVif1Packet;
}

void close_packet_buffer3()
{
	int n_packet;
	n_packet = sceVif1PkSize(&MyVif1Packet);

	printf("n_packet: %08x\n",(u_int)n_packet);

	sceVif1PkTerminate(&MyVif1Packet);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	sceVif1PkEnd(&MyVif1Packet, 0);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	MyVif1Packet3 = MyVif1Packet;
}

void initialize_packet_buffer4()
{
	// --- setup object to draw ---
	sceVif1PkInit(&MyVif1Packet, PacketData4);
	sceVif1PkReset(&MyVif1Packet);

	sceVif1PkCnt(&MyVif1Packet, 0);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	MyVif1Packet4 = MyVif1Packet;
}

void close_packet_buffer4()
{
	int n_packet;
	n_packet = sceVif1PkSize(&MyVif1Packet);

	printf("n_packet: %08x\n",(u_int)n_packet);

	sceVif1PkTerminate(&MyVif1Packet);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	sceVif1PkEnd(&MyVif1Packet, 0);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	MyVif1Packet4 = MyVif1Packet;
}

void initialize_packet_buffer5()
{
	// --- setup object to draw ---
	sceVif1PkInit(&MyVif1Packet, PacketData5);
	sceVif1PkReset(&MyVif1Packet);

	sceVif1PkCnt(&MyVif1Packet, 0);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	MyVif1Packet5 = MyVif1Packet;
}

void close_packet_buffer5()
{
	int n_packet;
	n_packet = sceVif1PkSize(&MyVif1Packet);

	printf("n_packet: %08x\n",(u_int)n_packet);

	sceVif1PkTerminate(&MyVif1Packet);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	sceVif1PkEnd(&MyVif1Packet, 0);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	MyVif1Packet5 = MyVif1Packet;
}

void initialize_packet_buffer6()
{
	// --- setup object to draw ---
	sceVif1PkInit(&MyVif1Packet, PacketData6);
	sceVif1PkReset(&MyVif1Packet);

	sceVif1PkCnt(&MyVif1Packet, 0);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	MyVif1Packet6 = MyVif1Packet;
}

void close_packet_buffer6()
{
	int n_packet;
	n_packet = sceVif1PkSize(&MyVif1Packet);

	printf("n_packet: %08x\n",(u_int)n_packet);

	sceVif1PkTerminate(&MyVif1Packet);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	sceVif1PkEnd(&MyVif1Packet, 0);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	MyVif1Packet6 = MyVif1Packet;
}

int add_mesh_to_packet_buffer(int ndata, float mpos[], float mnorm[])
{
	int i,j,npos;
	int n_poly;
	int n_packet;
	float vx,vy,vz,va;

	if (ndata > 6) {
		printf("too many polygons in a mesh.\n");
		return -1;
	}

	n_packet = sceVif1PkSize(&MyVif1Packet);

	if (n_packet + 310 > 0x10000) {
		printf("packet buffer overflowed. %d\n", n_packet);
		return -1;
	}

	n_poly = (ndata-1)*(ndata-1);

	// --- upper triangles ---
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8000, 0x6c, 4, 4); // V4_32

	if (TME_FLAG == 0) {
	    pData[3] = 0x00000004; pData[2] = 0x12412412; pData[1] = 0x9005c000; pData[0] = 0x00008000 + n_poly;//shaded triangle
	} else {
	    pData[3] = 0x00000004; pData[2] = 0x12412412; pData[1] = 0x900dc000; pData[0] = 0x00008000 + n_poly;//shaded triangle
//	    pData[3] = 0x00000004; pData[2] = 0x12412412; pData[1] = 0x902dc000; pData[0] = 0x00008000 + n_poly;//ABE+TME
	}
	sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);
	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// position
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8001, 0x6c, 4, 4); // V4_32

	for (i=0; i<ndata-1; i++) {
	for (j=0; j<ndata-1; j++) {

		npos = (i*ndata+j)*4;
		vx = mpos[npos]; vy = mpos[npos+1]; vz = mpos[npos+2]; va = mpos[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

		npos = ((i+1)*ndata+j)*4;
		vx = mpos[npos]; vy = mpos[npos+1]; vz = mpos[npos+2]; va = mpos[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

		npos = (i*ndata+j+1)*4;
		vx = mpos[npos]; vy = mpos[npos+1]; vz = mpos[npos+2]; va = mpos[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

	}
	}

	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// normal
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8001+n_poly*3, 0x6c, 4, 4); // V4_32

	for (i=0; i<ndata-1; i++) {
	for (j=0; j<ndata-1; j++) {

		npos = (i*ndata+j)*4;
		vx = mnorm[npos]; vy = mnorm[npos+1]; vz = mnorm[npos+2]; va = mnorm[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

		npos = ((i+1)*ndata+j)*4;
		vx = mnorm[npos]; vy = mnorm[npos+1]; vz = mnorm[npos+2]; va = mnorm[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

		npos = (i*ndata+j+1)*4;
		vx = mnorm[npos]; vy = mnorm[npos+1]; vz = mnorm[npos+2]; va = mnorm[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

	}
	}

	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// st
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8001+n_poly*6, 0x6c, 4, 4); // V4_32

	for (i=0; i<ndata-1; i++) {
	for (j=0; j<ndata-1; j++) {

		npos = (i*ndata+j)*4;
		vx = (float)i/(float)(ndata-1); vy = (float)j/(float)(ndata-1); vz = 1.0f; va = 1.0f;
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

		npos = ((i+1)*ndata+j)*4;
		vx = (float)(i+1)/(float)(ndata-1); vy = (float)j/(float)(ndata-1); vz = 1.0f; va = 1.0f;
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

		npos = (i*ndata+j+1)*4;
		vx = (float)i/(float)(ndata-1); vy = (float)(j+1)/(float)(ndata-1); vz = 1.0f; va = 1.0f;
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

	}
	}

	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// MSCNT
	pData[0] = SCE_VIF1_SET_MSCNT(0);
	sceVif1PkAddCode(&MyVif1Packet, pData[0]);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	// --- lower triangles ---
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8000, 0x6c, 4, 4); // V4_32

	if (TME_FLAG == 0) {
	    pData[3] = 0x00000004; pData[2] = 0x12412412; pData[1] = 0x9005c000; pData[0] = 0x00008000 + n_poly;//shaded triangle
	} else {
	    pData[3] = 0x00000004; pData[2] = 0x12412412; pData[1] = 0x900dc000; pData[0] = 0x00008000 + n_poly;//shaded triangle
//	    pData[3] = 0x00000004; pData[2] = 0x12412412; pData[1] = 0x902dc000; pData[0] = 0x00008000 + n_poly;//ABE+TME
	}
	sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);
	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// position
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8001, 0x6c, 4, 4); // V4_32

	for (i=0; i<ndata-1; i++) {
	for (j=0; j<ndata-1; j++) {

		npos = ((i+1)*ndata+j)*4;
		vx = mpos[npos]; vy = mpos[npos+1]; vz = mpos[npos+2]; va = mpos[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

		npos = ((i+1)*ndata+j+1)*4;
		vx = mpos[npos]; vy = mpos[npos+1]; vz = mpos[npos+2]; va = mpos[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

		npos = (i*ndata+j+1)*4;
		vx = mpos[npos]; vy = mpos[npos+1]; vz = mpos[npos+2]; va = mpos[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

	}
	}

	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// normal
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8001+n_poly*3, 0x6c, 4, 4); // V4_32

	for (i=0; i<ndata-1; i++) {
	for (j=0; j<ndata-1; j++) {

		npos = ((i+1)*ndata+j)*4;
		vx = mnorm[npos]; vy = mnorm[npos+1]; vz = mnorm[npos+2]; va = mnorm[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

		npos = ((i+1)*ndata+j+1)*4;
		vx = mnorm[npos]; vy = mnorm[npos+1]; vz = mnorm[npos+2]; va = mnorm[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

		npos = (i*ndata+j+1)*4;
		vx = mnorm[npos]; vy = mnorm[npos+1]; vz = mnorm[npos+2]; va = mnorm[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

	}
	}

	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// st
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8001+n_poly*6, 0x6c, 4, 4); // V4_32

	for (i=0; i<ndata-1; i++) {
	for (j=0; j<ndata-1; j++) {

		npos = ((i+1)*ndata+j)*4;
		vx = (float)(i+1)/(float)(ndata-1); vy = (float)j/(float)(ndata-1); vz = 1.0f; va = 1.0f;
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

		npos = ((i+1)*ndata+j+1)*4;
		vx = (float)(i+1)/(float)(ndata-1); vy = (float)(j+1)/(float)(ndata-1); vz = 1.0f; va = 1.0f;
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

		npos = (i*ndata+j+1)*4;
		vx = (float)i/(float)(ndata-1); vy = (float)(j+1)/(float)(ndata-1); vz = 1.0f; va = 1.0f;
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

	}
	}

	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// MSCNT
	pData[0] = SCE_VIF1_SET_MSCNT(0);
	sceVif1PkAddCode(&MyVif1Packet, pData[0]);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	return 0;
}

int add_lines_to_packet_buffer(int ndata, float mpos[], float mnorm[])
// ndata should be 6 for the "r_opt1.vsm" code
{
	int i,j,npos;
	int n_poly;
	int n_packet;
	float vx,vy,vz,va;

	if (ndata > 6) {
		printf("too many polygons in a mesh.\n");
		return -1;
	}

	n_packet = sceVif1PkSize(&MyVif1Packet);

	if (n_packet + 310 > 0x10000) {
		printf("packet buffer overflowed. %d\n", n_packet);
		return -1;
	}

	n_poly = ndata*ndata;

	// --- upper triangles ---
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8000, 0x6c, 4, 4); // V4_32

	if (TME_FLAG == 0) {
	    pData[3] = 0x00000000; pData[2] = 0x00000412; pData[1] = 0x30454000; pData[0] = 0x00008000 + n_poly; //AA1 line
	} else {
	    pData[3] = 0x00000000; pData[2] = 0x00000412; pData[1] = 0x304d4000; pData[0] = 0x00008000 + n_poly; //AA1 line
//	    pData[3] = 0x00000000; pData[2] = 0x00000412; pData[1] = 0x302d4000; pData[0] = 0x00008000 + n_poly; //ABE+TME
	}
	sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);
	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// position
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8001, 0x6c, 4, 4); // V4_32

	for (i=0; i<ndata; i++) {
	for (j=0; j<ndata; j++) {

		npos = (i*ndata+j)*4;
		vx = mpos[npos]; vy = mpos[npos+1]; vz = mpos[npos+2]; va = mpos[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

	}
	}

	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// normal
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8001+n_poly, 0x6c, 4, 4); // V4_32

	for (i=0; i<ndata; i++) {
	for (j=0; j<ndata; j++) {

		npos = (i*ndata+j)*4;
		vx = mnorm[npos]; vy = mnorm[npos+1]; vz = mnorm[npos+2]; va = mnorm[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

	}
	}

	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// st
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8001+n_poly*2, 0x6c, 4, 4); // V4_32

	for (i=0; i<ndata; i++) {
	for (j=0; j<ndata; j++) {

		npos = (i*ndata+j)*4;
		vx = (float)i/(float)(ndata-1); vy = (float)j/(float)(ndata-1); vz = 1.0f; va = 1.0f;
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

	}
	}

	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// MSCNT
	pData[0] = SCE_VIF1_SET_MSCNT(0);
	sceVif1PkAddCode(&MyVif1Packet, pData[0]);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	// --- lower triangles ---
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8000, 0x6c, 4, 4); // V4_32

	if (TME_FLAG == 0) {
	    pData[3] = 0x00000000; pData[2] = 0x00000412; pData[1] = 0x30454000; pData[0] = 0x00008000 + n_poly; //AA1 line
	} else {
	    pData[3] = 0x00000000; pData[2] = 0x00000412; pData[1] = 0x304d4000; pData[0] = 0x00008000 + n_poly; //AA1 line
//	    pData[3] = 0x00000000; pData[2] = 0x00000412; pData[1] = 0x302d4000; pData[0] = 0x00008000 + n_poly; //ABE+TME
	}
	sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);
	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// position
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8001, 0x6c, 4, 4); // V4_32

	for (j=0; j<ndata; j++) {
	for (i=0; i<ndata; i++) {

		npos = (i*ndata+j)*4;
		vx = mpos[npos]; vy = mpos[npos+1]; vz = mpos[npos+2]; va = mpos[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

	}
	}

	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// normal
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8001+n_poly, 0x6c, 4, 4); // V4_32

	for (j=0; j<ndata; j++) {
	for (i=0; i<ndata; i++) {

		npos = (i*ndata+j)*4;
		vx = mnorm[npos]; vy = mnorm[npos+1]; vz = mnorm[npos+2]; va = mnorm[npos+3];
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

	}
	}

	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// st
	sceVif1PkOpenUpkCode(&MyVif1Packet, 0x8001+n_poly*2, 0x6c, 4, 4); // V4_32

	for (j=0; j<ndata; j++) {
	for (i=0; i<ndata; i++) {

		npos = (i*ndata+j)*4;
		vx = (float)i/(float)(ndata-1); vy = (float)j/(float)(ndata-1); vz = 1.0f; va = 1.0f;
		pData[0] = *(u_int*)&vx; pData[1] = *(u_int*)&vy; pData[2] = *(u_int*)&vz; pData[3] = *(u_int*)&va;
		sceVif1PkAddUpkData32N(&MyVif1Packet, pData, 4);

	}
	}

	sceVif1PkCloseUpkCode(&MyVif1Packet);

	// MSCNT
	pData[0] = SCE_VIF1_SET_MSCNT(0);
	sceVif1PkAddCode(&MyVif1Packet, pData[0]);
	sceVif1PkAlign(&MyVif1Packet, 2, 0);

	return 0;
}

int cross_product(float v1[3], float v2[3], float vr[3])
{
	vr[2] = v1[0]*v2[1]-v2[0]*v1[1];
	vr[1] = v1[2]*v2[0]-v2[2]*v1[0];
	vr[0] = v1[1]*v2[2]-v2[1]*v1[2];
	return 0;
}

float dot_product(float x[3], float y[3])
{
	return(x[0]*y[0]+x[1]*y[1]+x[2]*y[2]);
}

int normalize(float v[3])
{
	float mather;

	mather = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if (mather > 1e-6f) {
		v[0] /= mather;
		v[1] /= mather;
		v[2] /= mather;
		return 0;
	}
	return -1;
}

int get_middle_control_points(float p1[3], float p2[3], float n1[3], float n2[3], float q1[3], float q2[3], float ts[2])
{
	int i;
	float dist,t;
	float t1,t2,k1,k2,s1,s2,calpha;
	float dd[3],tmp1[3],tmp2[3];

	normalize(n1); normalize(n2);

	for (i=0; i < 3; i++) dd[i] = p2[i]-p1[i];
	dist = sqrt(dd[0]*dd[0]+dd[1]*dd[1]+dd[2]*dd[2]);

	if (dist < 0.01) {
		for (i=0; i < 3; i++){
			q1[i] = p1[i];
			q2[i] = p2[i];
		}
		return -1;
	}

	for (i=0; i < 3; i++) dd[i] /= dist;

	s1 = fabs(dot_product(n1,dd)); s2 = fabs(dot_product(n2,dd));

	if (s1 < 0.01) s1 = 0.01; if (s2 < 0.01) s2 = 0.01;

	t1 = sqrt(1.-s1*s1); t2 = sqrt(1.-s2*s2);

	if (t1 > 0.999) t1 = 0.999; if (t2 > 0.999) t2 = 0.999;
	if (t1 < 0.05) t1 = 0.05; if (t2 < 0.05) t2 = 0.05;

	k1 = (s2+0.1*t1*t2)/(s1*t2+s2*t1+0.2*t1*t2); k2 = (s1+0.1*t1*t2)/(s1*t2+s2*t1+0.2*t1*t2);

	if (k1 < 0.05) k1 = 0.05; if (k2 < 0.05) k2 = 0.05;

	t = (t1+t2)/2.; calpha = 4.*t/(3.*(1.+t));

	cross_product(dd,n1,tmp1);
	cross_product(n1,tmp1,tmp2);
	normalize(tmp2);
	for (i=0; i < 3; i++) q1[i] = p1[i]+tmp2[i]*k1*calpha*dist;
	cross_product(dd,n2,tmp1);
	cross_product(tmp1,n2,tmp2);
	normalize(tmp2);
	for (i=0; i < 3; i++) q2[i] = p2[i]+tmp2[i]*k2*calpha*dist;

	ts[0] = t1; ts[1] = t2;

	return 0;
}

int get_middle_control_points_wdir(float p1[3], float p2[3], float n1[3], float n2[3], float dir1[3], float dir2[3],
	float q1[3], float q2[3], float ts[2])
{
	int i;
	float dist,t;
	float t1,t2,k1,k2,s1,s2,calpha;
	float dd[3];

	normalize(n1); normalize(n2);
	normalize(dir1); normalize(dir2);

	for (i=0; i < 3; i++) dd[i] = p2[i]-p1[i];
	dist = sqrt(dd[0]*dd[0]+dd[1]*dd[1]+dd[2]*dd[2]);

	if (dist < 0.01) {
		for (i=0; i < 3; i++){
			q1[i] = p1[i];
			q2[i] = p2[i];
		}
		return -1;
	}

	for (i=0; i < 3; i++) dd[i] /= dist;

	t1 = fabs(dot_product(dir1,dd)); t2 = fabs(dot_product(dir2,dd));

	if (t1 > 0.999) t1 = 0.999; if (t2 > 0.999) t2 = 0.999;
	if (t1 < 0.05) t1 = 0.05; if (t2 < 0.05) t2 = 0.05;

	s1 = sqrt(1.-t1*t1); s2 = sqrt(1.-t2*t2);

	if (s1 < 0.01) s1 = 0.01; if (s2 < 0.01) s2 = 0.01;

	k1 = (s2+0.1*t1*t2)/(s1*t2+s2*t1+0.2*t1*t2); k2 = (s1+0.1*t1*t2)/(s1*t2+s2*t1+0.2*t1*t2);

	if (k1 < 0.05) k1 = 0.05; if (k2 < 0.05) k2 = 0.05;

	t = (t1+t2)/2.; calpha = 4.*t/(3.*(1.+t));

	for (i=0; i < 3; i++) q1[i] = p1[i]+dir1[i]*k1*calpha*dist;
	for (i=0; i < 3; i++) q2[i] = p2[i]-dir2[i]*k2*calpha*dist;

	ts[0] = t1; ts[1] = t2;

	return 0;
}

int pn_bezier(float t,float t1,float t2,float cp1[3],float cp2[3],float cp3[3],float cp4[3],
				float n1[3],float n2[3],float posr[3],float nrmr[3])
{
	int i;
	float bs1, bs2, bs3, bs4;
	float dpdt[3];
	float bsd1, bsd2, bsd3;
	float nl[3];
	float dist, dd[3];
	float tmp1[3], tmp2[3];

	normalize(n1); normalize(n2);

	for (i=0; i < 3; i++) dd[i] = cp4[i]-cp1[i];
	dist = sqrt(dd[0]*dd[0]+dd[1]*dd[1]+dd[2]*dd[2]);

	if (dist < 0.01) {
		for (i=0; i < 3; i++){
			posr[i] = cp1[i];
			nrmr[i] = n1[i];
		}
		return -1;
	}

	bs1 = (1.-t)*(1.-t)*(1.-t);
	bs2 = 3.*(1.-t)*(1.-t)*t;
	bs3 = 3.*(1.-t)*t*t;
	bs4 = t*t*t;

	bsd1 = 3.*(1.-t)*(1.-t);
	bsd2 = 6.*(1.-t)*t;
	bsd3 = 3.*t*t;

	for (i=0; i<3; i++) posr[i] = cp4[i]*bs1 + cp3[i]*bs2 + cp2[i]*bs3 + cp1[i]*bs4;
	for (i=0; i<3; i++) dpdt[i] = (cp3[i]-cp4[i])*bsd1 + (cp2[i]-cp3[i])*bsd2 + (cp1[i]-cp2[i])*bsd3;

	for (i=0; i<3; i++) nl[i] = t*t2*n1[i]+(1.-t)*t1*n2[i];
	cross_product(dpdt, nl, tmp1);
	cross_product(tmp1, dpdt, tmp2);

	for (i=0; i<3; i++) nrmr[i] = tmp2[i];
	normalize(nrmr);

	return 0;
}

int pn_bezier_qnorm(float t,float t1,float t2,float cp1[3],float cp2[3],float cp3[3],float cp4[3],
				float n1[3],float n2[3],float posr[3],float nrmr[3])
{
	int i;
	float bs1, bs2, bs3, bs4;
	float dpdt[3];
	float bsd1, bsd2, bsd3;
	float nl[3];
	float dist, dd[3];
	float tmp1[3], tmp2[3];

	normalize(n1); normalize(n2);

	for (i=0; i < 3; i++) dd[i] = cp4[i]-cp1[i];
	dist = sqrt(dd[0]*dd[0]+dd[1]*dd[1]+dd[2]*dd[2]);

	if (dist < 0.01) {
		for (i=0; i < 3; i++){
			posr[i] = cp1[i];
			nrmr[i] = n1[i];
		}
		return -1;
	}

	bs1 = (1.-t)*(1.-t)*(1.-t);
	bs2 = 3.*(1.-t)*(1.-t)*t;
	bs3 = 3.*(1.-t)*t*t;
	bs4 = t*t*t;

	bsd1 = 3.*(1.-t)*(1.-t);
	bsd2 = 6.*(1.-t)*t;
	bsd3 = 3.*t*t;

	for (i=0; i<3; i++) posr[i] = cp4[i]*bs1 + cp3[i]*bs2 + cp2[i]*bs3 + cp1[i]*bs4;
	for (i=0; i<3; i++) dpdt[i] = (cp3[i]-cp4[i])*bsd1 + (cp2[i]-cp3[i])*bsd2 + (cp1[i]-cp2[i])*bsd3;

	for (i=0; i<3; i++) nl[i] = t*t2*n1[i]+(1.-t)*t1*n2[i];
	cross_product(dpdt, nl, tmp1);
	cross_product(tmp1, dpdt, tmp2);

	for (i=0; i<3; i++) nrmr[i] = dpdt[i];
	normalize(nrmr);

	return 0;
}

int subdivide16(int ndiv, float in_pos[2][2][4], float in_nrm[2][2][4], float pos[16][16][4], float nrm[16][16][4])
{
	int i,j;
	float ctlpa[16][3], ctlpb[16][3];
	float ctlna[16][3], ctlnb[16][3];
	float bezier[4][3];
	float ts[2];
	float dt;
	int at_first = 0;
	int count0, count1;

	if (at_first == 0) {
		count0 = GetCOUNT();
	}

	for (i=0; i< ndiv; i++) {
	for (j=0; j< ndiv; j++) {
		pos[i][j][3] = 1.0f;
		nrm[i][j][3] = 1.0f;
	}
	}

	for (i=0; i<3; i++) {

		ctlpa[0][i] = in_pos[0][0][i];
		ctlpa[ndiv-1][i] = in_pos[1][0][i];
		ctlpb[0][i] = in_pos[0][1][i];
		ctlpb[ndiv-1][i] = in_pos[1][1][i];

		ctlna[0][i] = in_nrm[0][0][i];
		ctlna[ndiv-1][i] = in_nrm[1][0][i];
		ctlnb[0][i] = in_nrm[0][1][i];
		ctlnb[ndiv-1][i] = in_nrm[1][1][i];

	}

	//interpolation over a-side
	for (i=0; i<3; i++) {
		bezier[0][i] = ctlpa[0][i];
		bezier[3][i] = ctlpa[ndiv-1][i];
	}

	get_middle_control_points(bezier[0], bezier[3], ctlna[0], ctlna[ndiv-1], bezier[1], bezier[2], ts);

	for (i=0; i<ndiv; i++) {
		dt = (float)(ndiv-i-1)/(float)(ndiv-1);
		pn_bezier(dt,ts[0],ts[1],bezier[0],bezier[1],bezier[2],bezier[3],ctlna[0],ctlna[ndiv-1],ctlpa[i],ctlna[i]);
	}

	//interpolation over b-side
	for (i=0; i<3; i++) {
		bezier[0][i] = ctlpb[0][i];
		bezier[3][i] = ctlpb[ndiv-1][i];
	}
	get_middle_control_points(bezier[0], bezier[3], ctlnb[0], ctlnb[ndiv-1], bezier[1], bezier[2], ts);
	for (i=0; i<ndiv; i++) {
		dt = (float)(ndiv-i-1)/(float)(ndiv-1);
		pn_bezier(dt,ts[0],ts[1],bezier[0],bezier[1],bezier[2],bezier[3],ctlnb[0],ctlnb[ndiv-1],ctlpb[i],ctlnb[i]);
	}

	//interpolation between a & b
	for (j=0; j<ndiv; j++) {
		for (i=0; i<3; i++) {
			bezier[0][i] = ctlpa[j][i];
			bezier[3][i] = ctlpb[j][i];
		}
		get_middle_control_points(bezier[0], bezier[3], ctlna[j], ctlnb[j], bezier[1], bezier[2], ts);
		for (i=0; i<ndiv; i++) {
			dt = (float)(ndiv-i-1)/(float)(ndiv-1);
			pn_bezier(dt,ts[0],ts[1],bezier[0],bezier[1],bezier[2],bezier[3],ctlna[j],ctlnb[j],pos[j][i],nrm[j][i]);
		}
	}

	if (at_first == 0) {
		count1 = GetCOUNT();
//		printf("subdivide16: %d cycle\n", count1 - count0);
		at_first = 1;
	}

	return 0;
}

int subdivide16_wdir(int ndiv, float in_pos[2][2][4], float in_nrm[2][2][4], float in_dir[2][2][4],
	float pos[16][16][4], float nrm[16][16][4])
{
	int i,j;
	float ctlpa[16][3], ctlpb[16][3];
	float ctlna[16][3], ctlnb[16][3];
	float bezier[4][3];
	float ts[2];
	float dt;

	for (i=0; i< ndiv; i++) {
	for (j=0; j< ndiv; j++) {
		pos[i][j][3] = 1.0f;
		nrm[i][j][3] = 1.0f;
	}
	}

	for (i=0; i<3; i++) {

		ctlpa[0][i] = in_pos[0][0][i];
		ctlpa[ndiv-1][i] = in_pos[1][0][i];
		ctlpb[0][i] = in_pos[0][1][i];
		ctlpb[ndiv-1][i] = in_pos[1][1][i];

		ctlna[0][i] = in_nrm[0][0][i];
		ctlna[ndiv-1][i] = in_nrm[1][0][i];
		ctlnb[0][i] = in_nrm[0][1][i];
		ctlnb[ndiv-1][i] = in_nrm[1][1][i];

	}

	//interpolation over a-side
	for (i=0; i<3; i++) {
		bezier[0][i] = ctlpa[0][i];
		bezier[3][i] = ctlpa[ndiv-1][i];
	}
	get_middle_control_points_wdir(bezier[0], bezier[3], ctlna[0], ctlna[ndiv-1], in_dir[0][0], in_dir[1][0],
		bezier[1], bezier[2], ts);
	for (i=0; i<ndiv; i++) {
		dt = (float)(ndiv-i-1)/(float)(ndiv-1);
		pn_bezier(dt,ts[0],ts[1],bezier[0],bezier[1],bezier[2],bezier[3],ctlna[0],ctlna[ndiv-1],ctlpa[i],ctlna[i]);
	}

	//interpolation over b-side
	for (i=0; i<3; i++) {
		bezier[0][i] = ctlpb[0][i];
		bezier[3][i] = ctlpb[ndiv-1][i];
	}
	get_middle_control_points_wdir(bezier[0], bezier[3], ctlnb[0], ctlnb[ndiv-1], in_dir[0][1], in_dir[1][1],
		bezier[1], bezier[2], ts);
	for (i=0; i<ndiv; i++) {
		dt = (float)(ndiv-i-1)/(float)(ndiv-1);
		pn_bezier(dt,ts[0],ts[1],bezier[0],bezier[1],bezier[2],bezier[3],ctlnb[0],ctlnb[ndiv-1],ctlpb[i],ctlnb[i]);
	}

	//interpolation between a & b
	for (j=0; j<ndiv; j++) {
		for (i=0; i<3; i++) {
			bezier[0][i] = ctlpa[j][i];
			bezier[3][i] = ctlpb[j][i];
		}
		get_middle_control_points(bezier[0], bezier[3], ctlna[j], ctlnb[j], bezier[1], bezier[2], ts);
		for (i=0; i<ndiv; i++) {
			dt = (float)(ndiv-i-1)/(float)(ndiv-1);
			pn_bezier(dt,ts[0],ts[1],bezier[0],bezier[1],bezier[2],bezier[3],ctlna[j],ctlnb[j],pos[j][i],nrm[j][i]);
		}
	}

	return 0;
}

int create_patch(int flg, float in_pos[2][2][4], float in_nrm[2][2][4])
{
	int i,j,k;
	int ii,jj;
	float pos[16][16][4], nrm[16][16][4];
	float posx[6][6][4], nrmx[6][6][4];

	subdivide16(detail_level*5+6, in_pos, in_nrm, pos, nrm);

	for (ii=0; ii<detail_level+1; ii++) {
	for (jj=0; jj<detail_level+1; jj++) {

		for (i=0; i<6; i++) {
		for (j=0; j<6; j++) {

			for (k=0; k<4; k++) {

				posx[i][j][k] = pos[ii*5+i][jj*5+j][k];
				nrmx[i][j][k] = nrm[ii*5+i][jj*5+j][k];

			}

		}
		}

		if (flg == 0) {
			add_mesh_to_packet_buffer(6, (float*)posx, (float*)nrmx);
		} else {
			add_lines_to_packet_buffer(6, (float*)posx, (float*)nrmx);
		}

	}
	}

	return 0;
}

int create_patch_wdir(int flg, float in_pos[2][2][4], float in_nrm[2][2][4], float in_dir[2][2][4])
{
	int i,j,k;
	int ii,jj;
	float pos[16][16][4], nrm[16][16][4];
	float posx[6][6][4], nrmx[6][6][4];

	subdivide16_wdir(detail_level*5+6, in_pos, in_nrm, in_dir, pos, nrm);

	for (ii=0; ii<detail_level+1; ii++) {
	for (jj=0; jj<detail_level+1; jj++) {

		for (i=0; i<6; i++) {
		for (j=0; j<6; j++) {

			for (k=0; k<4; k++) {

				posx[i][j][k] = pos[ii*5+i][jj*5+j][k];
				nrmx[i][j][k] = nrm[ii*5+i][jj*5+j][k];

			}

		}
		}

		if (flg == 0) {
			add_mesh_to_packet_buffer(6, (float*)posx, (float*)nrmx);
		} else {
			add_lines_to_packet_buffer(6, (float*)posx, (float*)nrmx);
		}

	}
	}

	return 0;
}

void create_cube()
{
	int i,j;
	float in_pos[2][2][4], in_nrm[2][2][4];
	int flg = 0; //shaded triangle

	detail_level = 0;

	for (i=0; i<2; i++) {
	for (j=0; j<2; j++) {
		in_pos[i][j][3] = 1.0f;
		in_nrm[i][j][3] = 1.0f;
	}
	}

	// outer shell
	in_pos[0][1][0] = -5.0f; in_pos[0][1][1] =  5.0f; in_pos[0][1][2] =  5.0f;
	in_pos[0][0][0] =  5.0f; in_pos[0][0][1] =  5.0f; in_pos[0][0][2] =  5.0f;
	in_pos[1][1][0] = -5.0f; in_pos[1][1][1] = -5.0f; in_pos[1][1][2] =  5.0f;
	in_pos[1][0][0] =  5.0f; in_pos[1][0][1] = -5.0f; in_pos[1][0][2] =  5.0f;

	in_nrm[0][1][0] = -0.0f; in_nrm[0][1][1] =  0.0f; in_nrm[0][1][2] =  1.0f;
	in_nrm[0][0][0] =  0.0f; in_nrm[0][0][1] =  0.0f; in_nrm[0][0][2] =  1.0f;
	in_nrm[1][1][0] = -0.0f; in_nrm[1][1][1] = -0.0f; in_nrm[1][1][2] =  1.0f;
	in_nrm[1][0][0] =  0.0f; in_nrm[1][0][1] = -0.0f; in_nrm[1][0][2] =  1.0f;

	create_patch(flg, in_pos, in_nrm);

	in_pos[0][1][0] =   5.0f; in_pos[0][1][1] =  5.0f; in_pos[0][1][2] = -5.0f;
	in_pos[0][0][0] =   5.0f; in_pos[0][0][1] =  5.0f; in_pos[0][0][2] =  5.0f;
	in_pos[1][1][0] =   5.0f; in_pos[1][1][1] = -5.0f; in_pos[1][1][2] = -5.0f;
	in_pos[1][0][0] =   5.0f; in_pos[1][0][1] = -5.0f; in_pos[1][0][2] =  5.0f;

	in_nrm[0][1][0] =   1.0f; in_nrm[0][1][1] =  0.0f; in_nrm[0][1][2] =  0.0f;
	in_nrm[0][0][0] =   1.0f; in_nrm[0][0][1] =  0.0f; in_nrm[0][0][2] =  0.0f;
	in_nrm[1][1][0] =   1.0f; in_nrm[1][1][1] =  0.0f; in_nrm[1][1][2] =  0.0f;
	in_nrm[1][0][0] =   1.0f; in_nrm[1][0][1] =  0.0f; in_nrm[1][0][2] =  0.0f;

	create_patch(flg, in_pos, in_nrm);

	in_pos[0][1][0] =  5.0f; in_pos[0][1][1] =  5.0f; in_pos[0][1][2] =  -5.0f;
	in_pos[0][0][0] = -5.0f; in_pos[0][0][1] =  5.0f; in_pos[0][0][2] =  -5.0f;
	in_pos[1][1][0] =  5.0f; in_pos[1][1][1] = -5.0f; in_pos[1][1][2] =  -5.0f;
	in_pos[1][0][0] = -5.0f; in_pos[1][0][1] = -5.0f; in_pos[1][0][2] =  -5.0f;

	in_nrm[0][1][0] =  0.0f; in_nrm[0][1][1] =  0.0f; in_nrm[0][1][2] =  -1.0f;
	in_nrm[0][0][0] =  0.0f; in_nrm[0][0][1] =  0.0f; in_nrm[0][0][2] =  -1.0f;
	in_nrm[1][1][0] =  0.0f; in_nrm[1][1][1] =  0.0f; in_nrm[1][1][2] =  -1.0f;
	in_nrm[1][0][0] =  0.0f; in_nrm[1][0][1] =  0.0f; in_nrm[1][0][2] =  -1.0f;

	create_patch(flg, in_pos, in_nrm);

	in_pos[0][1][0] =  -5.0f; in_pos[0][1][1] =  5.0f; in_pos[0][1][2] =  -5.0f;
	in_pos[0][0][0] =  -5.0f; in_pos[0][0][1] =  5.0f; in_pos[0][0][2] =   5.0f;
	in_pos[1][1][0] =  -5.0f; in_pos[1][1][1] = -5.0f; in_pos[1][1][2] =  -5.0f;
	in_pos[1][0][0] =  -5.0f; in_pos[1][0][1] = -5.0f; in_pos[1][0][2] =   5.0f;

	in_nrm[0][1][0] =  -1.0f; in_nrm[0][1][1] =  0.0f; in_nrm[0][1][2] =  -0.0f;
	in_nrm[0][0][0] =  -1.0f; in_nrm[0][0][1] =  0.0f; in_nrm[0][0][2] =  -0.0f;
	in_nrm[1][1][0] =  -1.0f; in_nrm[1][1][1] =  0.0f; in_nrm[1][1][2] =  -0.0f;
	in_nrm[1][0][0] =  -1.0f; in_nrm[1][0][1] =  0.0f; in_nrm[1][0][2] =  -0.0f;

	create_patch(flg, in_pos, in_nrm);

	// bottom
	in_pos[0][0][0] = -5.0f; in_pos[0][0][1] =  5.0f; in_pos[0][0][2] = -5.0f;
	in_pos[1][0][0] =  5.0f; in_pos[1][0][1] =  5.0f; in_pos[1][0][2] = -5.0f;
	in_pos[0][1][0] = -5.0f; in_pos[0][1][1] =  5.0f; in_pos[0][1][2] =  5.0f;
	in_pos[1][1][0] =  5.0f; in_pos[1][1][1] =  5.0f; in_pos[1][1][2] =  5.0f;

	in_nrm[0][0][0] =  0.0f; in_nrm[0][0][1] =  1.0f; in_nrm[0][0][2] =  0.0f;
	in_nrm[1][0][0] =  0.0f; in_nrm[1][0][1] =  1.0f; in_nrm[1][0][2] =  0.0f;
	in_nrm[0][1][0] =  0.0f; in_nrm[0][1][1] =  1.0f; in_nrm[0][1][2] =  0.0f;
	in_nrm[1][1][0] =  0.0f; in_nrm[1][1][1] =  1.0f; in_nrm[1][1][2] =  0.0f;

	create_patch(flg, in_pos, in_nrm);

	in_pos[0][0][0] =   5.0f; in_pos[0][0][1] = -5.0f; in_pos[0][0][2] = -5.0f;
	in_pos[1][0][0] =  -5.0f; in_pos[1][0][1] = -5.0f; in_pos[1][0][2] = -5.0f;
	in_pos[0][1][0] =   5.0f; in_pos[0][1][1] = -5.0f; in_pos[0][1][2] =  5.0f;
	in_pos[1][1][0] =  -5.0f; in_pos[1][1][1] = -5.0f; in_pos[1][1][2] =  5.0f;

	in_nrm[0][0][0] =   0.0f; in_nrm[0][0][1] = -1.0f; in_nrm[0][0][2] =  0.0f;
	in_nrm[1][0][0] =  -0.0f; in_nrm[1][0][1] = -1.0f; in_nrm[1][0][2] =  0.0f;
	in_nrm[0][1][0] =   0.0f; in_nrm[0][1][1] = -1.0f; in_nrm[0][1][2] =  0.0f;
	in_nrm[1][1][0] =  -0.0f; in_nrm[1][1][1] = -1.0f; in_nrm[1][1][2] =  0.0f;

	create_patch(flg, in_pos, in_nrm);

}

void create_tube(float rad, float pos[2][3], float dir[2][3])
{
	int i,j;
	float in_pos[2][2][4], in_nrm[2][2][4];
	float in_dir[2][2][4];
	float ref_vector[3];
	float tmpx[2], tmpy[2], tmpz[2];
	float f_max; int i_max;
	float bi_vector[4][2][3]; //bi_normal vectors for direction vectors
	int flg = 0; //shaded triangle

	for (i=0; i<2; i++) {
	for (j=0; j<2; j++) {
		in_pos[i][j][3] = 1.0f;
		in_nrm[i][j][3] = 1.0f;
	}
	}

	//get reference vector
	//x axis
	ref_vector[0] = 1.0f; ref_vector[1] = 0.0f; ref_vector[2] = 0.0f;
	tmpx[0] = fabs(dot_product(ref_vector, dir[0]));
	tmpx[1] = fabs(dot_product(ref_vector, dir[1]));

	//y axis
	ref_vector[0] = 0.0f; ref_vector[1] = 1.0f; ref_vector[2] = 0.0f;
	tmpy[0] = fabs(dot_product(ref_vector, dir[0]));
	tmpy[1] = fabs(dot_product(ref_vector, dir[1]));

	//z axis
	ref_vector[0] = 0.0f; ref_vector[1] = 0.0f; ref_vector[2] = 1.0f;
	tmpz[0] = fabs(dot_product(ref_vector, dir[0]));
	tmpz[1] = fabs(dot_product(ref_vector, dir[1]));

	f_max = 1000.0f; i_max = 0;
	if (tmpx[0]+tmpx[1] < f_max) {
		f_max = tmpx[0]+tmpx[1];
		i_max = 1;
	}
	if (tmpy[0]+tmpy[1] < f_max) {
		f_max = tmpy[0]+tmpy[1];
		i_max = 2;
	}
	if (tmpz[0]+tmpz[1] < f_max) {
		f_max = tmpz[0]+tmpz[1];
		i_max = 3;
	}

	if (i_max == 0) {
		return;
	} else if (i_max == 1) { //x axis
		ref_vector[0] = 1.0f; ref_vector[1] = 0.0f; ref_vector[2] = 0.0f;
	} else if (i_max == 2) { //y axis
		ref_vector[0] = 0.0f; ref_vector[1] = 1.0f; ref_vector[2] = 0.0f;
	} else if (i_max == 3) { //z axis
		ref_vector[0] = 0.0f; ref_vector[1] = 0.0f; ref_vector[2] = 1.0f;
	}

	//get bi_normal vectors
	for (i=0; i<2; i++) {
		cross_product(ref_vector, dir[i], bi_vector[0][i]);
		cross_product(dir[i], ref_vector, bi_vector[1][i]);
		cross_product(bi_vector[0][i], dir[i], bi_vector[2][i]);
		cross_product(bi_vector[1][i], dir[i], bi_vector[3][i]);
		for (j=0; j<3; j++) {
			bi_vector[0][i][j] *= rad;
			bi_vector[1][i][j] *= rad;
			bi_vector[2][i][j] *= rad;
			bi_vector[3][i][j] *= rad;
		}
	}

	//patch 1
	for (j=0; j<3; j++) {
		in_pos[0][0][j] = pos[0][j] + bi_vector[3][0][j];
		in_pos[1][0][j] = pos[1][j] + bi_vector[3][1][j];
		in_pos[0][1][j] = pos[0][j] + bi_vector[0][0][j];
		in_pos[1][1][j] = pos[1][j] + bi_vector[0][1][j];

		in_nrm[0][0][j] = bi_vector[3][0][j];
		in_nrm[1][0][j] = bi_vector[3][1][j];
		in_nrm[0][1][j] = bi_vector[0][0][j];
		in_nrm[1][1][j] = bi_vector[0][1][j];

		in_dir[0][0][j] = dir[0][j];
		in_dir[1][0][j] = dir[1][j];
		in_dir[0][1][j] = dir[0][j];
		in_dir[1][1][j] = dir[1][j];
	}

	create_patch_wdir(flg, in_pos, in_nrm, in_dir);

	//patch 2
	for (j=0; j<3; j++) {
		in_pos[0][0][j] = pos[0][j] + bi_vector[0][0][j];
		in_pos[1][0][j] = pos[1][j] + bi_vector[0][1][j];
		in_pos[0][1][j] = pos[0][j] + bi_vector[2][0][j];
		in_pos[1][1][j] = pos[1][j] + bi_vector[2][1][j];

		in_nrm[0][0][j] = bi_vector[0][0][j];
		in_nrm[1][0][j] = bi_vector[0][1][j];
		in_nrm[0][1][j] = bi_vector[2][0][j];
		in_nrm[1][1][j] = bi_vector[2][1][j];

		in_dir[0][0][j] = dir[0][j];
		in_dir[1][0][j] = dir[1][j];
		in_dir[0][1][j] = dir[0][j];
		in_dir[1][1][j] = dir[1][j];
	}

	create_patch_wdir(flg, in_pos, in_nrm, in_dir);

	//patch 3
	for (j=0; j<3; j++) {
		in_pos[0][0][j] = pos[0][j] + bi_vector[2][0][j];
		in_pos[1][0][j] = pos[1][j] + bi_vector[2][1][j];
		in_pos[0][1][j] = pos[0][j] + bi_vector[1][0][j];
		in_pos[1][1][j] = pos[1][j] + bi_vector[1][1][j];

		in_nrm[0][0][j] = bi_vector[2][0][j];
		in_nrm[1][0][j] = bi_vector[2][1][j];
		in_nrm[0][1][j] = bi_vector[1][0][j];
		in_nrm[1][1][j] = bi_vector[1][1][j];

		in_dir[0][0][j] = dir[0][j];
		in_dir[1][0][j] = dir[1][j];
		in_dir[0][1][j] = dir[0][j];
		in_dir[1][1][j] = dir[1][j];
	}

	create_patch_wdir(flg, in_pos, in_nrm, in_dir);

	//patch 4
	for (j=0; j<3; j++) {
		in_pos[0][0][j] = pos[0][j] + bi_vector[1][0][j];
		in_pos[1][0][j] = pos[1][j] + bi_vector[1][1][j];
		in_pos[0][1][j] = pos[0][j] + bi_vector[3][0][j];
		in_pos[1][1][j] = pos[1][j] + bi_vector[3][1][j];

		in_nrm[0][0][j] = bi_vector[1][0][j];
		in_nrm[1][0][j] = bi_vector[1][1][j];
		in_nrm[0][1][j] = bi_vector[3][0][j];
		in_nrm[1][1][j] = bi_vector[3][1][j];

		in_dir[0][0][j] = dir[0][j];
		in_dir[1][0][j] = dir[1][j];
		in_dir[0][1][j] = dir[0][j];
		in_dir[1][1][j] = dir[1][j];
	}

	create_patch_wdir(flg, in_pos, in_nrm, in_dir);

}

void create_handle()
{
	float pos[2][3], dir[2][3];

	detail_level = 2;

	pos[0][0] = 5.0f; pos[0][1] = 0.0f; pos[0][2] = 0.0f;
	dir[0][0] = 1.0f; dir[0][1] = 0.0f; dir[0][2] = 0.0f;

	pos[1][0] = 10.0f; pos[1][1] =-5.0f; pos[1][2] = 2.0f;
	dir[1][0] = 0.0f; dir[1][1] =-1.0f; dir[1][2] = 0.0f;

	create_tube(0.9f, pos, dir);

	pos[0][0] = 10.0f; pos[0][1] =-5.0f; pos[0][2] = 2.0f;
	dir[0][0] = 0.0f; dir[0][1] =-1.0f; dir[0][2] = 0.0f;

	pos[1][0] = 5.0f; pos[1][1] =-10.0f; pos[1][2] = -2.0f;
	dir[1][0] =-1.0f; dir[1][1] = 0.0f; dir[1][2] = 0.0f;

	create_tube(0.9f, pos, dir);

	pos[0][0] = 5.0f; pos[0][1] =-10.0f; pos[0][2] = -2.0f;
	dir[0][0] =-1.0f; dir[0][1] = 0.0f; dir[0][2] = 0.0f;

	pos[1][0] = 0.0f; pos[1][1] =-5.0f; pos[1][2] = 0.0f;
	dir[1][0] = 0.0f; dir[1][1] = 1.0f; dir[1][2] = 0.0f;

	create_tube(0.9f, pos, dir);

}

void create_cube_line()
{
	int i,j;
	float in_pos[2][2][4], in_nrm[2][2][4];
	int flg = 1; //shaded line

	detail_level = 0;

	for (i=0; i<2; i++) {
	for (j=0; j<2; j++) {
		in_pos[i][j][3] = 1.0f;
		in_nrm[i][j][3] = 1.0f;
	}
	}

	// outer shell
	in_pos[0][1][0] = -5.0f; in_pos[0][1][1] =  5.0f; in_pos[0][1][2] =  5.0f;
	in_pos[0][0][0] =  5.0f; in_pos[0][0][1] =  5.0f; in_pos[0][0][2] =  5.0f;
	in_pos[1][1][0] = -5.0f; in_pos[1][1][1] = -5.0f; in_pos[1][1][2] =  5.0f;
	in_pos[1][0][0] =  5.0f; in_pos[1][0][1] = -5.0f; in_pos[1][0][2] =  5.0f;

	in_nrm[0][1][0] =  0.0f; in_nrm[0][1][1] =  0.0f; in_nrm[0][1][2] =  1.0f;
	in_nrm[0][0][0] =  0.0f; in_nrm[0][0][1] =  0.0f; in_nrm[0][0][2] =  1.0f;
	in_nrm[1][1][0] =  0.0f; in_nrm[1][1][1] =  0.0f; in_nrm[1][1][2] =  1.0f;
	in_nrm[1][0][0] =  0.0f; in_nrm[1][0][1] =  0.0f; in_nrm[1][0][2] =  1.0f;

	create_patch(flg, in_pos, in_nrm);

	in_pos[0][1][0] =   5.0f; in_pos[0][1][1] =  5.0f; in_pos[0][1][2] = -5.0f;
	in_pos[0][0][0] =   5.0f; in_pos[0][0][1] =  5.0f; in_pos[0][0][2] =  5.0f;
	in_pos[1][1][0] =   5.0f; in_pos[1][1][1] = -5.0f; in_pos[1][1][2] = -5.0f;
	in_pos[1][0][0] =   5.0f; in_pos[1][0][1] = -5.0f; in_pos[1][0][2] =  5.0f;

	in_nrm[0][1][0] =   1.0f; in_nrm[0][1][1] =  0.0f; in_nrm[0][1][2] =  0.0f;
	in_nrm[0][0][0] =   1.0f; in_nrm[0][0][1] =  0.0f; in_nrm[0][0][2] =  0.0f;
	in_nrm[1][1][0] =   1.0f; in_nrm[1][1][1] =  0.0f; in_nrm[1][1][2] =  0.0f;
	in_nrm[1][0][0] =   1.0f; in_nrm[1][0][1] =  0.0f; in_nrm[1][0][2] =  0.0f;

	create_patch(flg, in_pos, in_nrm);

	in_pos[0][1][0] =  5.0f; in_pos[0][1][1] =  5.0f; in_pos[0][1][2] =  -5.0f;
	in_pos[0][0][0] = -5.0f; in_pos[0][0][1] =  5.0f; in_pos[0][0][2] =  -5.0f;
	in_pos[1][1][0] =  5.0f; in_pos[1][1][1] = -5.0f; in_pos[1][1][2] =  -5.0f;
	in_pos[1][0][0] = -5.0f; in_pos[1][0][1] = -5.0f; in_pos[1][0][2] =  -5.0f;

	in_nrm[0][1][0] =  0.0f; in_nrm[0][1][1] =  0.0f; in_nrm[0][1][2] =  -1.0f;
	in_nrm[0][0][0] =  0.0f; in_nrm[0][0][1] =  0.0f; in_nrm[0][0][2] =  -1.0f;
	in_nrm[1][1][0] =  0.0f; in_nrm[1][1][1] =  0.0f; in_nrm[1][1][2] =  -1.0f;
	in_nrm[1][0][0] =  0.0f; in_nrm[1][0][1] =  0.0f; in_nrm[1][0][2] =  -1.0f;

	create_patch(flg, in_pos, in_nrm);

	in_pos[0][1][0] =  -5.0f; in_pos[0][1][1] =  5.0f; in_pos[0][1][2] =  -5.0f;
	in_pos[0][0][0] =  -5.0f; in_pos[0][0][1] =  5.0f; in_pos[0][0][2] =   5.0f;
	in_pos[1][1][0] =  -5.0f; in_pos[1][1][1] = -5.0f; in_pos[1][1][2] =  -5.0f;
	in_pos[1][0][0] =  -5.0f; in_pos[1][0][1] = -5.0f; in_pos[1][0][2] =   5.0f;

	in_nrm[0][1][0] =  -1.0f; in_nrm[0][1][1] =  0.0f; in_nrm[0][1][2] =  -0.0f;
	in_nrm[0][0][0] =  -1.0f; in_nrm[0][0][1] =  0.0f; in_nrm[0][0][2] =  -0.0f;
	in_nrm[1][1][0] =  -1.0f; in_nrm[1][1][1] =  0.0f; in_nrm[1][1][2] =  -0.0f;
	in_nrm[1][0][0] =  -1.0f; in_nrm[1][0][1] =  0.0f; in_nrm[1][0][2] =  -0.0f;

	create_patch(flg, in_pos, in_nrm);

	// bottom
	in_pos[0][0][0] = -5.0f; in_pos[0][0][1] =  5.0f; in_pos[0][0][2] = -5.0f;
	in_pos[1][0][0] =  5.0f; in_pos[1][0][1] =  5.0f; in_pos[1][0][2] = -5.0f;
	in_pos[0][1][0] = -5.0f; in_pos[0][1][1] =  5.0f; in_pos[0][1][2] =  5.0f;
	in_pos[1][1][0] =  5.0f; in_pos[1][1][1] =  5.0f; in_pos[1][1][2] =  5.0f;

	in_nrm[0][0][0] =  0.0f; in_nrm[0][0][1] =  1.0f; in_nrm[0][0][2] =  0.0f;
	in_nrm[1][0][0] =  0.0f; in_nrm[1][0][1] =  1.0f; in_nrm[1][0][2] =  0.0f;
	in_nrm[0][1][0] =  0.0f; in_nrm[0][1][1] =  1.0f; in_nrm[0][1][2] =  0.0f;
	in_nrm[1][1][0] =  0.0f; in_nrm[1][1][1] =  1.0f; in_nrm[1][1][2] =  0.0f;

	create_patch(flg, in_pos, in_nrm);

	in_pos[0][0][0] =   5.0f; in_pos[0][0][1] = -5.0f; in_pos[0][0][2] = -5.0f;
	in_pos[1][0][0] =  -5.0f; in_pos[1][0][1] = -5.0f; in_pos[1][0][2] = -5.0f;
	in_pos[0][1][0] =   5.0f; in_pos[0][1][1] = -5.0f; in_pos[0][1][2] =  5.0f;
	in_pos[1][1][0] =  -5.0f; in_pos[1][1][1] = -5.0f; in_pos[1][1][2] =  5.0f;

	in_nrm[0][0][0] =   0.0f; in_nrm[0][0][1] = -1.0f; in_nrm[0][0][2] =  0.0f;
	in_nrm[1][0][0] =  -0.0f; in_nrm[1][0][1] = -1.0f; in_nrm[1][0][2] =  0.0f;
	in_nrm[0][1][0] =   0.0f; in_nrm[0][1][1] = -1.0f; in_nrm[0][1][2] =  0.0f;
	in_nrm[1][1][0] =  -0.0f; in_nrm[1][1][1] = -1.0f; in_nrm[1][1][2] =  0.0f;

	create_patch(flg, in_pos, in_nrm);

}

void create_tube_line(float rad, float pos[2][3], float dir[2][3])
{
	int i,j;
	float in_pos[2][2][4], in_nrm[2][2][4];
	float in_dir[2][2][4];
	float ref_vector[3];
	float tmpx[2], tmpy[2], tmpz[2];
	float f_max; int i_max;
	float bi_vector[4][2][3]; //bi_normal vectors for direction vectors
	int flg = 1; //line

	for (i=0; i<2; i++) {
	for (j=0; j<2; j++) {
		in_pos[i][j][3] = 1.0f;
		in_nrm[i][j][3] = 1.0f;
	}
	}

	//get reference vector
	//x axis
	ref_vector[0] = 1.0f; ref_vector[1] = 0.0f; ref_vector[2] = 0.0f;
	tmpx[0] = fabs(dot_product(ref_vector, dir[0]));
	tmpx[1] = fabs(dot_product(ref_vector, dir[1]));

	//y axis
	ref_vector[0] = 0.0f; ref_vector[1] = 1.0f; ref_vector[2] = 0.0f;
	tmpy[0] = fabs(dot_product(ref_vector, dir[0]));
	tmpy[1] = fabs(dot_product(ref_vector, dir[1]));

	//z axis
	ref_vector[0] = 0.0f; ref_vector[1] = 0.0f; ref_vector[2] = 1.0f;
	tmpz[0] = fabs(dot_product(ref_vector, dir[0]));
	tmpz[1] = fabs(dot_product(ref_vector, dir[1]));

	f_max = 1000.0f; i_max = 0;
	if (tmpx[0]+tmpx[1] < f_max) {
		f_max = tmpx[0]+tmpx[1];
		i_max = 1;
	}
	if (tmpy[0]+tmpy[1] < f_max) {
		f_max = tmpy[0]+tmpy[1];
		i_max = 2;
	}
	if (tmpz[0]+tmpz[1] < f_max) {
		f_max = tmpz[0]+tmpz[1];
		i_max = 3;
	}

	if (i_max == 0) {
		return;
	} else if (i_max == 1) { //x axis
		ref_vector[0] = 1.0f; ref_vector[1] = 0.0f; ref_vector[2] = 0.0f;
	} else if (i_max == 2) { //y axis
		ref_vector[0] = 0.0f; ref_vector[1] = 1.0f; ref_vector[2] = 0.0f;
	} else if (i_max == 3) { //z axis
		ref_vector[0] = 0.0f; ref_vector[1] = 0.0f; ref_vector[2] = 1.0f;
	}

	//get bi_normal vectors
	for (i=0; i<2; i++) {
		cross_product(ref_vector, dir[i], bi_vector[0][i]);
		cross_product(dir[i], ref_vector, bi_vector[1][i]);
		cross_product(bi_vector[0][i], dir[i], bi_vector[2][i]);
		cross_product(bi_vector[1][i], dir[i], bi_vector[3][i]);
		for (j=0; j<3; j++) {
			bi_vector[0][i][j] *= rad;
			bi_vector[1][i][j] *= rad;
			bi_vector[2][i][j] *= rad;
			bi_vector[3][i][j] *= rad;
		}
	}

	//patch 1
	for (j=0; j<3; j++) {
		in_pos[0][0][j] = pos[0][j] + bi_vector[3][0][j];
		in_pos[1][0][j] = pos[1][j] + bi_vector[3][1][j];
		in_pos[0][1][j] = pos[0][j] + bi_vector[0][0][j];
		in_pos[1][1][j] = pos[1][j] + bi_vector[0][1][j];

		in_nrm[0][0][j] = bi_vector[3][0][j];
		in_nrm[1][0][j] = bi_vector[3][1][j];
		in_nrm[0][1][j] = bi_vector[0][0][j];
		in_nrm[1][1][j] = bi_vector[0][1][j];

		in_dir[0][0][j] = dir[0][j];
		in_dir[1][0][j] = dir[1][j];
		in_dir[0][1][j] = dir[0][j];
		in_dir[1][1][j] = dir[1][j];
	}

	create_patch_wdir(flg, in_pos, in_nrm, in_dir);

	//patch 2
	for (j=0; j<3; j++) {
		in_pos[0][0][j] = pos[0][j] + bi_vector[0][0][j];
		in_pos[1][0][j] = pos[1][j] + bi_vector[0][1][j];
		in_pos[0][1][j] = pos[0][j] + bi_vector[2][0][j];
		in_pos[1][1][j] = pos[1][j] + bi_vector[2][1][j];

		in_nrm[0][0][j] = bi_vector[0][0][j];
		in_nrm[1][0][j] = bi_vector[0][1][j];
		in_nrm[0][1][j] = bi_vector[2][0][j];
		in_nrm[1][1][j] = bi_vector[2][1][j];

		in_dir[0][0][j] = dir[0][j];
		in_dir[1][0][j] = dir[1][j];
		in_dir[0][1][j] = dir[0][j];
		in_dir[1][1][j] = dir[1][j];
	}

	create_patch_wdir(flg, in_pos, in_nrm, in_dir);

	//patch 3
	for (j=0; j<3; j++) {
		in_pos[0][0][j] = pos[0][j] + bi_vector[2][0][j];
		in_pos[1][0][j] = pos[1][j] + bi_vector[2][1][j];
		in_pos[0][1][j] = pos[0][j] + bi_vector[1][0][j];
		in_pos[1][1][j] = pos[1][j] + bi_vector[1][1][j];

		in_nrm[0][0][j] = bi_vector[2][0][j];
		in_nrm[1][0][j] = bi_vector[2][1][j];
		in_nrm[0][1][j] = bi_vector[1][0][j];
		in_nrm[1][1][j] = bi_vector[1][1][j];

		in_dir[0][0][j] = dir[0][j];
		in_dir[1][0][j] = dir[1][j];
		in_dir[0][1][j] = dir[0][j];
		in_dir[1][1][j] = dir[1][j];
	}

	create_patch_wdir(flg, in_pos, in_nrm, in_dir);

	//patch 4
	for (j=0; j<3; j++) {
		in_pos[0][0][j] = pos[0][j] + bi_vector[1][0][j];
		in_pos[1][0][j] = pos[1][j] + bi_vector[1][1][j];
		in_pos[0][1][j] = pos[0][j] + bi_vector[3][0][j];
		in_pos[1][1][j] = pos[1][j] + bi_vector[3][1][j];

		in_nrm[0][0][j] = bi_vector[1][0][j];
		in_nrm[1][0][j] = bi_vector[1][1][j];
		in_nrm[0][1][j] = bi_vector[3][0][j];
		in_nrm[1][1][j] = bi_vector[3][1][j];

		in_dir[0][0][j] = dir[0][j];
		in_dir[1][0][j] = dir[1][j];
		in_dir[0][1][j] = dir[0][j];
		in_dir[1][1][j] = dir[1][j];
	}

	create_patch_wdir(flg, in_pos, in_nrm, in_dir);

}

void create_handle_line()
{
	float pos[2][3], dir[2][3];

	detail_level = 2;

	pos[0][0] = 5.0f; pos[0][1] = 0.0f; pos[0][2] = 0.0f;
	dir[0][0] = 1.0f; dir[0][1] = 0.0f; dir[0][2] = 0.0f;

	pos[1][0] = 10.0f; pos[1][1] =-5.0f; pos[1][2] = 2.0f;
	dir[1][0] = 0.0f; dir[1][1] =-1.0f; dir[1][2] = 0.0f;

	create_tube_line(0.9f, pos, dir);

	pos[0][0] = 10.0f; pos[0][1] =-5.0f; pos[0][2] = 2.0f;
	dir[0][0] = 0.0f; dir[0][1] =-1.0f; dir[0][2] = 0.0f;

	pos[1][0] = 5.0f; pos[1][1] =-10.0f; pos[1][2] = -2.0f;
	dir[1][0] =-1.0f; dir[1][1] = 0.0f; dir[1][2] = 0.0f;

	create_tube_line(0.9f, pos, dir);

	pos[0][0] = 5.0f; pos[0][1] =-10.0f; pos[0][2] = -2.0f;
	dir[0][0] =-1.0f; dir[0][1] = 0.0f; dir[0][2] = 0.0f;

	pos[1][0] = 0.0f; pos[1][1] =-5.0f; pos[1][2] = 0.0f;
	dir[1][0] = 0.0f; dir[1][1] = 1.0f; dir[1][2] = 0.0f;

	create_tube_line(0.9f, pos, dir);

}
