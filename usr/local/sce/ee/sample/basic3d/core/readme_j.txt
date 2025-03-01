[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�x�[�V�b�N�T���v�� basic3d Core��
=================================

<�T���v���̉��>
EE Core�݂̂�p���čs�񉉎Z�A�����ϊ������s�����w�֐����������A3D�O���t		�B�b�N�X�ɕK�v�Ȑ��w�̊�b�ɂ��ė�����[�߂܂��B�܂��A���̂悤�Ȑ��w��	����vu0,vu1�Ɏ�������ۂ̎菕���ƂȂ�悤�Abasic3d vu0�ł����vu1�łŗp		���Ă���֐��Ɠ����̊֐����������܂��B����ɁA���Z���ʂ𒼐�GS�ɓn���p�P	�b�g���쐬���邱�Ƃɂ��ADMA�̓���ƂƂ���GS�̓���̊�{�d�l�ɂ��Ċm		�F���܂��B

<�t�@�C��>
	core.c		���C���v���O����
	mathfunc.c	�����ϊ����̍s�񉉎Z�֐�(Core��)
	mathfunc.h	�����ϊ����̍s�񉉎Z�֐��̃w�b�_�t�@�C��
	cube.s		�I�u�W�F�N�g�f�[�^(������)
	torus1.s	�I�u�W�F�N�g�f�[�^(�g�[���X �f�[�^����)
	flower.dsm	�e�N�X�`���f�[�^

<�R���p�C�����@>
	% make

<�N�����@>
	% make run

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run core.elf

<�d�l>
        �e�N�X�`���t��Triangle Strip�̃L���[�u��\������B
        �e�N�X�`���t��Triangle�̃g�[���X��\������B
        (select�{�^���Ő؂�ւ�)

        Z�o�b�t�@��24bit
        ������3��
        �e�N�X�`����32bit

<�R���g���[���̑�����@>
	����(�����L�[) : �I�u�W�F�N�g��x����]
        ����	       : �I�u�W�F�N�g��y����]
        L1L2�{�^��     : �I�u�W�F�N�g��z����]
        �����{�^��     : ���[���h���W�n�ɑ΂���J������y����]
        ���~�{�^��     : ���[���h���W�n�ɑ΂���J������x����]
        R1R2�{�^��     : �J�����̑O�i��މ�]
        SELECT�{�^��   : �I�u�W�F�N�g�؂�ւ�

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
                     (CPUcore�W�I���g��)    (STQ, RGBA, XYZF)�z��)
                                                    |
                                                    |
GS����RAM(4M) <-------------------------------------+
 �t���[����Z�o�b�t�@��������         DMA�]��(DMAtag,GIFtag�t��)
 (�����_�����O)


�v���O�����̐���(�ڍ�)
--------------------------------------------------------------------------

�E�ϐ��^�\���́^�֐�(mathfunc.c�̊֐�������)

        SampleCubeDataHead[]     :�I�u�W�F�N�g�f�[�^(������)�ւ̃|�C���^
        SampleTorus1DataHead[]   :�I�u�W�F�N�g�f�[�^(�g�[���X)�ւ̃|�C���^
        My_texture1[]            :�e�N�X�`���f�[�^�ւ̃|�C���^

        TexEnv          :sceGsTexEnv�\���̂�]������p�P�b�g�쐬�̍\����
        QWdata          :128bit�f�[�^�������Ղ����邽�߂̋��p��
        GifPacket       :Gif(GS)�ɓ]������p�P�b�g�𐶐����邽�߂̍\����
        ObjData         :�������ɓW�J�����I�u�W�F�N�g�f�[�^���Ǘ�����\����

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

        scePadData      pad     :�p�b�h�f�[�^��ǂݍ��ލ\����
        sceGsDBuff      db      :�_�u���o�b�t�@���������邽�߂̍\����
        sceGsLoadImage  pTex    :�e�N�X�`����GS���[�J���������ɓ]�����邽
                                 �߂̍\����
        sceDmaChan      *dmaVif :DMA�ɂ��f�[�^�]�������邽�߂̍\����
        TexEnv          texenv  :�e�N�X�`���f�[�^��]�����邽�߂̃p�P�b�g
                                 ���쐬����\����

        LoadObj(ObjData *o, u_int *Head)
                        :���C��RAM�ɓW�J�����I�u�W�F�N�g�f�[�^��ObjData�\
                         ���̂ɓo�^���A���삷�邽�߂̊֐�

        ReleaseObj(ObjData *o)
                        :�o�^�����I�u�W�F�N�g�f�[�^���J�����邽�߂̊֐�

        MakePacket(ObjData *obj, int num)
                        :�I�u�W�F�N�g�f�[�^�ɑ΂��ē����ϊ��E�����v�Z�Ȃ�
                         ���s���AGS�ɓ]������p�P�b�g�𐶐�����֐�

�Emain �֐����̕ϐ�

        i, j            :for���[�v�Ȃǂŗp����J�E���^
        fd              :�p�b�h��open�����ۂɕԂ�l���󂯎��t�@�C���L�q�q
        fr              :�_�u���o�b�t�@�Ńt���b�v���s�����߂̃X�C�b�`
        delta           :��]�p��ݒ肷�邽�߂̕ϐ�
        obj[2]          :�I�u�W�F�N�g��o�^���邽�߂̕ϐ�(cube&torus)
        obj_switch      :�I�u�W�F�N�g��؂�ւ��邽�߂̃g�O���X�C�b�`
        toggle          :select�{�^���̓ǂݍ��݂��g�O���Ő��䂷��ϐ�

�Emathfunc.c �ŃT�|�[�g����ϐ�����ъ֐�

        sceSamp0IVECTOR[4]      :128bit���E�Ŕz�u����int�^�̔z��(�v�f4)
        sceSamp0FVECTOR[4]      :128bit���E�Ŕz�u����float�^�̔z��(�v�f4)
        sceSamp0FMATRIX[4][4]   :128bit���E�Ŕz�u����float�^�̔z��(�v�f4x4)

�@�֐��ɂ��Ă̏ڂ���������mathfunc.c�ɂ���܂��̂ł�������Q�Ƃ��Ă�����
���Bmathfunc.c�Ŏ��������֐��̂قƂ�ǂ�Vu0�}�N���Ŏ�������Alibvu0�ɂ��
�񋟂��Ă��܂��BCore�łŎ�������֐�����sceSamp0******�ł����AVU0�łł�
sceVu0******�ƂȂ��Ă��܂��B

�@�Ȃ��ACore�ł�VU0��/VU1�łł͍s��̔z��`�����قȂ�܂��̂Œ��ӂ��Ă�����
���BCore�łł͈�ʓI�Ȕz��ł����AVU0��/VU1�łł�VU�̉��Z�@�\��L���ɗ��p��
�邽�߁A�]�n�`���ɂȂ��Ă��܂��B


�v���O��������i�����������j
--------------------------------------------------------------------------

�E�I�u�W�F�N�g�f�[�^�̓ǂݍ���
    LoadObj(&obj[0], SampleCubeDataHead);
    LoadObj(&obj[1], SampleTorus1DataHead);

	�T���v���ŗ��p����I�u�W�F�N�g�f�[�^���ASampleCubeDataHead�����
	SampleCubeDataHead����ObjData�\���̂ɓǂݍ��݂܂��B

�E�f�o�C�X�̏�����
    sceGsResetPath();	//�f�[�^�p�X�f�o�C�X�̏�����(VIF1,VU1,GIF)
    sceDmaReset(1);	//DMA������������(DMA)

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
	���̐ݒ���s��Ȃ��Ɖ�ʏ�ւ̕\�����ł��܂���B�܂��AVSync�œ�����
	�Ƃ��Ă���R���g���[���̐ݒ���s���܂���̂ŁA�f�o�C�X�̏���������
	�ɂ��̐ݒ����x�͍s���悤�ɂ��Ă��������B

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

	sceGsSetDefTexEnv()��p���āA�e�N�X�`���֘A��GS�ėp���W�X�^TEX0_1,
	TEX0_1, CLAMP_1�̐ݒ���s���p�P�b�g���쐬���܂��B

�E�e�N�X�`���̊��ݒ�p�p�P�b�g�̓]��
    FlushCache(0);
    dmaVif = sceDmaGetChan(SCE_DMA_GIF);
    dmaVif->chcr.TTE = 1;
    sceDmaSendN(dmaVif, &texenv.giftag, texenv.size+1);
                        //�e�N�X�`���ݒ�f�[�^�̓]��
    sceGsSyncPath(0, 0);//�f�[�^�]���̏I���҂�

	�������ꂽ�e�N�X�`�����ݒ�p�̃p�P�b�g���APATH3�o�R��Normal DMA
	�����s���ē]�����܂��B

�E�R���g���[���̏�����
    fd = open("pad:0", 0);

	1P���̃R���g���[�����g�����߂̏��������s���܂��B�Ȍ��read()��p��
	�āA�w�肵���o�b�t�@�Ƀf�[�^��ǂݍ��߂܂��B�R���g���[���̃f�[�^��
	VSync���ɍX�V����܂��B


�v���O�����̉���i���C�����[�v�����j
--------------------------------------------------------------------------

�EView-Screen�}�g���N�X�̐���
    sceSamp0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
                2048.0f, 2048.0f,1.0f, 16777215.0f,1.0f,65536.0f);

	sceSamp0ViewScreenMatrix()��p����View-Screen�}�g���N�X�����߂܂��B
	���_���瓊�e�X�N���[���܂ł̋�����512�A��ʂ̃A�X�y�N�g���1:0.47�A
	��ʂ̒��S��GdPrimitive���W�n��(2048,2048)�AZ�o�b�t�@�̒l��1.0-
	16777215.0�AZ�̒l��1.0-65536.0�Ƃ���View-Screen�}�g���N�X�����߂�
	���܂��B
        Core�łł̓��C�����[�v�̊O�ŏ��������Ă��܂��B���s���ɂ��̃}�g���N
	�X��ύX���邱�Ƃ͊�Ȃ��߂ł��B

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
	�����ł͓���ꂽ�f�[�^�����ƂɁA�I�u�W�F�N�g�A�J�����o���̉�]�p�A
	�ړ��ʂ��X�V���Ă��܂��B�R���g���[���̍��葤���I�u�W�F�N�g�A�E�葤
	�̃{�^�����J�����̑���ƂȂ��Ă��܂��B�܂��Aselect�{�^�����I�u�W�F
	�N�g�̐؂�ւ��ł��B

�ELocal-World�}�g���b�N�X(��]�̂�)�̐���
    sceSamp0UnitMatrix(work);                   //�P�ʍs��
    sceSamp0RotMatrix(local_world, work, rot);  //��]�s��

	Local-World�}�g���N�X�́ALocal���W�ŕێ�����Ă���I�u�W�F�N�g����
	�ɋ��߂�K�v������܂��B��]�A���s�ړ��Ƃ������A�t�B���ϊ����p����
	��܂��B
	���̃T���v���ł́A��]�}�g���b�N�X����ѕ��s�ړ��}�g���N�X���v�Z��
	�āA��������Z�������̂�Local-World�}�g���b�N�X�Ƃ��ėp���Ă���
	���B
	���������̃T���v���ł́A3�s�ڂ̕��s�ړ�(obj_trans)��ݒ肷��O��
	Local_Light�}�g���N�X��ݒ肷��K�v������܂��B���s�ړ�����������
	�s���p����Local_Light�}�g���N�X�𐶐�����ƁA�����̌v�Z������
	���s���܂���B����͕��s�ړ��A������������s��̑�4���p���Čv
	�Z���邽�߂ł��B���������ĉ�]�s���ݒ肵������ɁALocal_Light�}
	�g���N�X������Ă��������B

	�����ЂƂ��ӓ_�Ƃ��āAobj_trans�̑�4�v�f�ɂ͕K���[����ݒ肵�Ă�
	�������B�}�g���N�X��[4,4]�v�f�ɂ͔��ɃN���e�B�J���Ȓl���Z�b�g��
	��Ă��āA�[���ȊO�̒l�ł͂����ς��Ă��܂����ƂɂȂ邽�߂ł��B

�E���[�J�����C�g�}�g���b�N�X�̐���
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

�ELocal-World�}�g���b�N�X(��]�����s�ړ�)�̐���
    sceSamp0TransMatrix(local_world, local_world, obj_trans);
        //��]�����s�ړ�

	Local_Light�̃}�g���N�X�̐������I�������̂ŁALocal_World�}�g���N�X
	�ɕ��s�ړ��v�f(obj_trans)�������āA�^��Local_World�}�g���N�X�𐶐�
	���܂��B

�ELocal-Screen�}�g���b�N�X�̐���
    sceSamp0MulMatrix(world_screen, view_screen, world_view);
    sceSamp0MulMatrix(local_screen, world_screen, local_world);

	�����܂łɋ��߂�ꂽView-Screen�}�g���N�X�AWorld-View�}�g���N�X��
	�p���āA�܂�World-Screen�}�g���N�X�����߂܂��B�����Local-World�}
	�g���N�X���|�����킹�邱�ƂŁALocal-Screen�}�g���b�N�X�����߂���
	���B
	���̎��_�Ŋ|�����킹��Local_World�}�g���N�X�́A��]�ƕ��s�ړ��̗v
	�f���������Z�b�g���ꂽ���̂ł���K�v������܂��B

�E�p�P�b�g����
    MakePacket(&obj[obj_switch], i);

	�p�P�b�g�̐����́AMakePacket()�̓����ōs���܂��B����obj�́A�\��
	����I�u�W�F�N�g��ObjData�\���̂ւ̃|�C���^�ł��B�����؂�ւ���
	���ƂŊȒP�ɕ\���I�u�W�F�N�g��ύX���邱�Ƃ��ł��܂��B

�E�p�P�b�g�����ݒ�@�iMakePacket()�j
    pack->size = 0;
    pack->buf = (QWdata *)0x70000000;//SPR

	�p�P�b�g�̏������݃A�h���X��0x70000000�Ɏw�肵�Ă��܂��B����̓X�N
	���b�`�p�b�h�̈�̐擪�ł��̂ŁA�p�P�b�g�̑傫�����X�N���b�`�p�b�h
	�T�C�Y��16KB���z���Ȃ��悤�ɒ��ӂ��Ă��������B���C���������Ƀp�P�b
	�g�𐶐�����ꍇ�ł��ADMA�̓]������QWC�ɐݒ�ł���l�ɐ����������
	���̂ŁA�p�P�b�g�̃T�C�Y��1MB�ȏ�ɂȂ�Ȃ��悤�ɒ��ӂ��Ă��������B
	���̃T���v���ɂ�����g�[���X�f�[�^�́AVU1�ł�VUMem1�̃_�u���o�b�t
	�@���g�p���邽�߁A���̗e�ʂ𒴂��Ȃ��悤��4KB���x�̃u���b�N�T�C�Y
	�ɕ�������Ă��܂��B

�EDMAtag�AGIFtag�̐����@�iMakePacket()�j
    //DMAtag
    pack->buf[pack->size].ul128 = (u_long128)0;
    pack->buf[pack->size++].ui32[0] = 0x70000000|(obj->vertexNum[num]*3+1);
    //GIFtag
    pack->buf[pack->size].ul64[0]
        = SCE_GIF_SET_TAG(obj->vertexNum[num], 1, 1,obj->prim,
                                                SCE_GIF_PACKED,3);
    pack->buf[pack->size++].ul64[1] = 0x0412L;

	�p�P�b�g�擪�ɂ́ADMAtag�����GIFtag������K�v������܂��B
	DMA�́ASource Chain mode�𗘗p���Ă��܂��B���M����p�P�b�g�̃T�C�Y
	�͒��_��x3(STQ,RGBA,XYZF2)+1(GIFtag)�ADMAtag��ID��end(0x70000000)
	�ł��iDMA�p�P�b�g���ЂƂ����Ȃ̂Łj�B
	GIF��PACKED mode(SCE_GIF_PACKED)���g�p���A�v���~�e�B�u�̐ݒ��
	GIFtag�̒��ōs���悤�ɂ��Ă��܂�(obj->prim)�BREGS�̒l��0x412(STQ,
	RGBA,XYZF2)�ƂȂ��Ă��܂��B

�E�����ϊ��@�iMakePacket()�j
    Q = sceSamp0RotTransPers(v01, local_screen, vertex[j], 1);
    sceSamp0ScaleVector(tex, texUV[j], Q); //for Perspective correction

	sceSamp0RotTransPers()�֐��͓����ϊ����s���֐��ł��B�ڂ���������
	mathfunc.c���ɏ����Ă���܂��B�o�͌���(v01)��int�^�z��ŁAGS�ɒ���
	�n����`��(GS�̍��W�n)�ɂȂ��Ă��܂��B�܂��A�Ԃ�lQ��v01�̑�4�v�f
	�̋t���ŁAfloat�^�ł��B����̓p�[�X�y�N�e�B�u�␳�ɗp������̂ŁA
	�e�N�X�`�����W(S, T, Q)�ɗp���܂��B
        texUV�̓e�N�X�`����UV���W(U, V, 1.0 ,0)���Z�b�g����Ă��܂��B����
	��̒l��Q���|���邱�Ƃ�STQ�e�N�X�`�����W�����܂�܂��B

�E�����v�Z�@�iMakePacket()�j
    sceSamp0NormalColorVector(c01, local_light, light_color,
                         normal[j], color[j]);

	���_�̖@���x�N�g���E�F���ƁA���C�g�̃x�N�g���E�F��񂩂�����v�Z
	��̒��_�̐F�����肵�܂��B�����GS�ɒ��ړn����`���Œl���A���Ă���
	���B
	�֐������ł́A���C�g�̃x�N�g���ƒ��_�̖@���x�N�g���Ƃ̓��ς����߂�
	���_�ŁA��x���ϒl��0.0�`1.0�̊ԂŃN���b�s���O���܂��B����ɂR��
	�����̉e���𑫂����킹����A�����0.0�`255.0�̊ԂŃN���b�s���O���s
	���Ă��܂��B

�E�p�P�b�g�ւ̊i�[�@�iMakePacket()�j
    pack->buf[pack->size++].ul128 = *((u_long128*)tex);
    pack->buf[pack->size++].ul128 = *((u_long128*)c01);
    pack->buf[pack->size++].ul128 = *((u_long128*)v01);

	���߂�ꂽ�e�l��(S,T,Q) (R,G,B,A) (X,Y,Z,F) �̏��Ńp�P�b�g�ɋl�ߍ�
	�݂܂��B

�EGS�ւ̓]��
    MakePacket(&obj, i);
    FlushCache(0);
    sceDmaSend(dmaVif, (u_long128*)((u_int)obj.pack[i].buf|0x80000000));
    sceGsSyncPath(0, 0);

	MakePacket()�Ő������ꂽ�v���~�e�B�u�̃p�P�b�g���APATH3�o�R��
	Source Chain DMA�����s���ē]�����܂��B
	��x�ɍ���p�P�b�g�T�C�Y�ɐ��������邽�߁A������I�u�W�F�N�g�̕�
	���u���b�N�̌��������J��Ԃ��܂��B

�E�`����̐؂�ւ��AV-Blank�҂�
    if(fr&0x01)
        sceGsSetHalfOffset(&db.draw1,2048,2048,sceGsSyncV(0)^0x01);
    else
        sceGsSetHalfOffset(&db.draw0,2048,2048,sceGsSyncV(0)^0x01);

	�t�B�[���h���ɓ������؂�ւ�����AV-Blank���Ԃ̊J�n��҂��܂��B

