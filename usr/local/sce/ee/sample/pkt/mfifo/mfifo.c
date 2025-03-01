/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *               Emotion Engine Library Sample Program
 *
 *                           - <mfifo> -
 *
 *                          Version <1.00>
 *                               SJIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                            <mfifo.c>
 *              <main function of memory fifo sample>
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      1.00            June,14,1999    akiyuki     first version
 */

#include <eekernel.h>
#include <eeregs.h>
#include <stdlib.h>
#include <stdio.h>
#include <libdev.h>
#include <libgraph.h>
#include <libdma.h>
#include <libpkt.h>
#include <libvu0.h>
#include <libpad.h>
#include <sifdev.h>
#include <sifrpc.h>

#define SCREEN_WIDTH	(640)
#define SCREEN_HEIGHT	(224)
#define OFFX		(((4096-SCREEN_WIDTH)/2)<<4)
#define OFFY		(((4096-SCREEN_HEIGHT)/2)<<4)
#define PI		(3.141592f)
#define SCE_PADreset	(SCE_PADL1|SCE_PADL2|SCE_PADR1|SCE_PADR2)
#define bulkof(a)	(sizeof(a)/sizeof(a[0]))
#define min(a,b)	(((a)<(b))?(a):(b))
#define max(a,b)	(((a)>(b))?(a):(b))

#define	FOG	(10.f)
#define	DELTA	(1.f*PI/180.f)		// ��]�p�P��

#define	VX	80.f		// object�̑傫��(���S����̋���)
#define	VY	80.f
#define	VZ	80.f
#define	VW	1.f

#define	CR	255.f			// object color
#define	CG	255.f
#define	CB	255.f
#define	CA	0.f

#define	CAMZ	(0.f)		// camera��Z�ʒu
#define	OBJZ	(3000.f)	// object�ʒu(z)
#define	OBJL	(210.f)		// object�Ԃ̋���
#define	NX	(10)		// object�\����(x)
#define	NY	(10)		// object�\����(y)
#define	NZ	(100)		// object�\����(z)
#define	OBJECTS	(NX*NY*NZ)	// �ő�object��

#define	SPROBJ	(20)		// SPR�ɒu��object�̐�
#define	OBJSIZE	(24/2)		// object���packet size(qword)
#define	PKTSIZE	(SPROBJ*OBJSIZE+16)// SPR�Ɋm�ۂ���packet size(qword)
#define	MFIFO_SIZE	(0x40000)// Memory FIFO size(byte)
// ��global�ϐ��ŗ̈�m�ۂ���ꍇ��alignment�𑵂�����ő�l��0x8000�ƂȂ�


/* TransData�^
	object�`��ʒu��x,y,z���W���i�[
	i�Ԗڂ�object�`��ʒu�͈ȉ��̂悤�ɋ��߂�
	{ x, y, z } = { x[i%NX], y[(i%(NX*NY))/NX], z[i/(NX*NY)] }
*/
typedef struct {
	float		x[NX];
	float		y[NY];
	float		z[NZ];
} TransData;

// WorkData�^:	��Ɨ̈�\����
typedef struct
{
	TransData xyz;
	sceVu0FVECTOR xyzl;	// x,y,z:����offset, l:object�Ԃ̋���
	sceVu0FVECTOR vf[8];	// object�e���_�̍��W
	sceVu0FVECTOR cf[6];	// object�e�ʂ̐F
	sceVu0FVECTOR normal[6];// object�e�ʂ̖@��
	sceVu0FMATRIX Rotate;
	sceVu0FMATRIX WorldScreen;
	sceVu0FMATRIX NormalLight;
	sceVu0FMATRIX LightColor;
	sceVu0FMATRIX LocalLight;
} WorkData;

/* ScratchPadRam
	�X�N���b�`�p�b�h�����Ɋi�[����f�[�^��\��
	���̍\���̂̑傫����16KB�𒴂��Ȃ��悤�ɂ���
*/
typedef struct {
	u_long128 pBase[2][PKTSIZE];	// Packet�̈�(Double Buffer)
	WorkData wd;		// ��Ɨp�s��Ȃ�
	sceGsDBuff db;			// for GS Double Buffer
} ScratchPadRam;

// xyz�̕`��ʒu����xyzl�ɏ]���Đݒ肷��
void SetTransData( WorkData *wd )
{
	int i;
	float offset;
	offset = -wd->xyzl[3]*(NX-1)/2.f+wd->xyzl[0];
	for ( i = 0; i < NX; i++ ) {
		wd->xyz.x[i] = wd->xyzl[3]*i+offset;
	}
	offset = -wd->xyzl[3]*(NY-1)/2.f+wd->xyzl[1];
	for ( i = 0; i < NY; i++ ) {
		wd->xyz.y[i] = wd->xyzl[3]*i+offset;
	}
	for ( i = 0; i < NZ; i++ ) {
		wd->xyz.z[i] = OBJZ+wd->xyzl[3]*i+wd->xyzl[2];
	}
}

// �@���x�N�g���̍쐬
void MakeNormal( sceVu0FVECTOR o,
	sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2 )
{
	sceVu0FVECTOR w1, w2;
	sceVu0SubVector( w1, v1, v0 );
	sceVu0SubVector( w2, v2, v0 );
	sceVu0OuterProduct( o, w1, w2 );
	sceVu0Normalize( o, o );
}

// �e��f�[�^�̏��������s��
void InitWorkData( WorkData *wd )
{
	sceVu0FVECTOR _vf[] = {
		{ -VX,  VY, -VZ,  VW },
		{ -VX,  VY,  VZ,  VW },
		{ -VX, -VY, -VZ,  VW },
		{ -VX, -VY,  VZ,  VW },
		{  VX, -VY, -VZ,  VW },
		{  VX, -VY,  VZ,  VW },
		{  VX,  VY, -VZ,  VW },
		{  VX,  VY,  VZ,  VW },
	};
	sceVu0FVECTOR _cf[] = {
		{ CR, CG, CB, CA },
		{ CR, CG, CB, CA },
		{ CR, CG, CB, CA },
		{ CR, CG, CB, CA },
		{ CR, CG, CB, CA },
		{ CR, CG, CB, CA },
	};
// �J�����̈ʒu�ƌ�����y���ݒ�
	sceVu0FVECTOR camera_p  = { 0.0f, 0.0f, CAMZ, 1.0f };
	sceVu0FVECTOR camera_zd = { 0.0f, 0.0f, 1.0f, 1.0f };
	sceVu0FVECTOR camera_yd = { 0.0f, 1.0f, 0.0f, 1.0f };

	sceVu0FMATRIX WorldView;
	sceVu0FMATRIX ViewScreen;
// �O�̃��C�g���ꂼ��̐F
	sceVu0FVECTOR col0 = { 0.8f, 0.3f, 0.3f, 1.0f };
	sceVu0FVECTOR col1 = { 0.3f, 0.8f, 0.3f, 1.0f };
	sceVu0FVECTOR col2 = { 0.3f, 0.3f, 0.8f, 1.0f };
// ���F
	sceVu0FVECTOR ambient = { 0.15f, 0.15f, 0.15f, 0.0f };
// �O�̃��C�g�̌�����ݒ�
	sceVu0FVECTOR light0, light1, light2;
	sceVu0FMATRIX matrix = {
		{ 0.0f,-1.7320508f, 1.0f, 1.0f },// ��ڂ̃��C�g
		{ 0.0f, 0.0f, 0.0f, 0.0f },	// �_�~�[
		{ 0.0f, 0.0f, 0.0f, 0.0f },	// �_�~�[
		{ 0.0f, 0.0f, 0.0f, 0.0f },	// �_�~�[
	};
	sceVu0CopyVector( light0, matrix[0] );
	sceVu0RotMatrixZ( matrix, matrix, PI*2.f/3.f );// Z�����S��120�x��]
	sceVu0CopyVector( light1, matrix[0] );
	sceVu0RotMatrixZ( matrix, matrix, PI*2.f/3.f );// Z�����S��120�x��]
	sceVu0CopyVector( light2, matrix[0] );
// �����v�Z�ɗp����}�g���N�X���쐬
	sceVu0NormalLightMatrix( wd->NormalLight, light0, light1, light2 );
	sceVu0LightColorMatrix( wd->LightColor, col0, col1, col2, ambient );
// �J�������_ ( world -> view )
	sceVu0CameraMatrix( WorldView, camera_p, camera_zd, camera_yd );
// �����ϊ��}�g���N�X�𐶐� ( view -> screen )
	sceVu0ViewScreenMatrix( ViewScreen, 512.0f, 1.0f, 0.47f, 2048.0f,
		2048.0f, 1.0f, 16777215.0f, 64.0f, 65536.0f );	// view->screen
	sceVu0MulMatrix( wd->WorldScreen, ViewScreen, WorldView );
	wd->xyzl[0] = wd->xyzl[1] = wd->xyzl[2] = 0.f;
	wd->xyzl[3] = OBJL;
	SetTransData( wd );
	memcpy( wd->vf, _vf, sizeof(_vf) );
	memcpy( wd->cf, _cf, sizeof(_cf) );
	MakeNormal( wd->normal[0], wd->vf[1], wd->vf[3], wd->vf[7] );
	MakeNormal( wd->normal[1], wd->vf[0], wd->vf[1], wd->vf[6] );
	MakeNormal( wd->normal[2], wd->vf[2], wd->vf[0], wd->vf[4] );
	MakeNormal( wd->normal[3], wd->vf[0], wd->vf[2], wd->vf[1] );
	MakeNormal( wd->normal[4], wd->vf[2], wd->vf[4], wd->vf[3] );
	MakeNormal( wd->normal[5], wd->vf[4], wd->vf[6], wd->vf[5] );
}

// �����v�Z���������Ƃ�RGB�l
void LightRGB( sceVu0IVECTOR *ci, WorkData *wd, int n )
{
	int i;
	sceVu0FVECTOR vector;
	for ( i = 0; i < n; i++ ) {
		sceVu0ApplyMatrix( vector, wd->LocalLight, wd->normal[i] );
		sceVu0ClampVector( vector, vector, 0.0f, 1.0f );
		vector[3] = 1.0f;
		sceVu0ApplyMatrix( vector, wd->LightColor, vector );
		sceVu0MulVector( vector, wd->cf[i], vector );
		sceVu0FTOI0Vector( *ci++, vector );
	}
}

// �����ƒ��_����GS�ɓn��packet���쐬(TriangleFan)
void SetPacket( sceGifPacket *packet,
	sceVu0IVECTOR c0,sceVu0IVECTOR c1,sceVu0IVECTOR c2,float fog,
	sceVu0IVECTOR v0,sceVu0IVECTOR v1,sceVu0IVECTOR v2,sceVu0IVECTOR v3,
	sceVu0IVECTOR v4,sceVu0IVECTOR v5,sceVu0IVECTOR v6,sceVu0IVECTOR v7 )
{
	sceGifPkAddGsData( packet, SCE_GS_SET_PRIM(0x05,0,0,0,0,0,0,0,0) );
	sceGifPkAddGsData( packet,
		SCE_GS_SET_RGBAQ(c0[0],c0[1],c0[2],0x00,*(u_int *)&fog) );
	sceGifPkAddGsData( packet, SCE_GS_SET_XYZ(v0[0],v0[1],v0[2]) );
	sceGifPkAddGsData( packet, SCE_GS_SET_XYZ(v1[0],v1[1],v1[2]) );
	sceGifPkAddGsData( packet, SCE_GS_SET_XYZ(v2[0],v2[1],v2[2]) );
	sceGifPkAddGsData( packet, SCE_GS_SET_XYZ(v3[0],v3[1],v3[2]) );
	sceGifPkAddGsData( packet,
		SCE_GS_SET_RGBAQ(c1[0],c1[1],c1[2],0x00,*(u_int *)&fog) );
	sceGifPkAddGsData( packet, SCE_GS_SET_XYZ(v4[0],v4[1],v4[2]) );
	sceGifPkAddGsData( packet, SCE_GS_SET_XYZ(v5[0],v5[1],v5[2]) );
	sceGifPkAddGsData( packet,
		SCE_GS_SET_RGBAQ(c2[0],c2[1],c2[2],0x00,*(u_int *)&fog) );
	sceGifPkAddGsData( packet, SCE_GS_SET_XYZ(v6[0],v6[1],v6[2]) );
	sceGifPkAddGsData( packet, SCE_GS_SET_XYZ(v7[0],v7[1],v7[2]) );
}

// object����PacketData���쐬���A����packet�ɒǉ�����B
void AddObjectPacket( sceGifPacket *packet, WorkData *wd, int i )
{
	static sceVu0FVECTOR trans = { 0.f, 0.f, 0.f, 0.f };
	static sceVu0FMATRIX LocalWorld;
	static sceVu0FMATRIX LocalScreen;
	static sceVu0IVECTOR vi[8];	// 4�����̃x�N�g�� (int)
	static sceVu0IVECTOR ci[6];	// 4�����̃x�N�g�� (int)

	trans[0] = wd->xyz.x[i%NX];
	trans[1] = wd->xyz.y[(i%(NX*NY))/NX];
	trans[2] = wd->xyz.z[i/(NX*NY)];
	sceVu0TransMatrix( LocalWorld, wd->Rotate, trans );
	sceVu0MulMatrix( LocalScreen, wd->WorldScreen, LocalWorld );
// LocalScreen��world*view*screen�̈ꊇ�ϊ��s��

// ���_�𓧎��ϊ�
	sceVu0RotTransPersN( vi, LocalScreen, wd->vf, bulkof(vi), 1 );
	LightRGB( ci, wd, bulkof(ci) );

// GS��Packetmode REGLIST mode �� �R�}���h��ǉ�
	SetPacket( packet, ci[3], ci[2], ci[1], FOG,
		vi[0], vi[1], vi[3], vi[2], vi[4], vi[6], vi[7], vi[1] );
	SetPacket( packet, ci[5], ci[0], ci[4], FOG,
		vi[5], vi[4], vi[6], vi[7], vi[1], vi[3], vi[2], vi[4] );
}

// PadAction�Ŏg���ϐ�
int objects = NX*NY;	// �\��object��
int autorotate = 1;	// ������]�t���O

// pad�������ꂽ���̓�����w��
int PadAction( u_int button, WorkData *wd )
{
	static int command = 0;		// key check flag
	static int select = 0;
	if ( button == 0 ) {
		if ( command ) command = 0;
		if ( select ) select = 0;
		return 0;
	}
	if ( button & SCE_PADselect ) {	// init key
		if ( button & SCE_PADR1 )
			wd->xyzl[3] = 0.f;
		if ( button & SCE_PADR2 )
			wd->xyzl[3] = OBJL;
		if ( button & SCE_PADLright || button & SCE_PADLleft )
			wd->xyzl[0] = 0.f;
		if ( button & SCE_PADLdown || button & SCE_PADLup )
			wd->xyzl[1] = 0.f;
		if ( button & SCE_PADL1 || button & SCE_PADL2 )
			wd->xyzl[2] = 0.f;
		if ( button & SCE_PADRright || button & SCE_PADRdown )
			objects = NX*NY;
		if ( button & SCE_PADstart ) {
			wd->xyzl[0] = 0.f;
			wd->xyzl[1] = 0.f;
			wd->xyzl[2] = 0.f;
			wd->xyzl[3] = OBJL;
			command = 1;
		}
		if ( !select ) {
			printf( "objects = %d\n", objects );
			select = 1;
		}
	}
	else if ( command )
		return 0;
	else {
		if ( button & SCE_PADRright && objects < OBJECTS )
			objects += NX;
		if ( button & SCE_PADRdown && 0 < objects )
			objects -= NX;
		if ( button & SCE_PADstart ) {
			autorotate = !autorotate;
			command = 1;
			return 0;
		}
		if ( button & SCE_PADLright ) wd->xyzl[0] += 30.f;
		if ( button & SCE_PADLleft  ) wd->xyzl[0] -= 30.f;
		if ( button & SCE_PADLdown  ) wd->xyzl[1] += 30.f;
		if ( button & SCE_PADLup    ) wd->xyzl[1] -= 30.f;
		if ( button & SCE_PADL2     ) wd->xyzl[2] += 30.f;
		if ( button & SCE_PADL1     ) wd->xyzl[2] -= 30.f;
		if ( button & SCE_PADR2     ) wd->xyzl[3] += 6.f;
		if ( button & SCE_PADR1     ) wd->xyzl[3] -= 6.f;
		if ( button & SCE_PADRup    ) {
			int i;
			for ( i = 0; i < bulkof(wd->vf); i++ )
				sceVu0ScaleVectorXYZ( wd->vf[i], wd->vf[i],
								101.f/100.f );
		}
		if ( button & SCE_PADRleft  ) {
			int i;
			for ( i = 0; i < bulkof(wd->vf); i++ )
				sceVu0ScaleVectorXYZ( wd->vf[i], wd->vf[i],
								100.f/101.f );
		}
	}
	return 1;
}

// SourceChannel�̓]���I����҂��A���ɑ��M���悤�Ƃ��Ă���
// SourceData��RingBuffer�������鎖���o����܂ő҂���Kick�B
void KickSourceChannel( u_int sadr, u_int qwc )
{
	static u_int tadr, madr, remain;
	while ( *D8_CHCR & D_CHCR_STR_M );// DMA8 polling
	*D8_SADR = sadr;
	*D8_QWC = qwc;
	qwc <<= 4;
	do {
	// ����EE#1�o�O�΍�BEE#2�ȍ~�ł�"tadr = *D2_TADR"�݂̂ŉB
		while ( (tadr = *D2_TADR) == 0 );
		madr = *D8_MADR;
	// ��RingBuffer�̋󂫗e�ʂ����߂�B
		remain = (tadr==madr) ? MFIFO_SIZE
			: (tadr+MFIFO_SIZE-madr)&(MFIFO_SIZE-0x10);
	} while ( remain <= qwc );// (qwc+1)byte�ȏ�󂭂̂�҂�
	((tD_CHCR*)D8_CHCR)->STR = 1;
}

int main()
{
	int i, obj, spr, vx, px = 0, fr = 0;
	u_int paddata;
	u_char rdata[32];
	const u_long giftag[2] = {
		SCE_GIF_SET_TAG( 0, 1, 0, 0, 1, 12 ),
		0x0000551551555510L
	};
	static char mfifo_base[MFIFO_SIZE] __attribute__ ((section(".mfifo")));
	static u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));
	ScratchPadRam *SPR = (ScratchPadRam*)0x70000000;
	WorkData *wd = &SPR->wd;
	sceGsDBuff *db = &SPR->db;	// for GS Double Buffer

	sceGifPacket Packet[2];		// for SPR Packet Double Buffer
	sceGifPacket *packet;		// for SPR Packet Double Buffer Change

	sceVu0FVECTOR rot;	// ��]�p
	sceVu0FMATRIX Unit;	// �P�ʍs��p


	// �g�p���郂�W���[���̃��[�h
	sceSifInitRpc(0);
	while ( sceSifLoadModule( "host0:/usr/local/sce/iop/modules/sio2man.irx",
			0, NULL ) < 0 ) {
		printf( "Can't load module sio2man\n" );
	}
	while ( sceSifLoadModule( "host0:/usr/local/sce/iop/modules/padman.irx",
			0, NULL ) < 0 ) {
		printf( "Can't load module padman\n" );
	}

// SPR�Ŏg�p����\��̃T�C�Y���`�F�b�N����
	printf( "SPR size = 0x%04X\n", sizeof(ScratchPadRam) );
	if ( 0x4000 < sizeof(ScratchPadRam) ) {
		printf( "out of scratch pad ram\n" );
		for ( ; ; );
	}

// Packet�f�[�^�̈��SPR�ɐݒ�
	sceGifPkInit( &Packet[0], SPR->pBase[0] );
	sceGifPkInit( &Packet[1], SPR->pBase[1] );

	InitWorkData( wd );
	sceVu0UnitMatrix( Unit );

	sceGsResetPath();
	sceDmaReset( 1 );
	sceGsResetGraph( 0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME );
	sceGsSetDefDBuff( db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT,
				SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR );
	FlushCache( 0 );// SetDefDBuff�̓��e���������ɃA�b�v�f�[�g����

	*D_RBOR = (u_int)mfifo_base;// Memory FIFO Address��ݒ�
	*D_RBSR = MFIFO_SIZE-0x10;// ��0x00007FF0�̂悤�ɂ���I
	printf( "RBOR, RBSR = 0x%08X, 0x%08X\n", *D_RBOR, *D_RBSR );

	scePadInit( 0 );
	scePadPortOpen( 0, 0, pad_dma_buf );
	vx = !sceGsSyncV( 0 );


	for ( ; ; ) {
		sceGsSetHalfOffset( (fr?&db->draw1:&db->draw0),2048,2048,vx );

		if ( 0 < scePadRead( 0, 0, rdata ) ) {
			paddata = 0xffff ^ ( rdata[2] << 8 | rdata[3] );
		}
		else {
			paddata = 0;
		}

		if ( PadAction( paddata, wd ) ) {
			SetTransData( wd );
		}

		if ( autorotate ) {	// ��葬�x�ŉ�]
			rot[0] += DELTA; if ( PI < rot[0] ) rot[0] -= 2.f*PI;
			rot[1] += DELTA; if ( PI < rot[1] ) rot[1] -= 2.f*PI;
			rot[2] += DELTA; if ( PI < rot[2] ) rot[2] -= 2.f*PI;
		}
		sceVu0RotMatrix( wd->Rotate, Unit, rot );
		sceVu0MulMatrix( wd->LocalLight, wd->NormalLight, wd->Rotate );

	// �����̏��Ԃ��d�v
		sceGsSyncPath(0,0);	// MFIFO���I���̂�҂���
		((tD_CTRL*)D_CTRL)->MFD = 0;// SwapDBuff�ׂ̈�MFIFO������
		sceGsSwapDBuff( db, fr );// �_�u���o�b�t�@�̐ؑւ�
		while ( *D2_CHCR & D_CHCR_STR_M );// DMA2 polling
		((tD_CTRL*)D_CTRL)->MFD = 3;// Memory FIFO �ݒ�

		*D8_MADR = (u_int)mfifo_base;
		*D2_TADR = (u_int)mfifo_base;
	// ����sceGsSwapDBuff�ɂ���Ĕj�󂳂��̂ŕK���Đݒ肷��
	//	D2_CHCR��STR bit��ENDtag��������Ɨ�����
		*D2_CHCR = 0x0104;// MFIFO�J�n (TTE=0,MOD=1,STR=1)

		obj = objects;			// �c��object��
		while ( obj ) {
			spr = (SPROBJ<=obj?SPROBJ:obj);
			packet = &Packet[px];
			sceGifPkReset( packet );
			sceGifPkCnt( packet, 0, 0, 0 );
			sceGifPkOpenGifTag( packet, *(u_long128*)&giftag );
			for ( i = objects-obj; i < objects-obj+spr; i++ )
				AddObjectPacket( packet, wd, i );
			sceGifPkCloseGifTag( packet );
			sceGifPkTerminate( packet );
			KickSourceChannel( (u_int)packet->pBase&0x00003FF0,
						sceGifPkSize( packet ) );
			px ^= 1;
			obj -= spr;
		}
		packet = &Packet[px];
		sceGifPkReset( packet );// Packet �̃��Z�b�g
		sceGifPkEnd( packet, 0, 0, 0 );// END Tag ���p�P�b�g�ɒǉ�
		sceGifPkTerminate( packet );// Packet �̏I��
		KickSourceChannel( (u_int)packet->pBase&0x00003FF0,
					sceGifPkSize( packet ) );
		px ^= 1;
		vx = !sceGsSyncV( 0 );
		fr ^= 1;
	}
}

