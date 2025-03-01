/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *               Emotion Engine Library Sample Program
 *
 *                           - <thread> -
 *
 *                          Version <1.00>
 *                               SJIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                            <thread.c>
 *                 <main function of multi thread>
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      1.00            Jun,9,1999      akiyuki     first version
 *      1.10            dec,6,1999      akiyuki     max threads change.
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
#include <libvifpk.h>
#include <libpad.h>
#include <sifdev.h>
#include <sifrpc.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 224
#define PI (3.141592f)
#define SCE_PADreset	(SCE_PADL1|SCE_PADL2|SCE_PADR1|SCE_PADR2)
#define bulkof(a)	(sizeof(a)/sizeof(a[0]))

// �����̂̒��_���W���Z�b�g
#define	VX	80.f
#define	VY	80.f
#define	VZ	80.f
#define	VW	1.f

// �����̂̊e�ʂ̐F���Z�b�g
#define	CX	255.f
#define	CY	255.f
#define	CZ	255.f
#define	CW	0.f

// THREADS �̍ő�l�� 256-main-sync-point-dummy
#define	THREADS		(252)		// thread�ő吔
#define	SYNC_PRIO	(1)		// SyncThread priority
#define	ROTATE_PRIO	(2)		// rotate_thread priority
#define	NL		(210.f)		// object�Ԃ̋���
#define	NZ		(3000.f)	// object��Z�ʒu
#define	CAMZ		(0.f)		// camera��Z�ʒu
#define	NX		(18)		// object�\����(x)
#define	NY		(14)		// object�\����(y)
#define	STACKSIZE	(4096)		// stack size
#define	PKTSIZE		(2+THREADS*(26/2)+1)// qword
#define	DELTA		(1.f*PI/180.f)	// ��]�p�P��
#define	FOG		(10.f)		// �t�H�O

#define	NV		(8)		// object���_��
#define	NC		(6)		// object�ʐ�

struct RotateData {
	char		stack[STACKSIZE] __attribute__ ((aligned(16)));
	u_int		*pad;		// �p�b�h���͒l
	sceVif1Packet	*packet;	// �o�̓p�P�b�g�ւ̃|�C���^
	sceVu0FMATRIX	*WorldView;	// �J�������_
	sceVu0FMATRIX	*ViewScreen;	// �����ϊ��}�g���N�X
	sceVu0FMATRIX	*NormalLight;	// ���C�g�ʒu
	sceVu0FMATRIX	*LightColor;	// ���C�g�J���[
	sceVu0FVECTOR	trans;		// �����ʒu
	sceVu0FVECTOR	rot;		// ������]�p
	int		id;		// thread id
	int		no;		// thread no
	u_int		autorotate : 1;	// ������]�t���O
	u_int		command : 1;	// key check flag
};

int PacketSemaphore;	// packet write semaphore id
int RotateSignal;	// object thread rotate semaphore id
int KickSignal;		// DMA send semaphore id

// �@������邽�߂̊֐�
void MakeNormal( sceVu0FVECTOR o, sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2 )
{
	sceVu0FVECTOR w1, w2;
	sceVu0SubVector( w1, v1, v0 );
	sceVu0SubVector( w2, v2, v0 );
	sceVu0OuterProduct( o, w1, w2 );
	sceVu0Normalize( o, o );
}

// �����v�Z���������Ƃ�RGB�l
void LightRGB( sceVu0IVECTOR *ci, sceVu0FMATRIX LocalLight, sceVu0FMATRIX LightColor,
	sceVu0FVECTOR *normal, sceVu0FVECTOR *cf, int n )
{
	sceVu0FVECTOR vector;
	sceVu0IVECTOR *endp = ci+n;
	while ( ci < endp ) {
		sceVu0ApplyMatrix( vector, LocalLight, *normal++ );
		sceVu0ClampVector( vector, vector, 0.0f, 1.0f );
		vector[3] = 1.0f;
		sceVu0ApplyMatrix( vector, LightColor, vector );
		sceVu0MulVector( vector, *cf++, vector );
		sceVu0FTOI0Vector( *ci++, vector );
	}
}

const u_long giftag[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 1, 12), 0x0000551551555510L };

// �����ƒ��_����GS�ɓn��packet���쐬(TriangleFan)
void AddObjectPacket( sceVif1Packet *packet,
	sceVu0IVECTOR c0,sceVu0IVECTOR c1,sceVu0IVECTOR c2,float fog,
	sceVu0IVECTOR v0,sceVu0IVECTOR v1,sceVu0IVECTOR v2,sceVu0IVECTOR v3,
	sceVu0IVECTOR v4,sceVu0IVECTOR v5,sceVu0IVECTOR v6,sceVu0IVECTOR v7 )
{
	sceVif1PkAddGsData( packet, SCE_GS_SET_PRIM(0x05,0,0,0,0,0,0,0,0) );
	sceVif1PkAddGsData( packet, SCE_GS_SET_RGBAQ(c0[0],c0[1],c0[2],0x00,*(u_int*)&fog) );
	sceVif1PkAddGsData( packet, SCE_GS_SET_XYZ(v0[0],v0[1],v0[2]) );
	sceVif1PkAddGsData( packet, SCE_GS_SET_XYZ(v1[0],v1[1],v1[2]) );
	sceVif1PkAddGsData( packet, SCE_GS_SET_XYZ(v2[0],v2[1],v2[2]) );
	sceVif1PkAddGsData( packet, SCE_GS_SET_XYZ(v3[0],v3[1],v3[2]) );
	sceVif1PkAddGsData( packet, SCE_GS_SET_RGBAQ(c1[0],c1[1],c1[2],0x00,*(u_int*)&fog) );
	sceVif1PkAddGsData( packet, SCE_GS_SET_XYZ(v4[0],v4[1],v4[2]) );
	sceVif1PkAddGsData( packet, SCE_GS_SET_XYZ(v5[0],v5[1],v5[2]) );
	sceVif1PkAddGsData( packet, SCE_GS_SET_RGBAQ(c2[0],c2[1],c2[2],0x00,*(u_int*)&fog) );
	sceVif1PkAddGsData( packet, SCE_GS_SET_XYZ(v6[0],v6[1],v6[2]) );
	sceVif1PkAddGsData( packet, SCE_GS_SET_XYZ(v7[0],v7[1],v7[2]) );
}

// pad�������ꂽ���̓�����w��
void PadAction( struct RotateData *rd )
{
	if ( *rd->pad & SCE_PADselect ) {			// init key
		if ( *rd->pad & SCE_PADR1 ) {
			rd->trans[0] = rd->trans[1] = 0.f;
		}
		if ( *rd->pad & SCE_PADR2 ) {
			rd->trans[0] = NL*(rd->no%NX)-NL*(NX-1)/2.f;
			rd->trans[1] = NL*(rd->no/NX)-NL*(NY-1)/2.f;
		}
		if ( *rd->pad & SCE_PADLright || *rd->pad & SCE_PADLleft ) {
			rd->trans[0] = NL*(rd->no%NX)-NL*(NX-1)/2.f;
		}
		if ( *rd->pad & SCE_PADLdown || *rd->pad & SCE_PADLup ) {
			rd->trans[1] = NL*(rd->no/NX)-NL*(NY-1)/2.f;
		}
		if ( *rd->pad & SCE_PADL1 || *rd->pad & SCE_PADL2 ) {
			rd->trans[2] = NZ;
		}
		if ( *rd->pad & SCE_PADRleft ) {
			rd->trans[0] = NL*(rd->no%NX)-NL*(NX-1)/2.f;// x �ʒu
			rd->trans[1] = NL*(rd->no/NX)-NL*(NY-1)/2.f;// y �ʒu
			rd->trans[2] = NZ;
			*(u_long128*)rd->rot = 0;
			rd->command = 1;
			return;
		}
		if ( *rd->pad & SCE_PADRup ) {
			rd->trans[0] = NL*(rd->no%NX)-NL*(NX-1)/2.f;// x �ʒu
			rd->trans[1] = NL*(rd->no/NX)-NL*(NY-1)/2.f;// y �ʒu
			rd->trans[2] = NZ;
			rd->rot[0] = rd->rot[1] = rd->rot[2] = (2.f*PI*rand()/RAND_MAX)-PI;
			rd->command = 1;
		}
		if ( *rd->pad & SCE_PADstart ) {
			rd->trans[0] = rd->trans[1] = 0.f;
			rd->trans[2] = CAMZ+350.f;
			rd->rot[0] = DELTA*2*rd->no; if ( PI < rd->rot[0] ) rd->rot[0] -= 2.f*PI;
			rd->rot[1] = rd->rot[2] = rd->rot[0];
			rd->command = 1;
		}
	}
	else if ( rd->command ) {
		if ( !*rd->pad ) {
			rd->command = 0;
		}
	}
	else {
		if ( *rd->pad & SCE_PADstart) {
			rd->autorotate = !rd->autorotate;
			rd->command = 1;
		}
		if ( *rd->pad & SCE_PADRleft ) {
			*(u_long128*)rd->rot = 0;
			return;
		}
		if ( *rd->pad & SCE_PADRup ) {
			rd->rot[0] = rd->rot[1] = rd->rot[2] = (2.f*PI*rand()/RAND_MAX)-PI;
		}
		if ( *rd->pad & SCE_PADR2 ) {
			rd->trans[0] += ((rd->no%NX)-(NX-1)/2.f)*4.f;
			rd->trans[1] += ((rd->no/NX)-(NY-1)/2.f)*4.f;
		}
		if ( *rd->pad & SCE_PADR1 ) {
			rd->trans[0] -= ((rd->no%NX)-(NX-1)/2.f)*4.f;
			rd->trans[1] -= ((rd->no/NX)-(NY-1)/2.f)*4.f;
		}
		if ( *rd->pad & SCE_PADLright ) rd->trans[0] += 20.f;
		if ( *rd->pad & SCE_PADLleft  ) rd->trans[0] -= 20.f;
		if ( *rd->pad & SCE_PADLdown  ) rd->trans[1] += 20.f;
		if ( *rd->pad & SCE_PADLup    ) rd->trans[1] -= 20.f;
		if ( *rd->pad & SCE_PADL2     ) rd->trans[2] += 20.f;
		if ( *rd->pad & SCE_PADL1     ) rd->trans[2] -= 20.f;
	}
// ��葬�x�ŉ�]
	if ( rd->autorotate ) {
		rd->rot[0] += DELTA; if ( PI < rd->rot[0] ) rd->rot[0] -= 2.f*PI;
		rd->rot[1] += DELTA; if ( PI < rd->rot[1] ) rd->rot[1] -= 2.f*PI;
		rd->rot[2] += DELTA; if ( PI < rd->rot[2] ) rd->rot[2] -= 2.f*PI;
	}
}

/*
	�`��object thread�̋�؂�ƂȂ�thread
	���䂪����Ă�������packet��object��ǉ����I��������ɂȂ�A
	dma���M�\�ł��邱�Ƃ�ʒm����B
*/
void PointThread( void *data )
{
	for ( ; ; ) {
		WaitSema( PacketSemaphore );
		SignalSema( PacketSemaphore );
		SignalSema( KickSignal );
		SignalSema( RotateSignal );
	}
}

// �����œn���ꂽ�s��̏��������s��
void InitCubeData( sceVu0FVECTOR *vf, sceVu0FVECTOR *cf, sceVu0FVECTOR *normal )
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
		{ CX, CY, CZ, CW },
		{ CX, CY, CZ, CW },
		{ CX, CY, CZ, CW },
		{ CX, CY, CZ, CW },
		{ CX, CY, CZ, CW },
		{ CX, CY, CZ, CW },
	};
	memcpy( vf, _vf, sizeof(_vf) );
	memcpy( cf, _cf, sizeof(_cf) );
	MakeNormal( normal[0], vf[1], vf[3], vf[7] );
	MakeNormal( normal[1], vf[0], vf[1], vf[6] );
	MakeNormal( normal[2], vf[2], vf[0], vf[4] );
	MakeNormal( normal[3], vf[0], vf[2], vf[1] );
	MakeNormal( normal[4], vf[2], vf[4], vf[3] );
	MakeNormal( normal[5], vf[4], vf[6], vf[5] );
}

// object thread
void CubeObject( void *data )
{
	struct RotateData *rd = (struct RotateData*)data;

	sceVu0FMATRIX WorldScreen;
	sceVu0FMATRIX LocalWorld;
	sceVu0FMATRIX LocalScreen;
	sceVu0FMATRIX LocalLight;
	sceVu0FMATRIX work;	// ��Ɨp�s��
	sceVu0FMATRIX unit;	// �P�ʍs��p
	sceVu0FVECTOR vf[NV];	// object�e���_
	sceVu0FVECTOR cf[NC];	// object�e�ʂ̐F
	sceVu0FVECTOR normal[NC];// object�e�ʂ̖@��
	sceVu0IVECTOR vi[NV];
	sceVu0IVECTOR ci[NC];

	InitCubeData( vf, cf, normal );
	sceVu0UnitMatrix( unit );// �P�ʍs��쐬
	sceVu0MulMatrix( WorldScreen, *rd->ViewScreen, *rd->WorldView );


	for ( ; ; ) {
		WaitSema( PacketSemaphore );
		PadAction( rd );

	// �I�u�W�F�N�g�̈ʒu(trans)�Ɖ�](rot)����}�g���N�X�𐶐�
		sceVu0RotMatrix( work, unit, rd->rot );		// �P�ʍs���rot����]
		sceVu0TransMatrix( LocalWorld, work, rd->trans );// local -> world
		sceVu0MulMatrix( LocalLight, *rd->NormalLight, LocalWorld );
		sceVu0MulMatrix( LocalScreen, WorldScreen, LocalWorld );
	// LocalScreen��world*view*screen�̈ꊇ�ϊ��s��

	// ���_�𓧎��ϊ�
		sceVu0RotTransPersN( vi, LocalScreen, vf, NV, 1 );
		LightRGB( ci, LocalLight, *rd->LightColor, normal, cf, NC );

	// GS��Packetmode REGLIST mode �� �R�}���h��ǉ�
		AddObjectPacket( rd->packet, ci[3], ci[2], ci[1], FOG,
			vi[0], vi[1], vi[3], vi[2], vi[4], vi[6], vi[7], vi[1] );
		AddObjectPacket( rd->packet, ci[5], ci[0], ci[4], FOG,
			vi[5], vi[4], vi[6], vi[7], vi[1], vi[3], vi[2], vi[4] );

		SignalSema( PacketSemaphore );
		SignalSema( RotateSignal );
	}
	ExitThread();
}

// �����œn���ꂽ�s��̏��������s��
void InitMatrix( sceVu0FMATRIX WorldView, sceVu0FMATRIX ViewScreen,
	sceVu0FMATRIX NormalLight, sceVu0FMATRIX LightColor )
{
// �J�����̈ʒu�ƌ�����y���ݒ�
	sceVu0FVECTOR camera_p  = { 0.0f, 0.0f, CAMZ, 1.0f };
	sceVu0FVECTOR camera_zd = { 0.0f, 0.0f, 1.0f, 1.0f };
	sceVu0FVECTOR camera_yd = { 0.0f, 1.0f, 0.0f, 1.0f };
// �O�̃��C�g���ꂼ��̐F
	sceVu0FVECTOR color0 = { 0.8f, 0.3f, 0.3f, 1.0f };
	sceVu0FVECTOR color1 = { 0.3f, 0.8f, 0.3f, 1.0f };
	sceVu0FVECTOR color2 = { 0.3f, 0.3f, 0.8f, 1.0f };
// ���F
	sceVu0FVECTOR ambient = { 0.1f, 0.1f, 0.1f, 0.0f };
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
	sceVu0NormalLightMatrix( NormalLight, light0, light1, light2 );
	sceVu0LightColorMatrix( LightColor, color0, color1, color2, ambient );
// �J�������_ ( world -> view )
	sceVu0CameraMatrix( WorldView, camera_p, camera_zd, camera_yd );
// �����ϊ��}�g���N�X�𐶐� ( view -> screen )
	sceVu0ViewScreenMatrix( ViewScreen, 512.0f, 1.0f, 0.47f, 2048.0f, 2048.0f, 1.0f, 16777215.0f, 64.0f, 65536.0f ); // view -> screen
}

int ThreadStart( void *thread, struct RotateData *rd )
{
	static struct ThreadParam tp;
	tp.entry = thread;
	tp.stack = rd->stack;
	tp.stackSize = sizeof(rd->stack);
	tp.initPriority = ROTATE_PRIO;
	tp.gpReg = &_gp;
	tp.option = (u_int)rd;
	rd->id = CreateThread( &tp );
	if ( rd->id < 0 ) {
		return rd->id;
	}
	return StartThread( rd->id, (void*)rd );
}

int ThreadStop( int id )
{
	if ( TerminateThread( id ) != id ) {
		return -1;
	}
	return DeleteThread( id );
}

/*
	int StartObjects( ... );
	����: object�ɓn���\���̂���э\���̂ɃZ�b�g����f�[�^
	�ߒl: point thread��ID
	1. struct RotateData�Ƀf�[�^���Z�b�g
	2. ���̃f�[�^�������Ƃ���ThreadStart���Ăяo��
	3. point thread���J�n
*/
int StartObjects( struct RotateData *rd, int sz_rd, int num, u_int *pad,
	sceVif1Packet *packet, sceVu0FMATRIX *WorldView, sceVu0FMATRIX *ViewScreen,
	sceVu0FMATRIX *NormalLight, sceVu0FMATRIX *LightColor )
{
	int i, id, n = 0;
	static struct ThreadParam pt;
	static char stack[STACKSIZE] __attribute__ ((aligned(16)));
	// start PointThread
	pt.entry = (void*)PointThread;
	pt.stack = stack;
	pt.stackSize = sizeof(stack);
	pt.initPriority = ROTATE_PRIO;
	pt.gpReg = &_gp;
	id = CreateThread( &pt );
	if ( id < 0 ) {
		return 0;
	}
	StartThread( id, NULL );
// set objects data
	rd[0].autorotate = 1;
	rd[0].pad = pad;
	rd[0].packet = packet;
	rd[0].WorldView = WorldView;
	rd[0].ViewScreen = ViewScreen;
	rd[0].NormalLight = NormalLight;
	rd[0].LightColor = LightColor;
	rd[0].trans[2] = NZ;				// z
	rd[0].trans[3] = 0.f;				// w
	for ( i = 0; i < sz_rd; i++ ) {
		rd[i] = rd[0];// ��L�f�[�^�ŏ�����
		rd[i].no = i;
		rd[i].trans[0] = NL*(i%NX)-NL*(NX-1)/2.f;// x �ʒu
		rd[i].trans[1] = NL*(i/NX)-NL*(NY-1)/2.f;// y
		rd[i].rot[0] = rd[i].rot[1] = rd[i].rot[2] = (2.f*PI*rand()/RAND_MAX)-PI;
	}
	// start objects
	for ( i = 0; i < num; i++ ) {
		if ( 0 <= ThreadStart( (void*)CubeObject, &rd[i] ) ) {
			n++;
		}
	}
	return n;
}

// SyncV�ɓ������čs����Ɨp��thread
void SyncThread( void *data )
{
	int vx, fr = 0, num;
	u_int paddata;
	u_char rdata[32];
	sceVif1Packet packet;
	static u_long128 packet_base[PKTSIZE] __attribute__ ((aligned(16)));
	sceGsDBuff db;
	sceDmaChan *dmaVif;
	sceVu0FMATRIX WorldView;
	sceVu0FMATRIX ViewScreen;
	sceVu0FMATRIX NormalLight;
	sceVu0FMATRIX LightColor;
	static struct RotateData rd[THREADS];


	InitMatrix( WorldView, ViewScreen, NormalLight, LightColor );
	num = StartObjects( rd, bulkof(rd), THREADS, &paddata, &packet,
		&WorldView, &ViewScreen, &NormalLight, &LightColor );
	sceGsResetPath();
	sceDmaReset( 1 );
	sceVif1PkInit( &packet, packet_base );
	dmaVif = sceDmaGetChan( SCE_DMA_VIF1 );
	dmaVif->chcr.TTE = 1;			// DMAtag���]������
	sceGsResetGraph( 0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME );// �`��G���W���̏�����
	sceGsSetDefDBuff( &db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR );// �_�u���o�b�t�@�̐ݒ�
	FlushCache( 0 );		// SetDefDBuff �̓��e���������ɃA�b�v�f�[�g����

	vx = !sceGsSyncV( 0 );


	for ( ; ; ) {
		sceGsSetHalfOffset( (fr?&db.draw1:&db.draw0), 2048, 2048, vx );
		// ����sceGsSwapDBuff�̑O�ɕK�v
		SyncDCache( &db, (void*)&db+sizeof(db) );

		if ( 0 < scePadRead( 0, 0, rdata ) ) {
			paddata = 0xffff ^ ( rdata[2] << 8 | rdata[3] );
		}
		else {
			paddata = 0;
		}

		if ( paddata & SCE_PADRright && num < THREADS
		  && 0 <= ThreadStart( (void*)CubeObject, &rd[num] ) ) {
			num++;
		}
		if ( paddata & SCE_PADRdown && 0 < num
		  && 0 <= ThreadStop( rd[num-1].id ) ) {
			num--;
		}

		sceGsSyncPath( 0, 0 );
		sceGsSwapDBuff( &db, fr );	// �_�u���o�b�t�@�̐ؑւ�
	// �p�P�b�g������
		sceVif1PkReset( &packet );	// Packet �̃��Z�b�g
		sceVif1PkCnt( &packet, 0 );	// CNT Tag��Packet�ɒǉ�
		sceVif1PkOpenDirectCode( &packet, 0 );// �_�C���N�g�R�}���h�̒ǉ�
		sceVif1PkOpenGifTag( &packet, *(u_long128*)&giftag );// GIF Tag �̒ǉ�
		SignalSema( PacketSemaphore );
	// �p�P�b�g���M�҂�
		WaitSema( KickSignal );
	// �p�P�b�g�㏈��
		WaitSema( PacketSemaphore );
		sceVif1PkCloseGifTag( &packet );// GIF Tag���N���[�Y
		sceVif1PkCloseDirectCode( &packet );// �_�C���N�g�R�}���h�� Num �Ǘ�����߂�
		sceVif1PkEnd( &packet, 0 );	// END Tag ���p�P�b�g�ɒǉ�
		sceVif1PkTerminate( &packet );	// Packet �̏I��
		FlushCache( 0 );
		sceDmaSend( dmaVif, packet.pBase );// DMA Start
		vx = !sceGsSyncV( 0 );
		fr ^= 1;
	}
}

int main()
{
	int id;
	struct SemaParam s;
	struct ThreadParam st;
	static char stack[STACKSIZE] __attribute__ ((aligned(16)));
	static u_long128 pad_dma_buf[scePadDmaBufferMax] __attribute__ ((aligned(64)));

	sceSifInitRpc(0);
	while ( sceSifLoadModule( "host0:/usr/local/sce/iop/modules/sio2man.irx", 0, NULL ) < 0 ) {
		printf( "Can't load module sio2man\n" );
	}
	while ( sceSifLoadModule( "host0:/usr/local/sce/iop/modules/padman.irx", 0, NULL ) < 0 ) {
		printf( "Can't load module padman\n" );
	}
	scePadInit( 0 );
	scePadPortOpen( 0, 0, pad_dma_buf );

	s.maxCount = 1;
	s.initCount = 0;
	RotateSignal = CreateSema( &s );
	s.maxCount = 1;
	s.initCount = 0;
	PacketSemaphore = CreateSema( &s );
	s.maxCount = 1;
	s.initCount = 0;
	KickSignal = CreateSema( &s );


	st.entry = (void*)SyncThread;
	st.stack = stack;
	st.stackSize = sizeof(stack);
	st.initPriority = SYNC_PRIO;
	st.gpReg = &_gp;
	id = CreateThread( &st );
	StartThread( id, NULL );

	
	for ( ; ; ) {
		WaitSema( RotateSignal );
		RotateThreadReadyQueue( ROTATE_PRIO );
	}
	return 0;
}

