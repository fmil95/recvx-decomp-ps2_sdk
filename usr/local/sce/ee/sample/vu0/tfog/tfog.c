/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */ 
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - tfog -
 *
 *                         Version 0.10 
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            tfog.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *	0.1		Jul,13,1998	suzu	    1st version
 *	0.2		Dec,23,1999	suzu	    changed for libgraph
 *	0.3		May,13,1999	suzu	    add comment
 */
// ����́APS2 �Ő��������f�[�^��̍\���ƃN���b�v����̗�������܂��B
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
	1024,		// distance to screen
	1.0,	0.5,	// aspect ratio
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
	{ 0, 0,50,1},	{-40,40,40,1},	{-40,40,40,1},	// light source position
	{ 0, 0, 0,0},	{  0, 0, 0,0},	{  0, 0, 0,0},	// light direction 
	{0.8,0.8,0.8,0},{0,0,0,0},	{0,0,0,0},	// light influence
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
	{    0,   0,  -1, 0},	// foot direction
	{    1,   1,  -1, 0},	// eye direction 
	{ -100,-100, 150, 1},	// standing position
};

// Your moving speed
// �J�����̈ړ����x���w�肵�܂��B���̒l���t���[������ view.p �����Z
// ����܂��B 
static VECTOR	 v = {0.5, 2.5, 0, 0};

//
// �����̂̒��_�f�[�^�𐶐����A�`�悵�܂��B
// �����̂� 2 �g�̃X�g���b�v���b�V���̑g�����ō�邱�Ƃ��ł��܂��B
// �f�[�^�̓X�g���b�v���b�V����P�ʂƂ���
//	
//		color, vertex, normal, texture
//	
// �̏����Œu����܂��B(cvnt)
//
// ���b�V���̐擪�� w �t�B�[���h�ɂ̓X�g���b�v�������L����܂��B
// ���_�� w �t�B�[���h�͓K�p�����}�g���N�X ID ���L����܂�
// �ʏ�I�u�W�F�N�g������̕ϊ��s��� 1 ��ނł���̂ł����� 0 ������
// ���܂��B 
//
// �X�N���[���ɓ���Ȃ������̂̓I�u�W�F�N�g�P�ʂŃN���b�v����܂��B
// �I�u�W�F�N�g�N���b�v�̓I�u�W�F�N�g�̃A�g���r���[�g�Ƃ��Ă��̃I�u�W�F
// �N�g�����钼���� (box) ���`�� box �ɑ΂��ăN���b�v�`�F�b�N��
// �s�Ȃ��܂��Bbox �̈ꕔ����ʂɊ܂܂��ꍇ�̓I�u�W�F�N�g�̑S���_��
// �ϊ�����܂��B
//
// ����ɑ΂��ė����̂̉��ɕ`�����n�ʂ͒��_�P�ʂ̃N���b�v���s�Ȃ��
// �܂��B
// �I�u�W�F�N�g�N���b�v�̓I�u�W�F�N�g�̑S���_�ɑ΂��ăN���b�v�`�F�b�N
// ���s�Ȃ�Ȃ��ōςނ��ߍ����ł����A���̔��ʁA�����ȃN���b�v�����邱�Ƃ�
// �ł��܂���BGS �v���~�e�B�u���W�n�� (-2048,-2048)-(2048,2048) �Ń��b
// �v���E���h���邽�߁A�I�u�W�F�N�g���傫���ꍇ�́A�e���_�� GS �v���~
// �e�B�u���W�n�̗L���͈͂Ɏ��܂邩�ǂ������ēx�����Ƀ`�F�b�N����K�v����
// ��܂��B
//
// �N���b�s���O����̎��ۂ� objsimpl.c �̃R�[�h���Q�Ɖ������B
//
//

#define C	255	// cube color
#define S	5	// cube size
#define N	680	// cube number

static void draw_cube(MATRIX *mw)
{
	// cube : color + vertex + normal + (texture)
	static VECTOR	cvnt[] = {

		 // 8 length triangle strip
		 {0, 0, 0, 8},	
		 // color	 
		 {C, 0, 0, 0},	 {C, 0, 0, 0},	 {C, 0, 0, 0},	 {C, 0, 0, 0},
		 {0, C, 0, 0},	 {0, C, 0, 0},	 {0, 0, C, 0},	 {0, 0, C, 0},
		 // vertex
		{-S, S, S, 0},	 {S, S, S, 0},	{-S,-S, S, 0},	 {S,-S, S, 0}, 
		{-S,-S,-S, 0},	 {S,-S,-S, 0},	{-S, S,-S, 0},	 {S, S,-S, 0},
		 // normal
		 {0, 0, 1, 0},	 {0, 0, 1, 0},	 {0, 0, 1, 0},	 {0, 0, 1, 0},
		 {0,-1, 0, 0},	 {0,-1, 0, 0},	 {0, 0,-1, 0},	 {0, 0,-1, 0},
		 // texture (not used)
		 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},
		 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},

		 // 2nd vertex (8 length triangle strip)
		 {0, 0, 0, 8},
		 {C, C, 0, 0},	 {C, C, 0, 0},	 {C, C, 0, 0},	 {C, C, 0, 0},
		 {0, C, C, 0},	 {0, C, C, 0},	 {C, 0, C, 0},	 {C, 0, C, 0},
		{-S,-S, S, 0},	{-S,-S,-S, 0},	{-S, S, S, 0},	{-S, S,-S, 0},
		{ S, S, S, 0},	 {S, S,-S, 0},	 {S,-S, S, 0},	 {S,-S,-S, 0},
		{-1, 0, 0, 0},	{-1, 0, 0, 0},	{-1, 0, 0, 0},	{-1, 0, 0, 0},
		 {0, 1, 0, 0},	 {0, 1, 0, 0},	 {1, 0, 0, 0},	 {1, 0, 0, 0},
		 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},
		 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},

		 // terminator
 		 {0, 0, 0, 0},
	};

	// rounding box for object clip
	static VECTOR b[] = {
		{-S,-S,-S, 1},	  {S,-S,-S, 1},	 {-S, S,-S, 1},	 {S, S,-S, 1}, 
		{-S,-S, S, 1},	  {S,-S, S, 1},	 {-S, S, S, 1},	 {S, S, S, 1}, 
	};
	OBJECT	object;
	memset(&object, 0, sizeof(OBJECT));
	objMakeTag(&object, 0, 0, 0, 0);	// GIFtag
	object.view   = &view;			// camera
	object.light  = &light;			// light
	object.mw     = mw;			// model-world matrix
	object.shadow = 1;			// shadow
	object.clip   = 0;			// clip detection
	object.spec   = 0;			// specular
	object.rate   = 0.0;			// morphing rate
	object.cvnt   = cvnt;			// cvnt
	object.c      = 0;			// no fixed color
	object.box    = b;			// rounding box
		object.driver3 = objDrvSimple;		// object driver
		
	objDraw(&object);			// draw
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
	static float	rotx = 0.0, roty =  0.0;	// rotation angle	
	static int	lmodel = 2;			// lighting mode
	static qword	p3_buf[2][0x20000];		// packet buffer
	static int	bufid = 0;			// buffer ID
	MATRIX		mw;				// model world
	int		ix, iy;

	// update your position
	switch (key_code) {
	    case 'n': AddVector(view.p, view.p, v);		break;
	    case 'p': SubVector(view.p, view.p, v);		break;
	    case 'f': light.lp0[2] -= 3;			break;
	    case 'b': light.lp0[2] += 3;			break;
	    case 'l': lmodel = (lmodel+1)%4; key_code = 0;	break;
	}

	// stop your postion to avoid blackout
	if (view.p[1] < -500 || view.p[1] > 500)
		key_code = 0;

	// stop light position
	light.lp0[2] = limit(light.lp0[2], 50, 400);

	// set eye point
	// keep your eye on (0,0,0)
	ScaleVector(view.zd, view.p, -1);

	// update matrix
	updateView(&screen, &view);
	updateLight(&light);

	// setup
	objInit(0, p3_buf[bufid]);
	bufid = 1-bufid;

	// make display list
	light.model = 3;
	draw_zplane(N, 40);	

	// update cube rotation
	rotx += 0.02;
	roty += 0.04;
	if (rotx > PI)	rotx -= 2*PI;
	if (roty > PI)	roty -= 2*PI;

	UnitMatrix(mw);
	RotMatrixX(mw, mw, rotx);
	RotMatrixY(mw, mw, roty);

	// draw cubes
	// Note that mw[3] indicates translation.
	light.model = lmodel;
	for (iy = -N; iy < N; iy += 40)
		for (ix = -N; ix < N; ix += 40) {
			setVector(mw[3], ix, iy, 15, 1);
			draw_cube(&mw);
		}

	// kick DMA
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
	printf("spot light operation test: \n");
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
		for (i = 0; i < 180; i++)
			draw(' ');
		while (draw('n'));

		for (i = 0; i < 180; i++)
			draw(' ');
		while (draw('p'));
	}
}
#endif
