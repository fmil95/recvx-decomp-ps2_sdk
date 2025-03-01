/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - tmip -
 *
 *                         Version 0.10 
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            tmip.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *	0.1		Jul,13,1998	suzu	    1st version
 *	0.2		Dec,23,1999	suzu	    changed for libgraph
 *	0.3		May,13,1999	suzu	    add comment
 */

//
// ����́A�I�u�W�F�N�g�̊e���_���Q�g�����A�Q����`���}���ĐV������
// �_�𐶐������ł��B����� MIPMODEL �ɂ����ă��f���Ԃ̓��}���s�Ȃ�
// �ꍇ�Ɏg�p���܂��B 
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
	1,						 // light mode
	// 	light-0		light-1		light-2
	{  0,  0,150,1}, { 90,  0, 90,1},{  0, 90, 90,1},// light position
	{  0,  0,  0,0}, {  0,  0,  0,0},{  0,  0,  0,0},// light direction
	{0.8,0.0,0.0,0}, {0.0,0.8,0.0,0},{0.0,0.0,0.8,0},// influence
	{0.2,0.2,0.2,0},				 // ambient
};

// 
// 	Camera position information
// 
// �J�����ʒu���w�肵�܂��B�J�����͉��������ɗ��Ă��A��Ɍ��_������
// �悤�ɏC������܂��B���Ȃ킿 view.zd = -vies.p �ƂȂ�悤�ɃJ����
// �̌������ݒ肳��܂��B
//
VIEW view = {
	{   0,  0,  -1, 0},		// foot direction
	{   1,  0,  -1, 0},		// eye direction 
	{-200,  0, 600, 1},		// standing position
};

// Your moving speed
// �J�����̈ړ����x���w�肵�܂��B���̒l���t���[������ view.p �����Z
// ����܂��B 
static VECTOR	 v = {0, 1, -2.5, 0};

// 
// initialize object vertex
// 
// �I�u�W�F�N�g�̒��_��ݒ肵�܂��B�I�u�W�F�N�g�͂R��ނ��肻�ꂼ���
// ���̌`������f�[�^�����蓖�Ă��܂��B
//
//			cvnt		cvnt1
//			---------------------	
//			��		�~��
//			�~��		�~��
//			�~��		��
//
// ���}�W�� rate �̒l�ɉ����āAcvnt, cvnt1 �Œ�`���ꂽ��̌`��
// ���d�ݕt�����ς���V�������_����������܂��B
//
// �J�E���^�I�u�W�F�N�g (cvnt1) �̐����ɂ͈�U�_�~�[�̃I�u�W�F�N�g 
// wbase �𐶐������̒��_�f�[�^�̃|�C���^�� obj �ɃR�s�[������@���Ƃ��
// �܂��B
//
static void init_morph_obj(OBJECT *obj, 
			int width, int step, void (*func0)(), void (*func1)())
{
	OBJECT	wbase;

	// set basic attributes
	memset(obj, 0, sizeof(OBJECT));
	objMakeTag(obj, 0, 0, 0, 1);
	obj->view    = &view;
	obj->light   = &light;
	obj->shadow  = 1;
	obj->clip    = 0;
	obj->spec    = 0;
	obj->rate    = 0.0;
	obj->c       = malloc(sizeof(VECTOR));
	obj->c1      = malloc(sizeof(VECTOR));
	obj->box     = malloc(8*sizeof(VECTOR));
	obj->driver3 = objDrvNormal;

	setVector(obj->c[0],  100, 200, 255, 0);
	setVector(obj->c1[0], 200, 100,   0, 0);

	// map
	(*func0)(obj,    width, step);
	(*func1)(&wbase, width, step);

	// set counter vertex
	obj->cvnt1 = wbase.cvnt;

}

// 
// draw objects
// 
static void draw_obj(float rotx, float roty, float rate)
{
	static OBJECT	obj0, obj1, obj2;
	static int	first = 1;
	static float	r[] = {1, 2.5, 4, -1};
	MATRIX		mw;
	int		i;
	float		dr = 0.3;

	// if first, generate vertex
	if (first) {
		first = 0;
		init_morph_obj(&obj0, 30, 4, map_corn,     map_sphere);
		init_morph_obj(&obj1, 30, 4, map_sphere,   map_cylinder);
		init_morph_obj(&obj2, 30, 4, map_cylinder, map_corn);
	}

	// make rotation
	UnitMatrix(mw);
	RotMatrixX(mw,  mw, rotx);
	RotMatrixY(mw,  mw, roty);

	// setup
	obj0.mw   = obj1.mw   = obj2.mw   = &mw;

	for (i = 0; r[i] > 0; i++) {

		// update morphing rate
		if (rate > 1.0)	{ rate = 2.0 - rate; dr = -0.3;}
		if (rate < 0.0)	{ rate = 0.0 - rate; dr = +0.3;}
		
		obj0.rate = obj1.rate = obj2.rate = rate;
		rate += dr;

		// draw object 0
		setVector(mw[3],   0*1.72*r[i],  60*r[i], 42, 1); 
		objDraw(&obj0);
		
		// draw object 1
		setVector(mw[3],  30*1.72*r[i], -30*r[i], 42, 1); 
		objDraw(&obj1);

		// draw object 2
		setVector(mw[3], -30*1.72*r[i], -30*r[i], 42, 1); 
		objDraw(&obj2);
	}
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
	static float	rotx = 0.0, roty =  0.0, rate = 1.0;
	static qword	p3_buf[2][0x10000];		// packet buffer
	static int	bufid = 0;
	
	switch (key_code) {
		case 'n': AddVector(view.p, view.p, v);	break;
		case 'p': SubVector(view.p, view.p, v);	break;
		case 'f': light.lp0[2] -= 3;		break;
		case 'b': light.lp0[2] += 3;		break;
		case 'l':
			light.model = (light.model+1)%4;
			key_code = 0;
			break;	
	}
	// update
	rotx += 0.01;
	roty += 0.02;

	if (rotx > PI) rotx -= 2*PI;	
	if (roty > PI) roty -= 2*PI;	

	// for demonstration
	rate += 0.02;
	if (rate > 2.0) rate -= 2.0;

	// break system 
	if (view.p[2] < 32 || view.p[2] > 1024)
		key_code = 0;
	light.lp0[2] = limit(light.lp0[2], 40, 800);

	// update matrix
	// you always look at (0,0,0)
	ScaleVector(view.zd, view.p, -1);
	updateView(&screen, &view);
	updateLight(&light);

	// clear GIF and GS
	objInit(0, p3_buf[bufid]);
	bufid = 1-bufid;

	// draw reference plane
	light.model = 3;
	draw_zplane(280, 40);

	// draw object
	light.model = 2;
	draw_obj(rotx, roty, rate);

	// kick
	objFlush();

	return(key_code);
}

// entry for OpenGL simulator
//
// �ȉ��̍s�� OpenGL ��p�����V�~�����[�^����œ��삷�邽�߂̂��̂�
// ���B�ʏ�� OPENGL ���`����K�v����܂���B
//
#ifdef OPENGL
void usage(void)
{
	printf("linear interporation test (for MIPMODEL):\n");
	printf("\tj/k:        morphing\n");
	printf("\tn/p:        move camera\n");
	printf("\tf/b:        move light source \n");
	printf("\tl:          toggle light mode (parallel/spot)\n");
	printf("\t[SPACE]:    pause\n");
	printf("\t[ESC]:      quit\n");
}
#else
void main1(void)
{
	int i, key_code = 'n';
	while (1) {
		while ((key_code = draw('n')));
		for (i = 0; i < 300; i++) draw(0);
		while ((key_code = draw('p')));
		for (i = 0; i < 600; i++) draw(0);
	}
}
#endif
