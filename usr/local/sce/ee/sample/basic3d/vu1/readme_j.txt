[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                   Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                      All Rights Reserved

�x�[�V�b�N�T���v�� basic3d VU1��
================================

<�T���v���̉��>
	EE Core��VU0�Ŏ������Ă��������ϊ���p�P�b�g�쐬�̃v���Z�X���AVU1�}�C�N		�����[�h�ō����ɏ��������������܂��B���̍ۂɕK�v��VUMem1�̃_�u���o�b�t	�@�����Ȃǂ̃e�N�j�b�N���������A��荂���ȃW�I���g�����������������b��	�񋟂��܂��B

<�t�@�C��>
	main.c		���C���v���O����
	mathfunc.c	�����ϊ����̍s�񉉎Z�֐�(CPUcore��)
	mathfunc.h	�w�b�_�t�@�C��
	cube.dsm	�I�u�W�F�N�g�f�[�^(������)(VU1�p�p�P�b�g�`��)
	torus.dsm	�I�u�W�F�N�g�f�[�^(�g�[���X �f�[�^����)
			(VU1�p�p�P�b�g�`��)
	torus1.dsm	�I�u�W�F�N�g�f�[�^(�g�[���X �f�[�^����)
			(VU1�p�p�P�b�g�`��)
	flower.dsm	�e�N�X�`���f�[�^
	basic.vsm	VU1�}�C�N�����[�h�̎��s���߃t�@�C��
	vu1dump.c	VU1���[�J���������̃_���v�֐�

<�R���p�C�����@>
	% make

<�N�����@>
	% make run

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run main.elf

<�d�l>
	�e�N�X�`���t��Triangle Strip�̃L���[�u��\������B
	�e�N�X�`���t��Triangle�̃g�[���X��\������B
	(select�{�^���Ő؂�ւ�)

	Z�o�b�t�@��24bit
	������3��
	�e�N�X�`����32bit

  <�R���g���[���̑�����@>
	����(�����L�[) : �I�u�W�F�N�g��x����]
	����           : �I�u�W�F�N�g��y����]
	L1L2�{�^��     : �I�u�W�F�N�g��z����]
	�����{�^��     : ���[���h���W�n�ɑ΂���J������y����]
	���~�{�^��     : ���[���h���W�n�ɑ΂���J������x����]
	R1R2�{�^��     : �J�����̑O�i��މ�]
	SELECT�{�^��   : �I�u�W�F�N�g�؂�ւ�

<�����T��>
	1. �e�N�X�`�������[�h
	2. �p�b�h�I�[�v��
	3. �}�g���N�X�A�����}�g���N�X��VU1�֓n���p�P�b�g���ɃZ�b�g
	4. VU1�ւ�DMA kick
	5. 2�ɖ߂�

	VU1�}�C�N���v���O�����ł͈ȉ��̂悤�ȏ������s���Ă��܂��B
	1. ��] x �����ϊ��}�g���N�X��ݒ�
	2. �e��p�����[�^�i�}�g���N�X���j��VU1���W�X�^�Ƀ��[�h
	3. �@���x�N�g���A��Ԃ���Q�̍��W�AST�l�A�F�����[�h
	4. ���W��ԁA�����ϊ��A�����v�Z�����Č��ʂ�VU1Mem�ɃX�g�A
	5. 3. �ɖ߂�B���_����J��Ԃ��B
	6. �X�g�A���ꂽ�f�[�^(Gif Packet)��GS��XGKICK

	VU1Mem��ł̓_�u���o�b�t�@�����O���s���܂��B


�f�[�^�t���[
--------------------------------------------------------------------------

<�������ߒ�>
���C��RAM                          GS����RAM(4M)
  �e�N�X�`���f�[�^ ----------------> �e�N�X�`���f�[�^�W�J
  �I�u�W�F�N�g�f�[�^�W�J
  �����ϊ��s�񓙂̓W�J

<���s�����ߒ�>
���C��RAM                            VUMem1(16K)
  �I�u�W�F�N�g�f�[�^ ----------------> GS�]���p�p�P�b�g�f�[�^�W�J
  (�e��}�g���N�X��       DMA�]��      (�W�I���g�����Z�C�_�u���o�b�t�@)
   �W�I���g���O�f�[�^)                 (�v���~�e�B�u�f�[�^(STQ,RGBA,XYZF))
                                                    |
                                                    |
GS����RAM(4M) <-------------------------------------+
 �t���[����Z�o�b�t�@��������                 GIF�Ƀf�[�^Kick
 (�����_�����O)

�v���O�����̐���(�ڍ�)
--------------------------------------------------------------------------

<�ϐ��E�\���́E�֐�>�@�imathfunc.c�̊֐��������j

        My_dma_start            :VU1�֑���p�P�b�g�Q�̐擪�v�f
        My_dma_next             :VU1�֑���2�߂̃p�P�b�g�̐擪�v�f
        My_matrix               :�p�P�b�g����local_screen�}�g���N�X
        My_rot_trans_matrix     :�p�P�b�g����local_world�}�g���N�X
        My_light_matrix         :�p�P�b�g����local_light�}�g���N�X
        My_cube_start           :cube�p�P�b�g�f�[�^�̐擪�v�f
        My_torus_start          :torus�p�P�b�g�f�[�^�̐擪�v�f
        My_texture1             :�e�N�X�`���f�[�^�̐擪

        TexEnv          :sceGsTexEnv�\���̂�]������p�P�b�g�쐬�̍\����

        camera_p        :�J�����ʒu
        camera_zd       :�J�����̎��������x�N�g��
        camera_yd       :�J�����̉������x�N�g��
        camera_rot      :�J�����̉�]�p�x�N�g��

        light0          :���C�g0
        light1          :���C�g1
        light2          :���C�g2
        color0          :���C�g0�̐F
        color1          :���C�g1�̐F
        color2          :���C�g2�̐F

        ambient         :����

        obj_trans       :�I�u�W�F�N�g�̈ړ��x�N�g��
        obj_rot         :�I�u�W�F�N�g�̉�]�x�N�g��

        local_world     :�I�u�W�F�N�gLocal���W��World���W�n�ɕϊ�����s��
        world_view      :World���W�n��View���W�n�ɕϊ�����s��
        view_screen     :View���W�n��Screen���W�n�ɕϊ�����s��
        local_screen    :Local���W��Screen���W�ɕϊ�����s��(��R�s��̐�)

        normal_light    :World���W�n�ɂ����钸�_�̖@���x�N�g���ƃ��C�g��
                         �����x�N�g���̓��ς���邽�߂̍s��
                         (���C�g�̕����x�N�g�����Z�b�g�����)
        local_light     :normal_light�s���local_world�s����|���āA
                         Local���W�n�ɂ����钸�_�̖@���x�N�g����
                         ���C�g�̕����x�N�g���̓��ς�����悤�ɂ���s��
        light_color     :local_light�s��ƒ��_�̖@���x�N�g���̐ςɂ��
                         �������ꂽ���σx�N�g���Ɋe���C�g�̐F(3�F�{����)
                         ���|���Ęa���Ƃ�s��
        local_color     :local_light��light_color�̐ς�������s��

        work            :�ꎞ�I�ȍ�Ƃ̂��߂̍s��

        sceGsDBuff      db      :�_�u���o�b�t�@���������邽�߂̍\����
        scePadData      pad     :�p�b�h�f�[�^��ǂݍ��ނ��߂̍\����
        sceGsLoadImage  gs_limage:�e�N�X�`����GS���[�J���������ɓ]�����邽
                                 �߂̍\����
        TexEnv          texenv  :�e�N�X�`���f�[�^��]�����邽�߂̃p�P�b�g
                                 ���쐬����\����

        void SetVu1PacketMatrix(void)
                        :VU1�ɑ���p�P�b�g���Ƀ}�g���N�X���Z�b�g����֐�

<main�֐����̕ϐ�>

        fd              :�p�b�h��open�̍ەԂ�l���󂯎��t�@�C���L�q�q
        frame           :�_�u���o�b�t�@�Ńt���b�v���s�����߂̃X�C�b�`
        delta           :��]�p��ݒ肷�邽�߂̕ϐ�
        obj_switch      :�I�u�W�F�N�g��؂�ւ��邽�߂̃g�O���X�C�b�`
        toggle          :select�{�^���̓ǂݍ��݂��g�O���Ő��䂷��ϐ�

<mathfunc.c�ŃT�|�[�g����ϐ��E�֐�>

        sceSamp0IVECTOR[4]      :128bit���E�Ŕz�u����int�^�̔z��(�v�f4)
        sceSamp0FVECTOR[4]      :128bit���E�Ŕz�u����float�^�̔z��(�v�f4)
        sceSamp0FMATRIX[4][4]   :128bit���E�Ŕz�u����float�^�̔z��(�v�f4x4)

�@�֐��ɂ��Ă̏ڂ���������mathfunc.c�ɂ���܂��̂ł�������Q�Ƃ��Ă�����
���Bmathfunc.c�Ŏ��������֐��̂قƂ�ǂ�VU0�}�N���Ŏ�������Ă���Alibvu0
�ɂ��񋟂��Ă��܂��B
�@VU1�łł́A�s��̌v�Z��Core�Ŏ��s����sceSamp0***�֐���p���Ă��܂����AVU0
�œ��l�Ɋ֐���u��������΁A����ɍ����ȏ��������҂ł��܂��B
�@�Ȃ��ACore�ł�VU0��/VU1�łł͍s��̔z��`�����قȂ�܂��̂Œ��ӂ��Ă�����
���BCore�łł͈�ʓI�Ȕz��ł����AVU0��/VU1�łł�VU�̉��Z�@�\��L���ɗ��p��
�邽�߁A�]�n�`���ɂȂ��Ă��܂��B


�v���O��������i�����������j
---------------------------------------------------------------------------

�E�I�u�W�F�N�g�f�[�^�̓ǂݍ���
	VU1�łł́A�I�u�W�F�N�g�f�[�^�̓R���p�C���̎��_�Ń����N���ꃁ������
	�W�J����Ă��܂��D*.dsm�`���̃t�@�C����VU1�ɒ��ڑ����p�P�b�g�`��
	�ŁA���̂܂�My_dma_start�|�C���^���g���܂��B

�E�f�o�C�X�̏�����
    sceGsResetPath();//�f�[�^�p�X�f�o�C�X�̏�����(VIF1,VU1,GIF)

	�v���O�����̐擪�ŁA�g�p���邷�ׂẴf�o�C�X�̏��������s���܂��B
	���������s���Ă��Ȃ��f�o�C�X�𗘗p�����ꍇ�A���҂�����������Ȃ���
	�Ƃ�����܂��̂ŁA�K�����s���Ă��������B

�E�`����̐ݒ�
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
        //GS������������
    sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, 
             SCREEN_HEIGHT, SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);
        //�_�u���o�b�t�@�̐ݒ�������Ȃ�
    *(u_long *)&db.clear0.rgbaq = SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10,
                                                0x80, 0x3f800000);
    *(u_long *)&db.clear1.rgbaq = SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10,
                                                0x80, 0x3f800000);

	�e�f�o�C�X�̏��������I�������ɁAGS�̕`����̐ݒ���s���܂��B
	���̐ݒ���s��Ȃ��Ɖ�ʏ�ւ̕\�����ł��܂���B�܂��AVSync�œ���
	���Ƃ��Ă���R���g���[���̐ݒ���s���܂���̂ŁA�f�o�C�X�̏�������
	��ɂ��̐ݒ����x�͍s���悤�ɂ��Ă��������B

�E�e�N�X�`���p�̃C���[�W�]��
    sceGsSetDefLoadImage(&pTex,13340,IMAGE_SIZE / 64,SCE_GS_PSMCT32,
                        0, 0,IMAGE_SIZE, IMAGE_SIZE);
                        //LoadImage���̐ݒ�
    FlushCache(0);
    sceGsExecLoadImage(&pTex, (u_long128 *)My_texture1);
                        //LoadImage�̎��s
    sceGsSyncPath(0, 0);//�f�[�^�]���̏I���҂�

	�e�N�X�`���p�̃C���[�W��GS���[�J��������ɓ]�����邽�߂�LoadImage
	�̏���ݒ肵�܂��B���̌�AsceGsExecLoadImage()�����s���邱�ƂŁA
	�w�肳�ꂽ�C���[�W��GS�̃��[�J��������ɓ]������܂��B���̊֐��͓�
	����DMA�𗘗p���܂��̂ŁA�]���O��FlushCache(0)��D�L���b�V���ƃ��C
	��������Ԃ̐�����������Ă��܂��B�Ō�ɁAsceGsSyncPath()�Ńf�[�^
	�]���̏I���҂��������Ȃ��܂��B

�E�e�N�X�`���̊��ݒ�p�p�P�b�g�̐���
    FlushCache(0);
    // --- set texture env 1 ---
    sceGsSetDefTexEnv(&texenv.gs_tex, 0, 13340, 
                        IMAGE_SIZE / 64, SCE_GS_PSMCT32, 
                          8, 8, 0, 0, 0, 0, 1);
    SCE_GIF_CLEAR_TAG(&texenv.giftag); 
    texenv.giftag.NLOOP = 9;
    texenv.giftag.EOP = 1;
    texenv.giftag.NREG = 1;
    texenv.giftag.REGS0 = 0xe;
    sceGsSetDefAlphaEnv(&texenv.gs_alpha, 0);
    *(u_long *) &texenv.gs_alpha.alpha1 
                = SCE_GS_SET_ALPHA(0, 2, 0, 1, 0);
    *(u_long *) &texenv.gs_test 
                =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 3, 0x3);
    texenv.gs_test1addr = (long)SCE_GS_TEST_1;
    *(u_long*)&texenv.gs_tex.clamp1 
                = SCE_GS_SET_CLAMP(0, 0, 0, 0, 0, 0); 

	sceGsSetDefTexEnv()��p���āA�e�N�X�`���֘A��GS�ėp���W�X�^TEX0_1, 
	TEX0_1, CLAMP_1�̐ݒ���s���p�P�b�g���쐬���܂��B

�E�e�N�X�`���̊��ݒ�p�p�P�b�g�̓]��
    FlushCache(0);
    sceGsPutDrawEnv(&texenv.giftag);  //�e�N�X�`���ݒ�f�[�^�̓]��
    sceGsSyncPath(0, 0);

	�������ꂽ�e�N�X�`�����ݒ�p�̃p�P�b�g���AsceGsPutDrawEnv()�֐�
	��p���ē]�����Ă��܂��B

�E�R���g���[���̏�����
    fd = open("pad:0", 0);

	1P���̃R���g���[�����g�����߂̏��������s���܂��B����Ȍ�Aread()��
	�w�肵���o�b�t�@�Ƀf�[�^��ǂݍ��߂܂��B�R���g���[���̃f�[�^��
	VSync���ɍX�V����܂��B


�v���O��������i���C�����[�v�j
--------------------------------------------------------------------------
�@�ȉ��A���C�����[�v�̒��ōs����A�R���g���[���̏��擾�A�}�g���N�X�̐�
���A�p�P�b�g����(���W�ϊ��A�����ϊ��AUV->STQ�ϊ��A�����v�Z)�A�ɂ��Đ�����
�܂��B

�EView-Screen�}�g���N�X�̐���
    sceSamp0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
                2048.0f, 2048.0f,1.0f, 16777215.0f,1.0f,65536.0f);

	sceSamp0ViewScreenMatrix()��p����View-Screen�}�g���N�X�����߂܂��B
	�����ł́A���_���瓊�e�X�N���[���܂ł̋�����512�A��ʂ̃A�X�y�N�g
	���1:0.47�A��ʂ̒��S��GdPrimitive���W�n��(2048,2048)�AZ�o�b�t�@
	�̒l��1.0-16777215.0�AZ�̒l��1.0-65536.0�Ƃ���View-Screen�}�g���N
	�X�����߂Ă��܂��B

�E�R���g���[���̏��擾
    // --- read pad ---
    read(fd, &pad, sizeof(pad));

    // --- object rotate & change view point ---
    if(pad.button & SCE_PADLdown){ 
                obj_rot[0] += deltta;
                if(obj_rot[0]>PI) obj_rot[0] -= 2.0f*PI; }
    if(pad.button & SCE_PADLup){ 
                obj_rot[0] -= delta;
                if(obj_rot[0]<-PI) obj_rot[0] += 2.0f*PI; }
    if(pad.button & SCE_PADLright){ 
                obj_rot[1] += delta;
                if(obj_rot[1]>PI) obj_rot[1] -= 2.0f*PI; }
    if(pad.button & SCE_PADLleft){ 
                obj_rot[1] -= delta;
                if(obj_rot[1]<-PI) obj_rot[1] += 2.0f*PI; }
    if(pad.button & SCE_PADL1){
                obj_rot[2] += delta;
                if(obj_rot[2]>PI) obj_rot[2] -= 2.0f*PI; }
    if(pad.button & SCE_PADL2){
                obj_rot[2] -= delta;
                if(obj_rot[2]<-PI) obj_rot[2] += 2.0f*PI; }
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
                if(camera_rot[1]<PI) camera_rot[1] += 2.0f*PI; }
    if(pad.button & SCE_PADR1){
                camera_p[2] -= delta*5;
                if(camera_p[2]<-100) camera_p[2] = -100; }
    if(pad.button & SCE_PADR2){
                camera_p[2] += delta*5;
                if(camera_p[2]>-10) camera_p[2] = -10; }
    if(!toggle && (pad.button & SCE_PADselect)){
         obj_switch = (++obj_switch)&0x01; toggle = 1;
    }else if(!(pad.button & SCE_PADselect)){toggle = 0;}

	�R���g���[���̃{�^�����́Aread()��p���Ď擾�ł��܂��B
	�����ł͓���ꂽ�f�[�^�����ƂɁA�I�u�W�F�N�g�ƃJ�����̉�]�p�A�ړ�
	�ʂ��X�V���Ă��܂��B�R���g���[���̍��葤���I�u�W�F�N�g�A�E�葤�̃{
	�^�����J�����̑���ƂȂ��Ă��܂��B�܂��Aselect�{�^���ŃI�u�W�F�N�g
	�̐؂�ւ����s���܂��B

�ELocal-World�}�g���N�X(��]�̂�)�̐���
    sceSamp0UnitMatrix(work);                   //�P�ʍs��
    sceSamp0RotMatrix(local_world, work, rot);  //��]�s��

	Local-World�}�g���N�X�́ALocal���W�ŕێ�����Ă���I�u�W�F�N�g����
	���߂�K�v������܂��B��]�A���s�ړ��Ƃ������A�t�B���ϊ����p�����
	�܂��B���̃T���v���ł́A��]�}�g���N�X����ѕ��s�ړ��}�g���N�X���v
	�Z���āA��������Z�������̂�Local-World�}�g���N�X�Ƃ��ėp���Ă�
	�܂��B
	���������̃T���v���ł́A3�s�ڂ̕��s�ړ�(obj_trans)��ݒ肷��O��
	Local_Light�}�g���N�X��ݒ肷��K�v������܂��B���s�ړ�����������
	�s���p����Local_Light�}�g���N�X�𐶐�����ƁA�����̌v�Z������
	���s���܂���B����͕��s�ړ��A������������s��̑�4���p���Čv
	�Z���邽�߂ł��B���������ĉ�]�s���ݒ肵������ɁALocal_Light�}
	�g���N�X������Ă��������B
	�����ЂƂ��ӓ_�Ƃ��āAobj_trans�̑�4�v�f�ɂ͕K���[����ݒ肵�Ă�
	�������B�}�g���N�X��[4,4]�v�f�ɂ͔��ɃN���e�B�J���Ȓl���Z�b�g��
	��Ă���A�[���ȊO�̒l�͂����ς��Ă��܂����ƂɂȂ邽�߂ł��B

�E���[�J�����C�g�}�g���N�X�̐���
    sceSamp0NormalLightMatrix( normal_light, light0,light1,light2);
    sceSamp0MulMatrix(local_light, normal_light,local_world);

	sceSamp0NormalLightMatrix()��p���āA3�̌�������Normal-Light�}�g
	���N�X�����߂܂��B����ɁALocal-World�}�g���N�X�Ɗ|�����킹�āA
	Local-Light�}�g���N�X�𐶐����܂��B�O�q�����悤�ɁA���̂Ƃ��|����
        Local_World�}�g���N�X�́Aobj_trans���Z�b�g����Ă��Ă͂����܂���B

�E���C�g�J���[�}�g���N�X�̐���
    sceSamp0LightColorMatrix(light_color, color0, color1, color2, ambient);

	�����F�Ɗ��F����sceSamp0LightColorMatrix()��p����Light_Color�}
	�g���N�X�����߂܂��B�����v�Z���ɁA�e�����̉e���F�����܂�����ɁA
	Light_Color�}�g���b�N�X����Z���āA�ŏI�I�ȃX�N���[����ł̒��_�F
	�����肵�܂��B

�EWorld-View�}�g���b�N�X�̐���
    sceSamp0RotCameraMatrix(world_view, camera_p, camera_zd, camera_yd,
                                                         camera_rot);

	���̊֐�sceSamp0RotCameraMatrix()�́A������sceSamp0CameraMatrix()
	���Ăяo���Ă��܂��B����sceSamp0CameraMatrix()�p���āAWorld-View�}
	�g���N�X�����߂܂��BView���W�n�͎��_(eye)�����_(0,0,0)�Ƃ��A������
	����Z+�A�E�������X+�A��������Y+�ƂȂ��Ă��܂�(�E����W�n�j�B����
	�ł́AWorld�ɑ΂��Ď��_camera_p��(0,0,-25)�A����camera_zd��(0,0,1)�A
	��������camera_yd��(0,1,0)�Ƃ��A��������World-View�}�g���N�X��
	�������܂��B
        sceSamp0RotCameraMatrix()�֐������ł͂����̃x�N�g�������炩����
        camera_rot�ɂ���ĉ�]���A���̌��ʂ�sceSamp0CameraMatrix()�ɓn����
	�ƂŁA�J�����̉�]���������܂��B

�ELocal-World�}�g���N�X(��]�����s�ړ�)�̐���
    sceSamp0TransMatrix(local_world, local_world, obj_trans);
        //��]�����s�ړ�

	Local_Light�̃}�g���N�X�̐������I�������̂ŁALocal_World�}�g���N�X
	�ɕ��s�ړ��v�f(obj_trans)�������āA�^��Local_World�}�g���N�X�𐶐�
	���܂��B

�ELocal-Screen�}�g���N�X�̐���
    sceSamp0MulMatrix(world_screen, view_screen, world_view);
    sceSamp0MulMatrix(local_screen, world_screen, local_world);

	�����܂łɋ��߂�ꂽView-Screen�}�g���N�X�AWorld-View�}�g���N�X��
	�p���āA�܂�World-Screen�}�g���N�X�����߂܂��B�����Local-World�}
	�g���N�X���|�����킹�āALocal-Screen�}�g���N�X�����߂��܂��B
	���̎��_�Ŋ|�����킹��Local-World�}�g���N�X�́A��]�ƕ��s�ړ��̗v
	�f���������Z�b�g���ꂽ���̂ł���K�v������܂��B

�EVUMem1�ɓ]������p�P�b�g�ɒl���Z�b�g
    SetVu1PacketMatrix();
    if(obj_switch==0) *((&My_dma_next)+1) = (u_int)(&My_cube_start);
    else if(obj_switch==1) *((&My_dma_next)+1) = (u_int)(&My_torus_start);

	SetVu1PacketMatrix()�֐��ŁAVU1�ɓ]������}�g���N�X�ɒl���Z�b�g��
	�܂��B�܂��A�I�u�W�F�N�g��؂�ւ��邽�߁A�p�P�b�g����DMAtag�ɁA
	VU1�ɓ]�����ׂ��I�u�W�F�N�g�f�[�^�̃A�h���X���Z�b�g���Ă��܂��B

�EVUMem1�Ƀp�P�b�g��]��
    // --- kick dma to draw object ---
    *D1_QWC  = 0x00;
    *D1_TADR = (u_int)&My_dma_start & 0x0fffffff;
    *D_STAT = 2;
    FlushCache(0);
    *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
    sceGsSyncPath(0, 0);

	�����ł̓`���l��1��p���ăp�P�b�g��]�����邽�߁A���ڃ��W�X�^�ɒl
	���Z�b�g���Ă��܂��BDMAtag��]������̂ŁAD1_QWC�ɂ̓[�����Z�b�g���A
	D1_TADR�ɂ��p�P�b�g�]���A�h���X��My_dma_start�ɂ��܂��B
	D_STAT�ɂ��`���l��1�̃X�e�[�^�X�r�b�g���Z�b�g���A�]���O�ɃL���b
	�V�����t���b�V�����ă������̐����������킹�܂��B���̌�D1_CHCR�̃`
	���l�����䃌�W�X�^�ɒl���Z�b�g���āADMA�]�����X�^�[�g���܂��B
	���̂Ƃ�VU1�ɑ���f�[�^�ɂ́AVU1�}�C�N���R�[�h��VIF�R�[�h�A�I�u�W
	�F�N�g�f�[�^���܂܂�A�����͈�x��DMA�]���ŏ�������܂��B���̏�
	���������ɂ��ẮA��́uVU1�EVIF1�f�[�^�����v�ɂĉ�����܂��B

�E�`����̐؂�ւ��AV-Blank���Ԃ̏I���҂�
    sceGsSetHalfOffset((frame&1) ? &db.draw1 : &db.draw0, 2048,2048,odev);
    odev = !sceGsSyncV(0);

	�t�B�[���h���ɓ������؂�ւ������AV-Blank���Ԃ̊J�n��҂��܂��B
	����ȍ~�̓��[�v�̐擪�ɖ߂�A������Ƃ��J��Ԃ��܂��B


VU1�EVIF1�f�[�^����
--------------------------------------------------------------------------
�@VU1�ɑ���f�[�^�̗���ɂ��ĊȒP�ɐ������܂��B

	���C��RAM ---> DMA ---> VIF1 ---> MicroMem1 or VUMem1

�@�ȒP�ɂ͂��̂悤�ȗ���Ńf�[�^��������킯�ł����A���̍ەK�v�ȏ����ߒ�
�ɂ��ĉ�����܂��B

�@�܂�DMA����ăf�[�^��]�����邽�߁ADMA������s���܂��BDMA�����DMAtag��
���邱�Ƃōs���܂��B����DMAtag�̒ǉ��̓��C�����[�v��*.dsm�t�@�C���̓����Ő�
�����ǉ����Ă��܂�(*.dsm�t�@�C�����́uDMAcnt *�v�u.EndDmaData�v�Ȃ�)�B

    if(obj_switch==0) *((&My_dma_next)+1) = (u_int)(&My_cube_start);
    else if(obj_switch==1) *((&My_dma_next)+1) = (u_int)(&My_torus_start);

�@���Ƃ��΂��̏����ł́A�I�u�W�F�N�g�̐؂�ւ������̂��߁ADMAtag��next���w
�肵�Ă����ADMAtag���̃A�h���X��؂�ւ��邱�Ƃœ]���I�u�W�F�N�g����������
���܂��B

�@���̌�f�[�^��VIF1��ʉ߂��܂��B����VIF1�ɂ����l�ȋ@�\����������Ă���A
��������VU1�𐧌䂷�邱�Ƃ��\�ł��BVIF�𐧌䂷��R�[�h��VIFcode�Ŏ�����
��A�p�P�b�g�ɑg�ݍ��ނ��Ƃ��ł��܂��B�{�T���v���ł� *.dsm�t�@�C�����Ŏg�p
���Ă��܂��B

	��:

	MPG    :�}�C�N���v���O���������[�h(�f�[�^��uMem�ɑ���)
	-- data --
	.EndMPG

	unpack 4, 4, V4_32, 0, *    :�㑱�f�[�^��W�J�EVUMem�ɏ�������
	-- data --
	.EndUnpack

�@���̃T���v���ł́A�}�C�N���R�[�h��VIFcode�̋@�\�ɂ����,���̂悤�Ȏ菇��
�_�u���o�b�t�@�������������Ă��܂��B

	MSCAL 0       :�}�C�N���v���O�������s(�A�h���X0�X�^�[�g)
	BASE 0        :�_�u���o�b�t�@�̈���̐擪�A�h���X��0�Ɏw��B
                       (vif1 base ���W�X�^�ݒ�)
	OFFSET 512    :�_�u���o�b�t�@�̑����̐擪�A�h���X��512�Ɏw��
                       (vif1 offset ���W�X�^�ݒ�)

	unpack ......
	-- object data --
	.EndUnpack
	MSCNT     :�}�C�N���R�[�h�̎��s�ĊJ(�`�揈��)

�@�}�C�N���R�[�h�̏����̗�����ȒP�Ɏ����܂��B

        �O���� --+-> �����ϊ��E��������(GIF�p�P�b�g�����E�]��) 
                 |                              |
                 |               |              |
                 +<----<----<----|<----<----<---+
                         restart | stop
                               MSCNT
                          (switch buffer)

�@�}�C�N���R�[�h�͑O�����������͈��̃��[�v������������A����MSCNT���߂�
����܂őҋ@���Ă��܂��B�����āA���̃o�b�t�@�ɒl���Z�b�g����AMSCNT���߂�
���s�����Ǝ��s�ĊJ�i�������̓}�C�N���v���O�����̏�����~�܂�MSCNT���X�g
�[���j���A�f�[�^���������܂��B
�@MSCNT���߂����s�����Ƃ��A�I�t�Z�b�g�A�h���X���؂�ւ����邽�߁A����
���������X�C�b�`���ă_�u���o�b�t�@�������\�ƂȂ�܂��B


Basic3d VU1 �}�C�N���R�[�h
--------------------------------------------------------------------------

<����̊T��>
	1.���[���h���W�n�ł̍��W�ϊ� 
	2.�����ϊ� 
	3.���_�̖@���x�N�g���ƕ��s�����ɂ�郉�C�e�B���O 
	4.�p�[�X�y�N�e�B�u�R���N�g���ꂽ�e�N�X�`���}�b�s���O 


<VUMem1��̃f�[�^�z�u>

   �A�h���X     �X�g�A���ꂽ�f�[�^              ����
  ---------------------------------------------------------------------
     0 - 3      - �����ϊ��}�g���N�X              ���f���S�̂ŋ��p�����
     4 - 7      - ���W�ϊ��}�g���N�X              �}�g���N�X�f�[�^��
     8 - 11     - ���s�����x�N�g��
    12 - 15     - �����F
  ----------------------------------------------------------------------
    0 - 511     [�\�[�X�f�[�^]
                - GIFtag
                - ���_���W
                - �@���x�N�g��
                - ���_�F
                - �e�N�X�`�����W
                                                VU1�̃_�u���o�b�t�@�̈� 0
                [GIF��XGKICK�����p�P�b�g]
                - GIFtag
                - ST
                - RGBAQ
                - XYZF2
  -----------------------------------------------------------------------
   512 - 1023
                [�\�[�X�f�[�^]
                - GIFtag
                - ���_���W
                - �@���x�N�g��
                - ���_�F
                - �e�N�X�`�����W
                                                VU1�̃_�u���o�b�t�@�̈� 1
                [GIF��XGKICK�����p�P�b�g]
                - GIFtag
                - ST
                - RGBAQ
                - XYZF2

  ---------------------------------------------------------------------

<�}�C�N���R�[�h>

;
;
; vu1 micro code for basic sample
;
; [��ȃ��W�X�^�̈Ӗ�]
;
; VF01 - VF04 : �����ϊ��}�g���N�X x   ���[���h���W�n�ł̉�]�E�ړ��}�g���N�X
; VF05 - VF08 : ���s�����̕����x�N�g��
; VF09 - VF11 : �e�����̐F
; VF19 : gif tag 
; VF20 : �������钸�_���W
; VF21 : �������钸�_�ł̖@���x�N�g��
; VF22 : �������钸�_�̃e�N�X�`�����W
; VF23 : �������钸�_�̐F
;
;
; VI02 : VUMEM����GIF��XGKICK�ő�����f�[�^�̐擪�igif tag���u�����j
; VI03 : VUMEM��ɂ��鏈�����钸�_�f�[�^�ւ̃��[�h�|�C���^
; VI04 : VUMEM��ɂ��鏈������@���x�N�g���ւ̃��[�h�|�C���^
; VI05 : VUMEM��ɂ��鏈������e�N�X�`�����W�ւ̃��[�h�|�C���^
; VI06 : VUMEM��ɂ��鏈�����钸�_�̐F�ւ̃��[�h�|�C���^
; VI07 : GIF�ɑ���RGBAQ�f�[�^�ւ̃X�g�A�|�C���^
; VI08 : GIF�ɑ���ST�f�[�^�ւ̃X�g�A�|�C���^
; VI09 : GIF�ɑ���XYZF2�f�[�^�ւ̃X�g�A�|�C���^
;
; VI10 : �������钸�_���i���[�v�J�E���^�j
;
;
; ---------------------------------------------------------------------


NOP IADDIU VI01, VI00, 0
NOP IADDIU VI02, VI00, 4
NOP IADDIU VI03, VI00, 16 
NOP BAL VI15, MulMatrix ; �����ϊ��}�g���N�X x   ���[���h���W�n�ł̉�]�E�ړ��}�g���N�X���v�Z����
NOP NOP

NOP IADDIU VI01, VI00, 8 
NOP LQI VF09, (VI01++) ; ���s�����x�N�g���̃��[�h (8)
NOP LQI VF10, (VI01++)
NOP LQI VF11, (VI01++)
NOP LQI VF12, (VI01++)
NOP LQI VF05, (VI01++) ; �e�����̐F�̃��[�h (12) 
NOP LQI VF06, (VI01++)
NOP LQI VF07, (VI01++)
NOP LQI VF08, (VI01++)
NOP LQI VF01, (VI01++) ; �����ϊ��}�g���N�X x   ���[���h���W�n�ł̉�]�E�ړ��}�g���N�X�̃��[�h (16)
NOP LQI VF02, (VI01++)
NOP LQI VF03, (VI01++)
NOP LQI VF04, (VI01++)

NOP[E] NOP
NOP NOP
START0:
NOP XTOP VI01                 ; �_�u���o�b�t�@�����O�̂��߂�
                              ; VIF��TOP���W�X�^�̒l�ǂݏo��
NOP ILWR.x VI10, (VI01)x      ; GIFtag����nloop�̒l��ǂݏo��
NOP IADDIU VI11, VI00, 0x7fff ; ���[�v�J�E���^�i���_���j�̒l�𓾂邽�߂Ƀ}�X�N��ݒ�
NOP IAND VI10, VI11, VI10     ; �}�X�N���� VI10 �𒸓_���̓��������[�v�J�E���^�ɐݒ�
NOP LQ VF19, 0(VI01)          ; gif tag�̓ǂݍ���
NOP IADDIU VI03, VI01, 1      ; �ǂݍ��ޒ��_�f�[�^�ւ̃|�C���^
NOP IADD VI04, VI03, VI10     ; �ǂݍ��ޖ@���x�N�g���ւ̃|�C���^
NOP IADD VI05, VI04, VI10     ; �ǂݍ��ޒ��_�F�f�[�^�ւ̃|�C���^
NOP IADD VI06, VI05, VI10     ; �ǂݍ��ރe�N�X�`�����W�f�[�^�ւ̃|�C���^
NOP IADD VI02, VI06, VI10     ; XGKICK����ʒu�̃|�C���^
NOP SQ VF19, 0(VI02)          ; giftag���X�g�A
NOP IADDIU VI07, VI02, 2      ; �v�Z���ꂽRGBAQ���X�g�A����ʒu�ւ̃|�C���^
NOP IADDIU VI08, VI02, 1      ; �v�Z���ꂽST���X�g�A����ʒu�ւ̃|�C���^
NOP IADDIU VI09, VI02, 3      ; �v�Z���ꂽXYZF2���X�g�A����ʒu�ւ̃|�C���^

LOOP:
NOP LQI VF20, (VI03++)        ; ���_�f�[�^�̓ǂݍ���
NOP LQI VF21, (VI04++)        ; �@���x�N�g���̓ǂݍ���
NOP LQI VF22, (VI05++)        ; ���_�F�̓ǂݍ���
NOP LQI VF23, (VI06++)        ; �e�N�X�`�����W�̓ǂݍ���

; --- ���W�ϊ� &�����ϊ� ---
MULAx.xyzw ACC, VF01, VF20x NOP   ; ���W�ϊ��Ɠ����ϊ����s�� 
MADDAy.xyzw ACC, VF02, VF20y NOP 
MADDAz.xyzw ACC, VF03, VF20z NOP 
MADDw.xyzw VF25, VF04, VF20w NOP 
NOP NOP
NOP NOP
NOP NOP
NOP DIV Q, VF00w, VF25w          ; 1/w�����߂�

; --- ���s�����x�N�g���Ɩ@���x�N�g���Ƃ̓��� ---
MULAx.xyzw ACC, VF05, VF21x NOP   ; ����
MADDAy.xyzw ACC, VF06, VF21y NOP 
MADDAz.xyzw ACC, VF07, VF21z NOP 
MADDw.xyzw VF13, VF08, VF21w NOP 
NOP NOP
NOP NOP
NOP NOP
MAXx.xyzw VF13, VF13, VF00x NOP ; 0.0f�����̒l��0.0f�ɂ���

; --- 1/w �������āA�X�g�A���� ---
MULq VF28, VF25, Q NOP       ; 1/w ��������
NOP NOP
NOP NOP
NOP NOP
FTOI4 VF27, VF28 NOP         ; �Œ菬���_�ɕϊ�
NOP NOP
NOP NOP
NOP NOP
NOP SQ VF27, 0(VI09)         ; XYZF2���X�g�A(unpacked mode)
NOP IADDIU VI09, VI09, 3     ; �X�g�A�|�C���^���C���N�������g

; st��ǂݍ����Q�������ăX�g�A����(for perspective correction)
MULq.xyz VF27, VF23, Q NOP   ; ST��1/w�������AQ��1/w��ݒ肷��
NOP NOP
NOP NOP
NOP NOP
NOP SQ VF27, 0(VI08)         ; STQ���X�g�A����(unpacked mode)
NOP IADDIU VI08, VI08, 3     ; �X�g�A�|�C���^���C���N�������g����
NOP NOP
NOP NOP

; --- �Ɠx�v�Z���ꂽ�}�e���A���̐F�����߃X�g�A���� ---
MULAx.xyzw ACC, VF09, VF13x NOP    ; ���ςɌ����F��������
MADDAy.xyzw ACC, VF10, VF13y NOP
MADDAz.xyzw ACC, VF11, VF13z NOP
MADDw.xyzw VF14, VF12, VF13w NOP
NOP NOP
NOP NOP
NOP NOP
MUL.xyz VF27, VF22, VF14 NOP       ; �v�Z���ꂽ���C�g�̐F���}�e���A���F�ɂ�����
NOP NOP
NOP NOP
NOP NOP
FTOI0 VF26, VF27 NOP               ; �����ɕϊ� 
NOP NOP
NOP NOP
NOP NOP
NOP SQ VF26, 0(VI07)               ; RGBAQ���X�g�A����(unpacked mode)
NOP IADDIU VI07, VI07, 3           ; �X�g�A�|�C���^���C���N�������g����

; --- ���[�v ---
NOP IADDI VI10, VI10, -1           ; ���[�v�J�E���^���f�N�������g
NOP IBNE VI10, VI00, LOOP          ; ���[�v���J��Ԃ�

; --- �v�Z���ʂ�GIF�ɑ���(XGKICK)�A�}�C�N���̎��s���~ ---
NOP NOP 
NOP XGKICK VI02 ; XGKICK 
NOP[E] NOP      ; �}�C�N���̒�~
NOP NOP 
NOP B START0    ; ���̃}�C�N�����s�ʒu�փW�����v
NOP NOP


; -------------------------------------------------------------------
; MulMatrix �}�g���N�X�̊|���Z 
; VI01 : �\�[�X�}�g���N�X1�̃A�h���X
; VI02 : �\�[�X�}�g���N�X1�̃A�h���X
; VI03 : �v�Z���ꂽ�}�g���N�X���X�g�A����A�h���X
;

MulMatrix:
NOP LQI.xyzw VF08, (VI02++)
NOP LQI.xyzw VF04, (VI01++)
NOP LQI.xyzw VF05, (VI01++)
NOP LQI.xyzw VF06, (VI01++)
NOP LQI.xyzw VF07, (VI01++)
MULAx.xyzw ACC, VF04, VF08x LQI.xyzw VF09, (VI02++)
MADDAy.xyzw ACC, VF05, VF08y NOP
MADDAz.xyzw ACC, VF06, VF08z NOP
MADDw.xyzw VF12, VF07, VF08w NOP
MULAx.xyzw ACC, VF04, VF09x LQI.xyzw VF10, (VI02++)
MADDAy.xyzw ACC, VF05, VF09y NOP
MADDAz.xyzw ACC, VF06, VF09z NOP
MADDw.xyzw VF13, VF07, VF09w SQI.xyzw VF12, (VI03++)
MULAx.xyzw ACC, VF04, VF10x LQI.xyzw VF11, (VI02++)
MADDAy.xyzw ACC, VF05, VF10y NOP
MADDAz.xyzw ACC, VF06, VF10z NOP
MADDw.xyzw VF14, VF07, VF10w SQI.xyzw VF13, (VI03++)
MULAx.xyzw ACC, VF04, VF11x NOP
MADDAy.xyzw ACC, VF05, VF11y NOP
MADDAz.xyzw ACC, VF06, VF11z NOP
MADDw.xyzw VF15, VF07, VF11w SQI.xyzw VF14, (VI03++)
NOP NOP
NOP NOP
NOP NOP
NOP SQI.xyzw VF15, (VI03++)
NOP NOP
NOP JR VI15
NOP NOP


�p�P�b�g����
--------------------------------------------------------------------------

�@VU1�}�C�N���R�[�h��p���ē����ϊ��E���s�����v�Z���s�����߂�VU1�ɑ���p�P
�b�g�i.dsm�`���j�ɂ��Đ������܂��B
�@GIFtag��ݒ肷�邱�ƂŁA�Ɨ��O�p�`�A�g���C�A���O���X�g���b�v�Ƃ��Ɉ�����
�Ƃ��ł��܂��B�����̒��_��`�悷��ꍇ�ɂ́AGIFtag - MSCNT�܂ł̃p�P�b�g��
�����Ȃ�ׂē]�����܂��B����ɂ��VU1�̓_�u���o�b�t�@�������s���܂��B

 <��>�g���C�A���O���X�g���b�v�L���[�u------------------------------------
.include "vumacros.h"
.global My_dma_start
.global My_matrix
.global My_light_matrix
.global My_rot_trans_matrix

My_dma_start:
.align 0
DMAcnt *     ; �}�C�N���R�[�h���̂�VU1�ɑ���
MPG 0, *
.include "basic.vsm" 
.EndMPG
.EndDmaData

DMAcnt *
unpack 4, 4, V4_32, 0, *  ; �X�N���[���}�g���N�X
My_matrix:
fwzyx 0.000000, 0.000000, 0.000000, 35.752483
fwzyx 0.000000, 0.000000, -14.765776, 0.000000
fwzyx 0.050000, 4995000.000000, 102.400002, 102.400002
fwzyx 1.000000, 100000000.000000, 2048.000000, 2048.000000
.EndUnpack

unpack 4, 4, V4_32, 4, * ; ���[���h���W�n�ł̉�]�A�ړ��}�g���N�X
My_rot_trans_matrix:
fwzyx 0.0, -0.000000, -0.000000, 1.000000
fwzyx 0.0, -0.000000, 1.000000, 0.000000
fwzyx 0.0, 1.000000, 0.000000, 0.000000
fwzyx 1.0, 0.000000, 0.000000, 0.000000
.EndUnpack

unpack 4, 4, V4_32, 8, * ; �����F�i���s����x3, ����x1�j
fxyzw 1.0, 0.0, 0.0, 0.0 ; ���s�����P (R,G,B)=(1.0, 0.0, 0.0)
fxyzw 0.0, 0.0, 1.0, 0.0 ; ���s�����Q (R,G,B)=(1.0, 0.0, 0.0)
fxyzw 0.0, 0.0, 0.0, 0.0 ; ���s�����R (R,G,B)=(1.0, 0.0, 0.0)
fxyzw 0.2, 0.2, 0.2, 1.0 ; ���� (R,G,B) = (0.2, 0.2, 0.2)
.EndUnpack

unpack 4, 4, V4_32, 12, * ; ���s�����x�N�g��(x3)
My_light_matrix:
fxyzw 0.0, -1.0, 0.0, 0.000000 ; �e�񂪃x�N�g���������B���̗�ł� Light1=(0.0, 1.0, 0.0), 
fxyzw 1.0, 0.0, 0.0, 0.000000  ; Light2 = (-1.0, 0.0, 0.0), Light3=(0.0, 0.0, 0.0)
fxyzw 0.0, 0.0, 0.0, 0.000000  ; ��S��͊����̐ݒ�
fxyzw 0.0, 0.0, 0.0, 1.000000
.EndUnpack

MSCAL 0 ; �}�C�N���̎��s
BASE 0 ; vif1 base �̃x�[�X���W�X�^�̐ݒ� 
OFFSET 512 ; vif1 offset ���W�X�^�̐ݒ�
.EndDmaData


; ------------------------------------------------------------------------
; �`�悵�悤�Ƃ��郂�f���f�[�^�B
; �����ł͂P�̗����́i�g���C�A���O���X�g���b�v�j�B

DMAcnt *
unpack[r] 4, 4, V4_32, 0, *
iwzyx 0x00000000, 0x00000412, 0x300a4000, 0x0000800e ; giftag(texture����j
.EndUnpack
unpack[r] 4, 4, V4_32, 1, *
; �X�g���b�v���_�̃��[���h���W�n�ł̈ʒu
fxyzw -5.0, -5.0, 5.0, 1.0
fxyzw -5.0, -5.0, -5.0, 1.0
fxyzw -5.0, 5.0, 5.0, 1.0
fxyzw -5.0, 5.0, -5.0, 1.0
fxyzw 5.0, 5.0, -5.0, 1.0
fxyzw -5.0, -5.0, -5.0, 1.0
fxyzw 5.0, -5.0, -5.0, 1.0
fxyzw -5.0, -5.0, 5.0, 1.0
fxyzw 5.0, -5.0, 5.0, 1.0
fxyzw -5.0, 5.0, 5.0, 1.0
fxyzw 5.0, 5.0, 5.0, 1.0
fxyzw 5.0, 5.0, -5.0, 1.0
fxyzw 5.0, -5.0, 5.0, 1.0
fxyzw 5.0, -5.0, -5.0, 1.0

; �@���x�N�g��
fxyzw -1.0, 0.0, 0.0, 1.0
fxyzw -1.0, 0.0, 0.0, 1.0
fxyzw -1.0, 0.0, 0.0, 1.0
fxyzw -1.0, 0.0, 0.0, 1.0
fxyzw 0.0, 1.0, 0.0, 1.0
fxyzw 0.0, 0.0, -1.0, 1.0
fxyzw 0.0, 0.0, -1.0, 1.0
fxyzw 0.0, -1.0, 0.0, 1.0
fxyzw 0.0, -1.0, 0.0, 1.0
fxyzw 0.0, 0.0, 1.0, 1.0
fxyzw 0.0, 0.0, 1.0, 1.0
fxyzw 0.0, 1.0, 0.0, 1.0
fxyzw 1.0, 0.0, 0.0, 1.0
fxyzw 1.0, 0.0, 0.0, 1.0

; �F
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0

; �e�N�X�`�����W
fxyzw 0.0, 0.0, 1.0, 0.0
fxyzw 0.0, 1.0, 1.0, 0.0
fxyzw 1.0, 0.0, 1.0, 0.0
fxyzw 1.0, 1.0, 1.0, 0.0
fxyzw 0.0, 1.0, 1.0, 0.0
fxyzw 1.0, 0.0, 1.0, 0.0
fxyzw 0.0, 0.0, 1.0, 0.0
fxyzw 1.0, 1.0, 1.0, 0.0
fxyzw 0.0, 1.0, 1.0, 0.0
fxyzw 1.0, 0.0, 1.0, 0.0
fxyzw 0.0, 0.0, 1.0, 0.0
fxyzw 0.0, 1.0, 1.0, 0.0
fxyzw 1.0, 0.0, 1.0, 0.0
fxyzw 1.0, 1.0, 1.0, 0.0

.EndUnpack
MSCNT  ; �}�C�N���R�[�h�̎��s�ĊJ�i�`�揈���j
.EndDmaData

DMAend

