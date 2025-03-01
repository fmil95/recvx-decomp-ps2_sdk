/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - tdiff -
 *
 *                         Version 0.10 
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            tdiff.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *	0.1		Jul,13,1998	suzu	    1st version
 *	0.2		Dec,23,1999	suzu	    changed for libgraph
 *	0.3		May,13,1999	suzu	    add comment
 */
// ����́A����ȃI�u�W�F�N�g�̃f�B�X�v���C���X�g���쐬����T���v���ł��B
// �ʏ�̃I�u�W�F�N�g�̓I�u�W�F�N�g���\������|���S���ɂ͓����}�g���N
// �X���K�p����܂��B���̃}�g���N�X���|���S�����Ƃɂ���͈͓��Ńf�B�X
// �g�[�V������^���邱�ƂŁA���⑽���̔j�Ђ��^���I�ɕ\�����邱�Ƃ���
// ���܂��B
//
// �������W�n�̊�{�I�ȓ���Ɋւ��ẮAtballs.c ���Q�Ɖ������B
//
// �T���v���ɂ͈ȉ��̃��[�J���w�b�_�t�@�C�� object.h ���K�v�ł��B
//	

#include "object.h"

// 
// 	Screen information
// 
// �X�N���[���܂ł̋����A����уA�X�y�N�g���ݒ肵�܂��B
// 640x240 �̃C���^�[���[�X�_�u���o�b�t�@�ŕ\������ꍇ�A�s�N�Z���̌`
// �� 1:0.5 �̒����`�ɂȂ�܂��B
//
SCREEN screen = {
	1024,			// distance to screen
	1.0,	0.5,		// aspect ratio
};

// 
// 	Lighting information
// 
// �����i�R�����j�̈ʒu�A�����A�����̉e���A����ъ��F��ݒ肵�܂��B
// �����ł͌����̕����͌����̈ʒu���玩���v�Z�����̂Œl��ݒ肷��K
// �v�͂���܂���B�����ł� light-0 �݂̂��g�p���܂��B
//
LIGHT light = {
	1,						// light mode
// 	light-0		light-1		light-2
	{ 0,   0,-40,1},{  0,  0, 40,1},{-40, 40, 40,1},// light source position
	{ 1,   1,  1,0},{  0,  0, -1,0},{  1, -1, -1,0},// light direction 
	{0.8,0.2,0.2,0},{0.2,0.8,0.0,2},{0.0,0.0,0.0,0},// light influence
	{0.2,0.2,0.2,0},				// ambient
};

// 
// 	Camera position information
// 
// �J�����ʒu���w�肵�܂��B�J�����͉��������ɗ��Ă��A��Ɍ��_������
// �悤�ɏC������܂��B���Ȃ킿 view.zd = -vies.p �ƂȂ�悤�ɃJ����
// �̌������ݒ肳��܂��B
//
VIEW view = {
	{    0,   0,  -1, 0},		// foot direction
	{    1,  -1,  -1, 0},		// eye direction 
	{ -600,  50,  50, 1},		// standing position
};

// Your moving speed
// �J�����̈ړ����x���w�肵�܂��B���̒l���t���[������ view.p �����Z
// ����܂��B 
static VECTOR	v = {1.0, 0, 0, 0};	// your speed

//
// �p�[�e�B�N���̃p�����[�^���w�肵�܂��B
//
#define C	240	// Color
#define S	  5	// Size
#define O	  6	// Origin

// special object drivers
extern void objDrvParticleAttributes(VECTOR r, int n, int s);
extern qword *objDrvParticle(qword *pbase, OBJECT *obj);

static void init_cube(OBJECT *obj)
{
	// Color, Vertex, Normal, (Texture)
	static VECTOR	cvnt[] = {
	{   0,   0,   0,4},
	{   C,   C,   C,0},{  C,   C,   C,0},{   C,   C,   C,0},{  C,   C,   C,0},
	{-S+O, S+O, S+O,0},{S+O, S+O, S+O,0},{-S+O,-S+O, S+O,0},{S+O,-S+O, S+O,0}, 
	{   0,   0,   1,0},{  0,   0,   1,0},{   0,   0,   1,0},{  0,   0,   1,0},
	{   0,   0,   0,0},{  0,   0,   0,0},{   0,   0,   0,0},{  0,   0,   0,0},
	};

	// setup member
	memset(obj, 0, sizeof(OBJECT));
	objMakeTag(obj, 0, 0, 0, 0);	// GIFtag
	obj->spec    = 0;		// specular	
	obj->shadow  = 0;		// shadow to z-plane
	obj->rate    = 0.0;		// morphing rate
	obj->clip    = 1;		// polygon clip
	obj->box     = 0;		// rounding box
	obj->cvnt    = cvnt;		// vertex
	obj->c       = 0;		// no fixed color
	obj->view    = &view;		// camera
	obj->light   = &light;		// light
	obj->driver3 = objDrvParticle;	// driver
}

// 
// 	main entry
// 
// �֐� draw() �����t���[���Ăяo����܂��B
// �e�ݒ�̏�����t���[���_�u���o�b�t�@�̃n���h�����O�� sce_pp3.c ��
// �܂Ƃ߂ċL�q����܂��B�֐� draw() �͂��ׂĂ̎G�p���������ꂽ��Ă�
// �o����܂��B�]���Ă����ł� GS �̐ݒ�Ɋւ�����ƃR�[�h�͊܂܂��
// ����B
// 
int draw(int key_code)
{
	static qword	p3_buf[2][0x10000];	// packet buffer
	static int	bufid = 0;		// buffer ID
	static VECTOR	orot, prot;		// object/particle rotation 
	OBJECT	object;
	MATRIX	mw;

	// update camera postion
	switch (key_code) {
	    case 'n': AddVector(view.p, view.p, v); break;
	    case 'p': SubVector(view.p, view.p, v); break;
	    case 'l':
		light.model = (light.model+1)%4;
		break;
	}

	// stop moving to avoid black out
	if (view.p[0] < -300 || view.p[0] > 300)
		key_code = 0;

	// rotate each particle and it's parents
	orot[0] += 0.5*PI/180;	// parent rotation
	orot[1] += 0.2*PI/180;
	prot[0] += 1.0*PI/180;	// particle rotation
	prot[1] += 2.0*PI/180;

	if (prot[0] > PI) prot[0] -= 2*PI;
	if (prot[1] > PI) prot[1] -= 2*PI;
	if (orot[0] > PI) orot[0] -= 2*PI;
	if (orot[1] > PI) orot[1] -= 2*PI;

	// update matrix
	// you always look at (0,0,0)
	ScaleVector(view.zd, view.p, -1);
	updateView(&screen, &view);
	updateLight(&light);

	// clear GIF and GS
	objInit(0, p3_buf[bufid]);
	bufid = 1-bufid;

	// init
	init_cube(&object);
	
	// make matrix
	object.mw = &mw;
	UnitMatrix(mw);
	RotMatrixX(mw, mw, orot[0]);
	RotMatrixY(mw, mw, orot[1]);

	// set particle attributes: see objpart.c
	objDrvParticleAttributes(prot, 5500, 128);
	objDraw(&object);

	// kick DMA
	objFlush(0);

	// return
	return(key_code);
}

// entry for OpenGL simulator
//
// �ȉ��̍s�� OpenGL ��p�����V�~�����[�^����œ��삷�邽�߂̂��̂�
// ���B�ʏ�� OPENGL ���`����K�v����܂���B
//
#ifdef OPENGL
void usage(void) {}
#else
void main1(void)
{
	int i;
	while (1) {
		for (i = 0; i < 1200; i++) draw('n');
		for (i = 0; i < 1200; i++) draw('p');
	}
}
#endif
