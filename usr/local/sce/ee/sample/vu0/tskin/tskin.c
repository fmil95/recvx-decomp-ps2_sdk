/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - tskin -
 *
 *                         Version 0.1.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : tskin.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *	0.1		Jul,13,1998	suzu	    1st version
 *	0.2		Dec,23,1999	suzu	    changed for libgraph
 *	0.3		May,13,1999	suzu	    add comment
 */
//
// ����́A�P��̃I�u�W�F�N�g�ɕ����̍s���K�p�����ł��B�֐߂�����
// ���ɐڍ�����ꍇ�ɂ͕K�v�Ȏ�@�ł��B
// �������W�n�̊�{�I�ȓ���Ɋւ��ẮAtballs.c ���Q�Ɖ������B
//
// 
// �T���v���ɂ͈ȉ��̃��[�J���w�b�_�t�@�C�� object.h ���K�v�ł��B
//	
#include "object.h"

// 	
// Screen information
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
// 	light-0		 light-1	light-2
	{100,-90,200,1}, {  0,40,40,1},	{-40,40,40,1},	// light source
	{  0,  0,  0,0}, {  0, 0, 0,0},	{  0, 0, 0,0},	// light direction 
	{0.8,0.8,0.8,0}, {  0, 0, 0,0}, {  0, 0, 0,0},	// light influence
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
	{   0,   0, -1, 0},		// foot direction
	{   0,   0,  1, 0},		// eye directio 
	{ 100,-400,100, 1},		// standing position
};

// Your moving speed
// �J�����̈ړ����x���w�肵�܂��B���̒l���t���[������ view.p �����Z
// ����܂��B 
static VECTOR	 v = {1.5,  2.5, 1, 0};

//
// init_joint:
//	�ג����~���̃I�u�W�F�N�g�f�[�^�𐶐����܂�
// 	disp_slim_cylinder() �� init_joint() ���R�[������܂�
//
static float scale; 
static void disp_slim_cylinder(int ix, int iy, int mx, VECTOR p)
{
	float	x, z, r, ax;
	int	t;
	x  = (float)ix/mx;		// x = [-1.0,1.0]	
	z  = (float)iy/mx;		// y = [-1.0,1.0]
	r  = 0.25;
	ax = x*PI;
	p[0] = scale*r*cos(ax);
	p[1] = scale*r*sin(ax);
	p[2] = scale*z;
	t = 4*(iy+mx)/mx;		// build-in matrix ID
	p[3] = *(float *)&t;
}

static int init_joint(OBJECT *obj, int width, int step)
{
	VECTOR	*meshv, *meshn;
	int	mx, nv, nm, ns;

	mx    = width/step;		// mesh grid count
	nm    = 2*mx+2;			// mesh point cont (with 1 margine)
	nv    = 2*(2*mx+1);		// strip mesh length
	ns    = 2*mx;			// strip mesh count
	scale = width;

	memset(obj, 0, sizeof(OBJECT));
	objMakeTag(obj, 0, 0, 0, 1);
	obj->spec    = 0;
	obj->shadow  = 1;
	obj->rate    = 0.0;
	obj->clip    = 1;
	obj->view    = &view;
	obj->light   = &light;
	obj->driver3 = objDrvNormal;
	obj->c       = malloc(sizeof(VECTOR));
	obj->cvnt    = malloc(((nv*4+1)*ns+1)*sizeof(VECTOR));
	obj->box     = 0;
	setVector(obj->c[0], 200,200,100, 0);

	meshv  = malloc(nm*nm*sizeof(VECTOR));
	meshn  = malloc(nm*nm*sizeof(VECTOR));

	make_mesh(meshv, meshn, mx, disp_slim_cylinder);
	map_mesh(obj, meshv, meshn, mx);

	free(meshv);
	free(meshn);
	return(nv*ns);
}


//
// draw_joint:
//	�ό`����~���̃f�B�X�v���C���X�g�𐶐����܂��B
// 	1 �̉~���ɑ΂��� 16 �̈قȂ�}�g���N�X�����蓖�Ă��܂��B
//	�~���̊e���_���g�p����}�g���N�X�̎w��́A���̒��_�� w �t�B�[
// 	���h�𗬗p���܂��B
//	�֐� make_joint_matrix() ���A���I�ɕω����� 16 �̍s��𐶐�
//	���܂��B	
//

static void draw_joint(OBJECT *joint, float rotc, float rotp, VECTOR tv)
{
	static int first = 1;
	static MATRIX	mj[16];
	MATRIX	mw;
	int	i, nv;

	if (first) {
		first = 0;
		nv = init_joint(joint, 60, 15);
	}

	// set model-world
	if (rotc > PI) rotc -= 2*PI;
	if (rotp > PI) rotp -= 2*PI;

	UnitMatrix(mw);
	RotMatrixZ(mw,  mw, rotc);
	TransMatrix(mw, mw, tv);

	// update local joint matrix
	joint->mw = mj;
	make_joint_matrix(mj, 8, 64, rotp);

	// map the local matrix to the world
	for (i = 0; i <= 8; i++)
		MulMatrix(mj[i], mw, mj[i]);

	// draw
	objDraw(joint);
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
	static float	rotp = 0;
	static OBJECT	joint;
	static int	lmodel = 2;
	static qword	p3_buf[2][0x10000];
	static int	bufid = 0;

	VECTOR		tv;
	int		i;
	float		a;

	switch (key_code) {
		case 'n': AddVector(view.p, view.p, v);	break;
		case 'p': SubVector(view.p, view.p, v);	break;
		case 'f': light.lp0[2] -= 5;		break;
		case 'b': light.lp0[2] += 5;		break;
		case 'l':
			lmodel = (lmodel+1)%4;
			key_code = 0;
			break;	
	}
	// update
	rotp += 0.1;
	if (rotp > PI) rotp -= 2*PI;

 	// break system 
	if (view.p[2] > 500 || view.p[2] < 30)
		key_code = 0;
	light.lp0[2] = limit(light.lp0[2], 20, 100);

	// set eye point
	// you always look at (0,0,0)
	ScaleVector(view.zd, view.p, -1);

	// update matrix
	updateView(&screen, &view);
	updateLight(&light);

	// clear GIF and GS
	objInit(0, p3_buf[bufid]);
	bufid = 1-bufid;

	// draw reference plane
	light.model = 3;
	draw_zplane(300, 60);

	// draw the central cylinder
	light.model = lmodel;
	setVector(tv, 0, 0, 64, 1);
	draw_joint(&joint, 0, rotp, tv);

	// draw other cylinders
	for (a = 0, i = 0; i < 12; i++, a += 2*PI/12) {
		tv[0] = 150*cos(a);
		tv[1] = 150*sin(a);
		tv[2] = 64;
		tv[3] = 1;
		draw_joint(&joint, a, a+rotp, tv);
	}

	// flush
	objFlush();

	// return
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
	printf("multi-matrix model test: \n");
	printf("\tn/p:        move camera\n");
	printf("\tf/b:        move light source \n");
	printf("\tl:          toggle light mode (parallel/spot)\n");
	printf("\t[SPACE]:    pause\n");
	printf("\t[ESC]:      quit\n");
}
#else
void main1(void)
{
	int i;
	while (1) {
		for (i = 0; i < 180; i++) draw(0);
		while (draw('n'));

		for (i = 0; i < 180; i++) draw(0);
		while (draw('p'));
	}
}
#endif


