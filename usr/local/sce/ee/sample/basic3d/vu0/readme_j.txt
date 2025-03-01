[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
		     Copyright (C) 1999 by Sony Computer Entertainment Inc.                                                       All Rights Reserved

�x�[�V�b�N�T���v�� basic3d VU0��
================================

<�T���v���̉��>
basic3d Core�łɂ����čs�񉉎Z�A�����ϊ������s�����w�֐��A3D�O���t�B�b�N	�X�ɕK�v�Ȑ��w�̊�b�ɂ��ė�����[�߂���ɁA�������̑��X�e�b�v�Ƃ���	VU0�}�N�����߂ւ̏���������������܂��B
VU0�}�N�����߂́A�R�v�����߂ł��̂�CPU�̖��߂Ƃ��ăA�Z���u���`���ł̗��p	���\�ł��B�܂��A�����ɍő�S�̃f�[�^���������邱�Ƃ��ł��邽�߁A�s��	���Z�A�x�N�g�����Z�Ƃ̐e�a���������Ȃ��Ă��܂��B
����basic3d VU0�ł́ACore�ł̍s�񉉎Z����VU0�}�N�����߂Œu������������	�����ƂŁAVU0�}�N�����߂̗��p�@�ɂ��Ă̗�����[�߂邱�Ƃ�ړI�Ƃ��Ă�		�܂��B�����āA����Ȃ鍂�����̂��߂�VU0�}�C�N�����߂��邢��VU1�}�C�N����	�߂𗘗p����ۂ̋��n���ƂȂ邱�Ƃ����҂��Ă��܂��B

<�t�@�C��>
	main.c		���C���v���O����
	vu0.c		���W�ϊ��A�����ϊ����̍s�񉉎Z�֐�
	cube.s		�I�u�W�F�N�g�f�[�^(������)
	torus1.s	�I�u�W�F�N�g�f�[�^(�g�[���X �f�[�^����)
	torus.s		�I�u�W�F�N�g�f�[�^(�g�[���X �f�[�^����)
	flower.dsm	�e�N�X�`���f�[�^

<�N�����@>
	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����́A�ȉ��̕��@�ł����s�\
	% dsedb
	> run main.elf

<�R���g���[���̑�����@>
	����(�����L�[) : x����](�I�u�W�F�N�g)
	����	�@�@�@ : y����](�I�u�W�F�N�g)
	L1L2�{�^���@�@ : z����](�I�u�W�F�N�g)

	���~�{�^���@�@ : x����](���_)
	�����{�^���@�@ : y����](���_)
	R1R2�{�^���@�@ : z���ړ�(���_)

	SELECT�{�^���@ : �I�u�W�F�N�g�̐ؑւ�(CUBE/TORUS)

<�d�l>
	�e�N�X�`���t��Triangle Strip�̃L���[�u��\������B
	�e�N�X�`���t��Triangle�̃g�[���X��\������B

	Z�o�b�t�@��24bit
	������3��
	�e�N�X�`����32bit

<�����T��>
	1. �e�N�X�`�������[�h
	2. �p�b�h�I�[�v��
	3. �����ϊ��s��E�����v�Z�s��Ȃǂ�ݒ�
	4. �����ϊ��E�����v�Z���s���AGS�ɓn���p�P�b�g�𐶐�
	6. GS�ւ�DMA kick
	7. 2�ɖ߂�


�f�[�^�t���[
--------------------------------------------------------------------------

<�������ߒ�>
���C��RAM                          GS����RAM(4M)
  �e�N�X�`���f�[�^ ----------------> �e�N�X�`���f�[�^�W�J
  �I�u�W�F�N�g�f�[�^�W�J
  �����ϊ��s�񓙂̓W�J

<���s�����ߒ�>
���C��RAM                                �X�N���b�`�p�b�h(16K)
  �I�u�W�F�N�g�f�[�^ --------------------> GS�]���p�p�P�b�g�f�[�^�W�J
                     �����ϊ��A�����v�Z    (�v���~�e�B�u�f�[�^
                     (VU0�W�I���g��)        (STQ, RGBA, XYZF)�z��)
                                                    |
                                                    |
GS����RAM(4M) <-------------------------------------+
  �t���[����Z�o�b�t�@��������         DMA�]��(DMAtag,GIFtag�t��)
  (�����_�����O)


�v���O�����̐���(�ڍ�)
--------------------------------------------------------------------------

<�ϐ��E�\���́E�֐�>�@(mathfunc.c�̊֐�������)

	SampleCubeDataHead[]     :�I�u�W�F�N�g�f�[�^(������)�ւ̃|�C���^
	SampleTorus1DataHead[]   :�I�u�W�F�N�g�f�[�^(�g�[���X)�ւ̃|�C���^
	My_texture1[]            :�e�N�X�`���f�[�^�ւ̃|�C���^

	TexEnv	  :sceGsTexEnv�\���̂�]�����邽�߂̃p�P�b�g�쐬�̍\����
	QWdata	  :128bit�f�[�^�������Ղ����邽�߂̋��p��
	GifPacket :Gif(GS)�ɓ]������p�P�b�g�𐶐����邽�߂̍\����
	ObjData	  :�������ɓW�J�����I�u�W�F�N�g�f�[�^���Ǘ����邽�߂̍\����

	camera_p  	:�J�����ʒu
	camera_zd 	:�J�����̎��������x�N�g��
	camera_yd 	:�J�����̉������x�N�g��
	camera_rot	:�J�����̉�]�p�x�N�g��

	light0		:���C�g0
	light1		:���C�g1
	light2		:���C�g2
	color0		:���C�g0�̐F
	color1		:���C�g1�̐F
	color2		:���C�g2�̐F

	ambient		:����

	obj_trans	:�I�u�W�F�N�g�̈ړ�
	obj_rot		:�I�u�W�F�N�g�̉�]

	local_world	:�I�u�W�F�N�g���[�J�����W��world���W�n�ɕϊ�����
			 �s��
	world_view	:world���W�n�����_���W�n�ɕϊ�����s��
	view_screen	:���_���W�n���X�N���[�����W�n�ɕϊ�����s��
	local_screen	:���[�J�����W���X�N���[�����W�ɕϊ�����s��
							(��R�s��̐�)

	normal_light	:���[���h���W�n�ɂ����钸�_�̖@���x�N�g���ƃ��C�g�� 			    �����x�N�g���̓��ς���邽�߂̍s��
			 (���C�g�̕����x�N�g�����Z�b�g�����)
	local_light	:normal_light�s���local_world�s����|���āA
			 ���[�J�����W�n�ɂ����钸�_�̖@���x�N�g����
			 ���C�g�̕����x�N�g���̓��ς�����悤�ɂ���s��
	light_color	:local_light�s��ƒ��_�̖@���x�N�g���̐ςɂ��
			 �������ꂽ���σx�N�g���ɁA�e���C�g�̐F(3�F�{��
			 ��)���|���Ęa���Ƃ�s��
	local_color	:local_light��light_color�̐ς�������s��
			 (���σx�N�g���𐶐�������ɁA�e���ϒl��0.0�`1.0��
			 �͈͂ŃN���b�s���O����K�v�����邽�߁A���ۂ͎g����			    ��)
	work		:�ꎞ�I�ȍ�Ƃ̂��߂̍s��

	scePadData      pad	:�p�b�h�f�[�^��ǂݍ��ނ��߂̍\����
	sceGsDBuff      db	:�_�u���o�b�t�@���������邽�߂̍\����
	sceGsLoadImage  pTex	:�e�N�X�`����GS���[�J���������ɓ]������̂�    				    �p����\����
	sceDmaChan      *dmaVif	:DMA�ɂ��f�[�^�]�������邽�߂̍\����
	TexEnv          texenv	:�e�N�X�`���f�[�^��]�����邽�߂̃p�P�b�g��				    �쐬����\����

	LoadObj(ObjData *o, u_int *Head)
			:���C���������ɓW�J�����I�u�W�F�N�g�f�[�^��ObjData
			 �\���̂ɓo�^���A���삷�邽�߂̊֐��B

	ReleaseObj(ObjData *o)
			:�o�^�����I�u�W�F�N�g�f�[�^���J�����邽�߂̊֐�

	MakePacket(ObjData *obj, int num)
			:�I�u�W�F�N�g�f�[�^�ɑ΂��ē����ϊ��E�����v�Z�Ȃǂ�
			 �s���AGS�ɓ]������p�P�b�g�𐶐�����֐�

<main�֐����̕ϐ�>
	i, j		:for���[�v�Ȃǂŗp����J�E���^
	fd		:�p�b�h��open����Ƃ��Ԃ�l���󂯎�� file 
			 descriptor
	fr		:�_�u���o�b�t�@�Ńt���b�v���s�����߂̃g�O���X�C�b�`
	delta		:��]�p��ݒ肷�邽�߂̕ϐ�
	obj		:�I�u�W�F�N�g��o�^���邽�߂̕ϐ�


�v���O��������i�����������j
--------------------------------------------------------------------------


�E�I�u�W�F�N�g�f�[�^�̓ǂݍ���
    LoadObj(&obj[0], SampleCubeDataHead);
    LoadObj(&obj[1], SampleTorus1DataHead);

	�I�u�W�F�N�g�f�[�^��SampleCubeDataHead�ASampleTorus1DataHead�����
	�ݍ��݂܂��B

�E�f�o�C�X�̏�����
    sceGsResetPath();	//�f�[�^�p�X�f�o�C�X�̏�����(VIF1,VU1,GIF)
    sceDmaReset(1);	//DMA������������(DMA)
    sceVpu0Reset();	//VU0������������(VIF0,VU0)

	�v���O�����̐擪�ŁA�g�p���邷�ׂẴf�o�C�X�̏��������s���܂��B��
	�������s���Ă��Ȃ��f�o�C�X�𗘗p�����ꍇ�A���҂�����������Ȃ�����
	������܂��̂ŁA�K�����s���Ă��������B

�E�`����̐ݒ�
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
		//GS������������
    sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT, 
			SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);
				//�_�u���o�b�t�@�̐ݒ�������Ȃ�
    *(u_long *)&db.clear0.rgbaq = SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10,
					0x80, 0x3f800000);
    *(u_long *)&db.clear1.rgbaq = SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10,
							0x80, 0x3f800000);

	�e�f�o�C�X�̏��������I�������ɁAGS�̕`����̐ݒ���s���܂��B��
	�̐ݒ���s��Ȃ��Ɖ�ʏ�ւ̕\�����ł��܂���B�܂��AVSync�œ�������	   ���Ă���R���g���[���̐ݒ���s���܂���̂ŁA�f�o�C�X�̏����������
	���̐ݒ����x�͍s���悤�ɂ��Ă��������B

�E�e�N�X�`���p�̃C���[�W�]��
    sceGsSetDefLoadImage(&pTex,13340,IMAGE_SIZE / 64,SCE_GS_PSMCT32,
			0, 0,IMAGE_SIZE, IMAGE_SIZE);//LoadImage���̐ݒ�
    FlushCache(0);
    sceGsExecLoadImage(&pTex, (u_long128 *)My_texture1);//LoadImage�̎��s
    sceGsSyncPath(0, 0);	//�f�[�^�]���̏I���҂�

	�e�N�X�`���p�̃C���[�W��GS���[�J��������ɓ]�����邽�߂ɁALoadImage        �̏���ݒ肵�܂��B���̌�AsceGsExecLoadImage()�����s���邱�ƂŁA
	�w�肳�ꂽ�C���[�W��GS�̃��[�J��������ɓ]������܂��B���̊֐��͓�
	����DMA�𗘗p���܂��̂ŁA�]���O��FlushCache(0)��D�L���b�V���ƃ��C��        ������Ԃ̐�����������Ă��܂��B�Ō�ɁAsceGsSyncPath()�Ńf�[�^�]��	   �̏I���҂��������Ȃ��܂��B����ŁAGS�̃��[�J���������ւ̃C���[�W�]
	�����������܂��B

�E�e�N�X�`���̊��ݒ�p�p�P�b�g�̐���
    FlushCache(0);
    texenv.size = sceGsSetDefTexEnv(&texenv.gs_tex, 0, 13340, 
	IMAGE_SIZE / 64,SCE_GS_PSMCT32, 8, 8, 0, 0, 0, 0, 1);
		//�e�N�X�`���ݒ�f�[�^�̍쐬
    SCE_GIF_CLEAR_TAG(&texenv.giftag);
    texenv.giftag.NLOOP = texenv.size;
    texenv.giftag.EOP = 1;
    texenv.giftag.NREG = 1;
    texenv.giftag.REGS0 = 0xe;
    *(u_long*)&texenv.gs_tex.clamp1
	= SCE_GS_SET_CLAMP(0, 0, 0, 0, 0, 0);

	sceGsSetDefTexEnv()��p���āA�e�N�X�`���֘A��GS�ėp���W�X�^TEX0_1, 	   TEX0_1, CLAMP_1�̐ݒ���s���p�P�b�g���쐬���܂��B

�E�e�N�X�`���̊��ݒ�p�p�P�b�g�̓]��
    FlushCache(0);
    dmaVif = sceDmaGetChan(SCE_DMA_GIF);
    dmaVif->chcr.TTE = 1;
    sceDmaSendN(dmaVif, &texenv.giftag, texenv.size+1);
				//�e�N�X�`���ݒ�f�[�^�̓]��
    sceGsSyncPath(0, 0);	//�f�[�^�]���̏I���҂�

	�������ꂽ�e�N�X�`�����ݒ�p�̃p�P�b�g���APATH3�o�R��Normal DMA
	�ɂ���ē]�����܂��B

�E�R���g���[���̏�����
    fd = open("pad:0", 0);

	1P���̃R���g���[�����g�����߂̏��������s���܂��B�Ȍ�́Aread()�Ŏw
	�肵���o�b�t�@�Ƀf�[�^��ǂݍ��߂܂��B�R���g���[���̃f�[�^��VSync
	���ɍX�V����܂��B

�v���O��������i���C�����[�v�����j
--------------------------------------------------------------------------

 �ȉ��A���C�����[�v�̒��ōs����A�R���g���[���̏��擾�A�}�g���b�N�X��
�����A�p�P�b�g����(���W�ϊ��A�����ϊ��AUV->STQ�ϊ��A�����v�Z)�A�p�P�b�g��
�����ɂ��Đ������Ă����܂��B

�EView-Screen�}�g���b�N�X�̐���
    sceVu0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
	    2048.0f, 2048.0f,1.0f, 16777215.0f,1.0f,65536.0f);

	sceVu0ViewScreenMatrix()��p���āAView-Screen�}�g���b�N�X�����߂�
	���B�����ł́A���_���瓊�e�X�N�[�����܂ł̋�����512�A��ʂ̃A�X�y
	�N�g���1:0.47�A��ʂ̒��S��GdPrimitive���W�n��(2048,2048)�AZ�o�b
	�t�@�̒l��1.0-16777215.0�AZ�̒l��1.0-65536.0�Ƃ���View-Screen�}�g
	���N�X�����߂Ă��܂��B

�E�R���g���[���̏��擾
    if(pad.button & SCE_PADLdown){
        obj_rot[0] += delta; if(obj_rot[0]>PI) obj_rot[0] -= 2.0f*PI; }
    if(pad.button & SCE_PADLup){
        obj_rot[0] -= delta; if(obj_rot[0]<-PI) obj_rot[0] += 2.0f*PI; }
    if(pad.button & SCE_PADLright){
        obj_rot[1] += delta; if(obj_rot[1]>PI) obj_rot[1] -= 2.0f*PI; }
    if(pad.button & SCE_PADLleft){
        obj_rot[1] -= delta; if(obj_rot[1]<-PI) obj_rot[1] += 2.0f*PI; }
    if(pad.button & SCE_PADL1){
        obj_rot[2] += delta; if(obj_rot[2]>PI) obj_rot[2] -= 2.0f*PI; }
    if(pad.button & SCE_PADL2){
        obj_rot[2] -= delta; if(obj_rot[2]<-PI) obj_rot[2] += 2.0f*PI; }
    if(pad.button & SCE_PADRdown){
        camera_rot[0] += delta; 
	if(camera_rot[0]>PI*0.4) camera_rot[0] = 0.4f*PI; }
    if(pad.button & SCE_PADRup){
        camera_rot[0] -= delta; 
	if(camera_rot[0]<-PI*0.4) camera_rot[0] = -0.4f*PI; }
        if(pad.button & SCE_PADRright){
        camera_rot[1] += delta; 
	if(camera_rot[1]>PI) camera_rot[1] -= 2.0f*PI; }
    if(pad.button & SCE_PADRleft){
        camera_rot[1] -= delta; 
	if(camera_rot[1]<-PI) camera_rot[1] += 2.0f*PI; }
    if(pad.button & SCE_PADR1){
        camera_p[2] -= delta*5; if(camera_p[2]<-100) camera_p[2] = -100; }
    if(pad.button & SCE_PADR2){
        camera_p[2] += delta*5; if(camera_p[2]>-10) camera_p[2] = -10; }
    if(!toggle && (pad.button & SCE_PADselect)){
        obj_switch = (++obj_switch)&0x01; toggle = 1;
    }else if(!(pad.button & SCE_PADselect)){toggle = 0;}

	�R���g���[���̃{�^�����́Aread()��p���Ď擾�ł��܂��B
	�����ł́A����ꂽ�f�[�^�����ƂɁA�I�u�W�F�N�g�ƃJ�����̉�]�p�A��
	���ʂ��X�V���Ă��܂��B�R���g���[���̍��葤���I�u�W�F�N�g�A�E�葤��
	�{�^�����J�����̑���ƂȂ��Ă��܂��B

�ELocal-World�}�g���N�X(��]�̂�)�̐���
    sceVu0UnitMatrix(work);                       //�P�ʍs��
    sceVu0RotMatrix(local_world, work, obj_rot);  //��]�s��

	Local-World�}�g���X�́ALocal���W�ŕێ�����Ă���I�u�W�F�N�g���Ƃ�
	���߂�K�v������܂��B��]�A���s�ړ��Ƃ������A�t�B���ϊ����p�����
	�܂��B
	���̃T���v���ł́A��]�}�g���N�X����ѕ��s�ړ��}�g���N�X���v�Z����
	��������Z�������̂�Local-World�}�g���N�X�Ƃ��ėp���Ă��܂��B
	���������̃T���v���ł́A�R�s�ڂ̕��s�ړ�(obj_trans)��ݒ肷��O��
        local_light�}�g���N�X��ݒ肷��K�v������܂��B���s�ړ�����������
	�s���p����local_light�}�g���N�X�𐶐�����ƁA�����̌v�Z������
	���s���܂���B����͕��s�ړ��A������������s��̑�S���p���Čv
	�Z���邽�߂ł��B���������ĉ�]�s���ݒ肵������ɁAlocal_light�}
	�g���N�X������Ă��������B
	�����ЂƂ��ӓ_�Ƃ��āAobj_trans�̑�S�v�f�ɂ͕K���[����ݒ肵��
	���������B�}�g���N�X��[4,4]�v�f�ɂ͔��ɃN���e�B�J���Ȓl���Z�b�g
	����Ă��܂����A�[���ȊO�̒l�ł͂����ς��Ă��܂����ƂɂȂ�܂��B

�E���[�J�����C�g�}�g���N�X�̐���
    sceVu0NormalLightMatrix( normal_light, light0,light1,light2);
    sceVu0MulMatrix(local_light, normal_light,local_world);

	sceVu0NormalLightMatrix()��p���āA�R�̌�������Normal-Light�}�g
	���N�X�����߂܂��B�����Local-World�}�g���N�X����Z���āALocal-
	Light�}�g���N�X�𐶐����܂��B�O�q�����悤�ɁA���̂Ƃ��|����
	local_world�́A��]�݂̂��ݒ肳�ꂽ�}�g���N�X�ł���K�v������܂��B

�E���C�g�J���[�}�g���N�X�̐���
    sceVu0LightColorMatrix( light_color, color0, color1, color2, ambient);

	�����F����ъ��F���sceVu0LightColorMatrix()��p���ă��C�g�J���[
        �}�g���N�X�����߂܂��B�����v�Z���ɁA�e�����̉e���F�����܂�����ɁA
	���C�g�J���[�}�g���N�X����Z���āA�ŏI�I�ȃX�N���[����ł̒��_�F��
	���肵�܂��B

�ELocal-World�}�g���N�X(��]�����s�ړ�)�̐���
    sceVu0TransMatrix(local_world, local_world, obj_trans);
					//��]�����s�ړ��s��

	local_light�̃}�g���N�X�������I�������̂ŁAlocal_screen�}�g���N�X
	���쐬����O�ɕ��s�ړ��̗v�f��local_world�ɉ����܂��B

�EWorld-View�}�g���N�X�̐���
    sceVu0RotCameraMatrix(world_view, camera_p, camera_zd,camera_yd,
							camera_rot);

	���̊֐� sceVu0RotCameraMatrix()�́A������sceVu0CameraMatrix()����
	�яo���܂��B����sceVu0CameraMatrix()�p���āAWorld-View�}�g���N�X��
	���߂܂��BView���W�n�͎��_(eye)�����_(0,0,0)�Ƃ��A����������Z+�A�E
	�������X+�A��������Y+�ƂȂ��Ă��܂�(�E����W�n�j�B�����ł́Aworld
	�ɑ΂��Ď��_camera_p��(0,0,-25)�A����camera_zd��(0,0,1)�Ɛ�������
	camera_yd��(0,1,0)�Ƃ��A��������World-View�}�g���N�X�𐶐�����
	���BsceVu0RotCameraMatrix()�֐������ł͂����̃x�N�g����camera_rot	   �ɂ���ĉ�]���A���̌��ʂ�sceVu0CameraMatrix()�ɓn���Ă��܂��B

�ELocal-Screen�}�g���N�X�̐���
    sceVu0MulMatrix(work, world_view, local_world);	     //Local-View
    sceVu0MulMatrix(local_screen, view_screen, work);        //Local-Screen

	�����܂łɋ��߂�ꂽWorld-View�}�g���N�X�ALocal-World�}�g���N�X��
	�p���āA�܂�Local-View�}�g���N�X�����߂܂��B����ɁAView-Screen�}
	�g���N�X����Z���邱�Ƃ�Local-Screen�}�g���N�X�����߂��܂��B

�E�p�P�b�g�̐���
    MakePacket(&obj[obj_switch], i);

	�p�P�b�g�̐����́AMakePacke()�̓����ōs���܂��B�ڂ����͌�ŉ��
	���܂��B

�EGS�ւ̃p�P�b�g�]��
    FlushCache(0);
    sceDmaSend(dmaVif,(u_long128*)((u_int)pBase&0x3ff0|0x80000000));
    sceGsSyncPath(0, 0);

	MakePacket()�Ő������ꂽ�v���~�e�B�u�̃p�P�b�g���APATH3�o�R��Source        Chain DMA�����s���āA�]�����܂��B

�E�`����̐؂�ւ��AV-Blank�҂�
    if(fr&0x01)
        sceGsSetHalfOffset(&db.draw1,2048,2048,sceGsSyncV(0)^0x01);
    else
        sceGsSetHalfOffset(&db.draw0,2048,2048,sceGsSyncV(0)^0x01);

	�t�B�[���h���Ƃɓ������؂�ւ�����AV-Blank���Ԃ̊J�n��҂���
	���B


�v���O��������i�p�P�b�g�����j
--------------------------------------------------------------------------

�@�ȉ���MakePacket()�ōs���p�P�b�g���������̓��e��������܂��B

�E�p�P�b�g�����ݒ�
    pack->size = 0;
    pack->buf = (QWdata *)0x70000000;//SPR

	�p�P�b�g�̏������݈ʒu��擪�Ɏ����Ă��܂��B
	�p�P�b�g�𐶐�����̈���A�����ł̓X�N���b�`�p�b�h�̐擪�ɐݒ肵��
	���܂��B�X�N���b�`�p�b�h�𗘗p����ۂ́A�p�P�b�g�̑傫�����X�N���b
	�`�p�b�h�̑傫���ł���16KB���z���Ȃ��悤�ɒ��ӂ��Ă��������B���C��
	�������𗘗p����ꍇ�ł��ADMA�̓]������QWC�ɐݒ�ł���l�ɐ�������
	��܂��̂ŁA�p�P�b�g�̃T�C�Y��1MB�ȏ�ɂȂ�Ȃ��悤�ɒ��ӂ��Ă���
	�����B

�EDMAtag�AGIFtag�̐���
    //DMAtag
    pack->buf[pack->size].ul128 = (u_long128)0;
    pack->buf[pack->size++].ui32[0]
        = 0x70000000 | (obj->vertexNum[num]*3 + 1);

    //GIFtag
    pack->buf[pack->size].ul64[0]
	= SCE_GIF_SET_TAG(obj->vertexNum[num], 1, 1,obj->prim,
          SCE_GIF_PACKED,3);
    pack->buf[pack->size++].ul64[1] = 0x0412L;

	�p�P�b�g�擪�ɂ�DMAtag�����GIFtag������K�v������܂��BDMA�́A
	Source Chain mode ���g�p���܂��B���M����p�P�b�g�̃T�C�Y�͒��_���~
	3(STQ,RGBA,XYZF2)�{1(GIFtag)�ADMAtag��ID�́iDMA�p�P�b�g���ЂƂ�
	���Ȃ̂Łjend��p���܂��B�܂��AGIF��PACKED mode�𗘗p���Ă��܂��B
	�v���~�e�B�u�̐ݒ��GIFtag�̒��ōs���悤�ɂ��Ă��܂��BREGS�̒l�́A
	0x412(STQ,RGBA,XYZF2)�ƂȂ��Ă��܂��B

�ESTQ�ARGBA�AXYZ�̐���
    sceVu0RotTransPersNClipCol(&pack->buf[pack->size].ul128, local_screen,
         vertex,normal, texUV,color, local_light,light_color, 
         obj->vertexNum[num]);

	sceVu0RotTransPersNClipCol()��p���ăp�P�b�g�̒��g(STQ�ARGBA�AXYZ)
	�����߂Ă��܂��B


�v���O��������iVU0�}�N�����߁j
--------------------------------------------------------------------------

�ȉ��AsceVu0RotTransPersNClipCol()�̏����ɂ��Đ������܂��BVU0�̃}�N��
���߂�p���č��W�ϊ��E�����ϊ��E�����v�Z�Ȃǂ��s���֐��ŁA�{�T���v���̊j��
�Ȃ镔���ł��Bvu0.c���Q�Ƃ��Ă��������B

�E�}�g���b�N�X�̐ݒ�
    # Local-World�}�g���b�N�X�̐ݒ�
    lqc2	vf4,0x0(%1)	#set local_world matrix[0]
    lqc2	vf5,0x10(%1)	#set local_world matrix[1]
    lqc2	vf6,0x20(%1)	#set local_world matrix[2]
    lqc2	vf7,0x30(%1)	#set local_world matrix[3]
    # Local-Light�}�g���b�N�X�̐ݒ�
    lqc2	$vf17,0x0(%6)	#set local_light matirix[0]
    lqc2	$vf18,0x10(%6)	#set local_light matirix[1]
    lqc2	$vf19,0x20(%6)	#set local_light matirix[2]
    # Light-Color�}�g���b�N�X�̐ݒ�
    lqc2	$vf21,0x0(%7)	#set light_color matrix[0]
    lqc2	$vf22,0x10(%7)	#set light_color matrix[1]	
    lqc2	$vf23,0x20(%7)	#set light_color matrix[2]
    lqc2	$vf20,0x30(%7)	#set light_color matrix[3]		

	�ŏ��ɁAVU0��VF���W�X�^�ɁALocal-World�}�g���N�X�ANormal-Light�}�g
	���N�X�ALight-Color�}�g���N�X��ݒ肵�܂��B

�E���_�^�@���^���_�F�^ST�̓ǂݍ���
    lqc2	vf8,0x0(%2)	#load XYZ
    lqc2	$vf24,0x0(%4)	#load NORMAL
    lqc2	$vf25,0x0(%5)	#load COLOR
    lqc2	$vf27,0x0(%8)	#load ST

	�p�P�b�g�����ɕK�v�Ȓ��_���W�A�@���A���_�F�A�e�N�X�`�����W�����W�X
	�^�ɓǂݍ��݂܂��B

�E���W�ϊ�
    # (X0,Y0,Z0,W0)=[SCREEN/LOCAL]*(X,Y,Z,1)
    vmulax.xyzw     ACC, vf4,vf8
    vmadday.xyzw    ACC, vf5,vf8
    vmaddaz.xyzw    ACC, vf6,vf8
    vmaddw.xyzw     vf12,vf7,vf8

	Local-Screen�}�g���N�X�ƒ��_���W����Z���āA�X�N���[�����W�����߂�
	���B���̂Ƃ���W�̒l�́AView���W�n�ł�Z�̒l�Ɠ������Ȃ�܂��B

�E�����ϊ�
    # (X1,Y1,Z1,1)=(X0/W0,Y0/W0,Z0/W0,W0/W0)
    vdiv    Q,vf0w,vf12w
    vwaitq
    vmulq.xyzw	vf12,vf12,Q
    vftoi4.xyzw	vf13,vf12

	�X�N���[�����W��1/W�Ə�Z���܂��B����ɁAGIF�p�P�b�g�p�ɌŒ菬���l
	�ɕϊ����܂��B�����Ōv�Z����1/W�́A��Ƀe�N�X�`�����W�Ə�Z���邽
	�ߕێ����Ă����܂��B

�E�����̉e�������߂�
    # (L1,L2,L3)=[LLM](Nx,Ny,Nz)
    # LLM:���[�J�����C�g�}�g���b�N�X
    # L1,L2,L3:�����̉e��
    # Nx,Ny,Nz:�@���x�N�g��
    vmulax.xyzw    ACC, $vf17,$vf24
    vmadday.xyzw   ACC, $vf18,$vf24
    vmaddz.xyzw    $vf24,$vf19,$vf24
    vmaxx.xyz      $vf24,$vf24,$vf0 #���̒l��0�ɂ���

	���[�J�����C�g�}�g���N�X�Ɩ@���x�N�g������Z���āA�����̉e��������
	�܂��B�����̕����Ɩ@���x�N�g���̌����������ꍇ�A���ʂ����̒l���Ƃ�
	�̂ł�����0�T�`�����[�g���܂��B

�E�����̉e���F�����߂�
    # (LTr,LTg,LTb,LTw)=[LCM](L1,L2,L3,1)
    # LCM:���C�g�J���[�}�g���b�N�X
    # LTr,LTg,LTb:�����̉e���F
    vmulax.xyzw    ACC, $vf21,$vf24
    vmadday.xyzw   ACC, $vf22,$vf24
    vmaddaz.xyzw   ACC, $vf23,$vf24
    vmaddw.xyzw    $vf24,$vf20,$vf0	

	���C�g�J���[�}�g���N�X�ƌ����̉e������Z���āA�����̉e���F�����߂�
	���B

�E�X�N���[����̒��_�F�����߂�
    # (RR,GG,BB) = (R,G,B)*(LTr,LTg,LTb)
    # R,G,B:���_�F
    # RR,GG,BB:�X�N���[�����_�F
    vmul.xyzw	$vf26,$vf24,$vf25
    # [0..255] �T�`�����[�V����
    vmaxx.xyz	$vf26,$vf26,$vf0
    lui		$2,0x437f
    ctc2	$2,$vi21
    vnop
    vnop
    vminii.xyz	$vf26,$vf26,I
    vftoi0.xyzw	$vf26,$vf26

	�����̉e���F�ƒ��_�F����Z���邱�ƂŁA�X�N���[����̒��_�F�����܂�
	�܂��B���ʂ́A0-255�͈̔͂łȂ���΂Ȃ�܂���̂ŁA���͈̔͂ł̃T
	�`�����[�V�������s���܂��B����ɁAGIF�p�P�b�g�p�ɌŒ菬���l�ɕϊ�
	���܂��B

�EST�����߂�
    # (S,T,Q) = (s,t,1)/w
    vmulq.xyz	$vf28,$vf27,Q

	�p�[�X�y�N�e�B�u�R���N�V�����̂��߂ɁA�����ϊ����ɕێ����Ă�����
	1/W�̒l�������܂��B

�ESTQ�ARGBA�AXYZ�̒l��ۑ�����
    sqc2	$vf28,0x0(%0)		#store STQ 
    addi	%0,0x10
    sqc2	$vf26,0x0(%0)		#store RGBA 
    addi	%0,0x10
    sqc2	vf13,0x0(%0)		#store XYZ 
    addi	%0,0x10
    #
    addi	%3,-1
    addi	%2,0x10
    addi	%4,0x10
    addi	%5,0x10
    addi	%8,0x10
    bne		$0,%3,_rotTPNCC_loop

	GIFtag�̒��Őݒ肳��Ă��鏇���ɂ��������āASTQ�ARGBA�AXYZ�̒l���p
	�P�b�g�ɕۑ����Ă����܂��BGIF�ő���ꍇ�ARGBAQ���߂ɂ�����Q�̒l��
	���O��ST���߂œ������W�X�^�ɕۑ����ꂽ�l���ݒ肳��܂��̂ŁA������
	�͒��ӂ��Ă��������B
	�ȏ�ŁAsceVu0RotTransPersNClipCol()�̃��C�����[�v�͏I�����܂��B�w
	�肳�ꂽ���_���ɂȂ�܂ŁA�e�A�h���X���C���N�������g���Ȃ���A���_
	�f�[�^�ǂݍ��݈ȍ~�̏������J��Ԃ��܂��B
