/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         Version 0.10
 *                           Shift-JIS
 *
 *                        All Rights Reserved.
 *
 *                            main.c
 *                     main function of memory card access
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.10            Jan,14,1999     yutaka    
 *      1.00            Dec, 6,1999     T.Honda
 */


#include <eekernel.h>
#include <string.h>
#include <stdlib.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libvu0.h>
#include <libdma.h>
#include <libpkt.h>
#include <libdev.h>
#include <sifrpc.h>
#include <libpad.h>
#include <libmc.h>
#include <libmtap.h>


/* �}���`�^�b�v���g�p����ꍇ�͂��̒�`���P�ɂ��܂� */
#define UseMultiTap	0

#define WORKSIZE	(0x80)
#define WORKBASE	0x70000000
#define SUPPER		(WORKBASE+WORKSIZE)
#define SLOWER		(WORKBASE+WORKSIZE)

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	224
#define OFFX		(((4096-SCREEN_WIDTH)/2)<<4)
#define OFFY		(((4096-SCREEN_HEIGHT)/2)<<4)
#define WALL_X		((SCREEN_WIDTH-16)<<4)	/* reflection point */
#define WALL_Y		((SCREEN_HEIGHT-8)<<4)


void sceDevConsLocate(int cd, u_int lx, u_int ly);
void sceDevConsPut(int cd, u_char c, u_char a);
void sceDevConsAttribute(int cd, u_char col);
void sceDevConsClearBox(int cd, int x, int y, u_int w, u_int h);
void sceDevConsSetColor(int cd, u_char c, u_char r, u_char g, u_char b);

#define MaxExecTitle	6
#define MaxVertTitle	3
#define LocDispStatus	68
#define SizeConsX	78
#define MaxDirView	16

#define BuffSizeRw	1024

typedef struct {
	unsigned char *Title;
	int (*Func)();
} Menu;

sceGsDBuff db;
int console;
int frame;
int openid = -1;
unsigned char openname[1024] = { 0 };
unsigned char rwbuff[BuffSizeRw];

/* �������[�J�[�h�̃A�N�Z�X�ΏۂƂȂ�|�[�g�A�X���b�g�ԍ� */
int mcport = 0, mcslot = 0;

int pdport = 0, pdslot = 0;


void BUG_WALKAROUND(sceVif1Packet *);
void dispTitle(int, int);
void dispError(int);
int inpstr(unsigned char *, int, int, int, int);
unsigned atox(unsigned char *);
int isdigit(unsigned char);
int isxdigit(unsigned char);
unsigned char toupper(unsigned char);


int menuMoveFunc(int, int);
int mnIdleFunc(int, int);
int mnGetDir(int, int);
int mnDefaultEnt(int, int);
int mnFileInfo(int, int);
int mnFormat(int, int);
int mnUnFormat(int, int);
int mnRename(int, int);
void remapColorLut();
void loadIopModule();


Menu menu[MaxExecTitle] = {
	{ "get dir",		mnGetDir },
	{ "mkdir + icon",	mnDefaultEnt },
	{ "file status",	mnFileInfo },
	{ "format",		mnFormat },
	{ "un-format",		mnUnFormat },
	{ "rename",		mnRename }
};

u_long128 padwork[scePadDmaBufferMax] __attribute__ ((aligned(64)));

int main(int argc, char* argv[]){
	sceVif1Packet packet[2];	/* DMA�p�P�b�g�n���h�� */
	sceDmaEnv env;			/* DMA ���ʊ� */
	sceDmaChan *p1;
	int sindex;
	u_long giftag[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 1, 12),
			 0x0000f43434343610L };	/* �X�v���C�g�pGIF�^�O */
	u_long giftagAD[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1),
			 0x000000000000000eL };	/* A�{D�pGIF�^�O */

	unsigned char padbuff[34];
	int ret, oldbtn, curbtn;
	int phase, func, exec;
	int count;


	loadIopModule();

	sceDevVif0Reset();
	sceDevVu0Reset();
	sceGsResetPath();
	sceDmaReset(1);

	*T0_MODE = T_MODE_CLKS_M | T_MODE_CUE_M; /* Hcount Timer Setting */
	sindex = 0;

	sceVif1PkInit(&packet[0], (u_long128 *)SUPPER);
	sceVif1PkInit(&packet[1], (u_long128 *)SLOWER);

	sceDmaGetEnv(&env);		/* DMA���ʊ��擾 */
	env.notify = 1<<SCE_DMA_VIF1; /* notify channel */
	sceDmaPutEnv(&env);

	/* DMA VIF�P�`�����l���f�X�N���v�^�擾 */
	p1 = sceDmaGetChan(SCE_DMA_VIF1);
	p1->chcr.TTE = 1;

	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
	sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT,
		SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);

	db.clear0.rgbaq.R = 0x0;	/* clear color for dbuf0 */
	db.clear0.rgbaq.G = 0x80;
	db.clear0.rgbaq.B = 0x80;

	db.clear1.rgbaq.R = 0x0;	/* clear color for dbuf1 */
	db.clear1.rgbaq.G = 0x80;
	db.clear1.rgbaq.B = 0x80;

	FlushCache(0);

	sceVif1PkReset(&packet[sindex]);
	sceVif1PkCnt(&packet[sindex], 0);
	sceVif1PkOpenDirectCode(&packet[sindex], 0);
	sceVif1PkOpenGifTag(&packet[sindex], *(u_long128*)&giftagAD);

	/* add alpha environment packet */
	sceVif1PkReserve(&packet[sindex],
		sceGsSetDefAlphaEnv((sceGsAlphaEnv *)packet[sindex].pCurrent,0)
		* 4);
	sceVif1PkCloseGifTag(&packet[sindex]);
	sceVif1PkCloseDirectCode(&packet[sindex]);
	sceVif1PkEnd(&packet[sindex], 0);
	sceVif1PkTerminate(&packet[sindex]);

	/* kick Gs initialize packet */
	sceDmaSend(p1,(u_int *)
		(((u_int)packet[sindex].pBase&0x0fffffff)|0x80000000));

	/* wait for Gs initialize packet end */
	sceGsSyncPath(0,0);

	/* make preset packet (double buffer on scratch pad ram) */
	for(sindex=0;sindex<2;sindex++) {
		sceVif1PkReset(&packet[sindex]);

		sceVif1PkCnt(&packet[sindex], 0);
		sceVif1PkOpenDirectCode(&packet[sindex], 0);
		sceVif1PkOpenGifTag(&packet[sindex], *(u_long128*)&giftag);

		sceVif1PkCloseGifTag(&packet[sindex]);
		sceVif1PkCloseDirectCode(&packet[sindex]);

		/* SCRATCH IN/OUT DMA BUG ��� */
		BUG_WALKAROUND(&packet[sindex]);
		sceVif1PkEnd(&packet[sindex], 0);
		sceVif1PkTerminate(&packet[sindex]);
	}

	sindex = 0;		/* scrach packet double buffer index */
	while(!sceGsSyncV(0));	/* display next in odd field when interlace*/

	sceMcInit();

#if UseMultiTap
	/* �}���`�^�b�v�h���C�o�̏�����  */
	ret = sceMtapInit();
	printf( "sceMtapInit : %d\n", ret );

	/* �e�|�[�g�ł̃}���`�^�b�v�g�p���\�ɂ��� */
	ret = sceMtapPortOpen(0);	/* �R���g���[���|�[�g0 */
	printf("MtapPortOpen for controller port 0 %d\n", ret );

	ret = sceMtapPortOpen(2);	/* �������[�J�[�h�|�[�g0 */
	printf("MtapPortOpen for MC port 0 %d\n", ret );
	ret = sceMtapPortOpen(3);	/* �������[�J�[�h�|�[�g1 */
	printf("MtapPortOpen for MC port 1 %d\n", ret );

	/* �}���`�^�b�v�̗L���𒲂ׂ� */
	ret = sceMtapGetConnection(2);
	printf("multi tap connection for MC port 0: %d\n", ret );
	ret = sceMtapGetConnection(3);
	printf("multi tap connection for MC port 1: %d\n", ret );

	sceGsSyncV( 0 );

	/* �e�|�[�g�Ŏg�p�\�ȃX���b�g���𒲂ׂ�B*/
	for(count = 0; count < 2; ++count) {
		ret = sceMcGetSlotMax(count);
		printf("max slot for MC port %d = %d\n", count, ret );
	}
#endif

	scePadInit(0);
	ret = scePadPortOpen(pdport, pdslot, padwork);

	sceDevConsInit();
	console = sceDevConsOpen(OFFX + (8<<4), OFFY + (8<<4), SizeConsX,30);

	remapColorLut();

	frame = 0;		/* frame double buffer index */
	phase = 0;
	func = 0;
	exec = 0;
	oldbtn = 0;
	padbuff[0] = 1;

	sceDevConsClearBox(console, 0,0,SizeConsX,30);
	dispTitle(console, 0);

	while(1) {
		sceGsSwapDBuff(&db, frame);
		++frame;

		ret = scePadRead(pdport, pdslot, padbuff);
		if(ret && !padbuff[0]) {
			curbtn = 0xFFFF ^((padbuff[2]<<8)|padbuff[3]);
		}
		else {
			curbtn = oldbtn;
		}

		if(!exec) {
			if((curbtn ^ oldbtn) & curbtn) {
				func = menuMoveFunc(func,
					(curbtn ^ oldbtn) & curbtn);
				dispTitle(console, func);
			}
			if((curbtn ^ oldbtn) & curbtn & SCE_PADstart) {
				/* �֐��� �X�^�[�g���[�h�ŌĂяo�� */
				dispTitle(console, ~func);
				sceDevConsClearBox(console, 0,MaxVertTitle+1,
					SizeConsX,30- MaxVertTitle);
				exec = (*(menu[func].Func))(0, curbtn);
				if(!exec) {
					dispTitle(console, func);
				}
			}			
			else {
				/* �A�C�h�����O���̂P�b�ɂP��̓J�[�h�|�[�g��
				   �X�e�[�^�X���`�F�b�N���ɍs�� */
				if( !(frame % 60) ) {
					func = ~func;
					exec = mnIdleFunc(0, curbtn);
					if(!exec) {
						func = ~func;
					}
				}
			}
		}
		/* �������[�J�[�h�ɃA�N�Z�X���̂Ƃ� */
		else {
			if(func >=0) {
				exec = (*(menu[func].Func))(1, curbtn);
				if(!exec) {
					dispTitle(console, func);
				}
			}
			else {
				exec = mnIdleFunc(1, curbtn);
				/* �A�C�h�����O�`�F�b�N���I�����猳�ɖ߂� */
				if(!exec) {
					func = ~func;
				}
			}
		}

		sceDevConsDraw(console);

		count = *T0_COUNT;
		oldbtn = curbtn;

		if(frame&0x01) {	/* interrace half pixcel adjust */
			sceGsSetHalfOffset(&db.draw1,2048,2048,
			sceGsSyncV(0)^0x01);
		}
		else {
			sceGsSetHalfOffset(&db.draw0,2048,2048,
			sceGsSyncV(0)^0x01);
		}
		FlushCache(0);
		sceGsSyncPath(0,0);
	}

	return 0;
}



/* MEM->SCRATCH,SCRATCH->MEM��DMA�𓯎��ɃL�b�N����ꍇ�� SCRATCH->MEM��
   DMA�̂P�p�P�b�g�̃T�C�Y���K���S�̔{��QWORD�ɂȂ�悤�ɒ������� */
void BUG_WALKAROUND(sceVif1Packet *pkt) {
	int dma_size,i;

	dma_size = ((u_int)pkt->pCurrent - (u_int)pkt->pBase - 0x10)/0x10;

	for(i=0;i<dma_size%4;i++) {
		sceVif1PkAddCode(pkt,SCE_VIF1_SET_NOP(0));
		sceVif1PkAddCode(pkt,SCE_VIF1_SET_NOP(0));
		sceVif1PkAddCode(pkt,SCE_VIF1_SET_NOP(0));
		sceVif1PkAddCode(pkt,SCE_VIF1_SET_NOP(0));
	}
}



void dispTitle(int console, int select) {
	int count;

	for(count=0; count< MaxExecTitle; ++count) {
		if(count== select) {
			sceDevConsAttribute(console, 4);
		}
		if( count == ~select) {
			sceDevConsAttribute(console, 5);
		}
		sceDevConsLocate(console,
			count/MaxVertTitle*16, count% MaxVertTitle);
		sceDevConsPrintf(console, "%2d.%s",count+1, menu[count].Title);

		sceDevConsAttribute(console, 7);
	}
	sceDevConsLocate(console, 0, MaxVertTitle);
	sceDevConsPrintf(console, "------------------------------------------------------------");
}



void dispError(int ret) {

	sceDevConsAttribute(console, 6);
	switch(ret) {
	case -2:
		sceDevConsMessage(console,15,10," No Format ");
		break;
	case -3:
		sceDevConsMessage(console,15,10," Memory Card Full ");
		break;
	case -4:
		sceDevConsMessage(console,15,10," Duplicated Entry ");
		break;
	case -5:
		sceDevConsMessage(console,15,10," Closed file handler ");
		break;
	default:
		sceDevConsMessage(console, 15,10, "No Card");
	}
	sceDevConsAttribute(console, 7);
}


int menuMoveFunc(int cur, int button) {

	if(button & SCE_PADLup) {
		if(cur > 0) {
			--cur;
		}
	}
	else if(button & SCE_PADLdown) {
		if(cur < (MaxExecTitle -1)) {
			++cur;
		}
	}
	else if(button & SCE_PADLleft) {
		if(cur >= MaxVertTitle) {
			cur -= MaxVertTitle;
		}
	}
	else if(button & SCE_PADLright) {
		if( (cur+ MaxVertTitle) < MaxExecTitle) {
			cur += MaxVertTitle;
		}
	}
	return cur;
}



int mnIdleFunc(int mode, int btn) {
	static int type, free, format, result, called;
	unsigned char *str;

	if(!mode || !called) {
		called = !sceMcGetInfo(mcport, mcslot, &type, &free, &format);
	}
	else {
		if(!sceMcSync(1, 0, &result)) {
			return 1;
		}

		called = 0;
		if(format) {
			str = "Formatted";
		}
		else {
			str = "No Format";
		}

		switch(result) {
		case 0:
			break;
		case -1:
		case -2:
			str = "Changed";
			break;
		default:
			str = "No Card";
			break;
		}
		sceDevConsLocate(console, LocDispStatus,0);
		sceDevConsPrintf(console, "%-10s", str);

		if(result >= -2) {
			sceDevConsLocate(console, LocDispStatus,1);
			sceDevConsPrintf(console, "type %4s",
				type!=2 ? "128K":"8M");

			sceDevConsLocate(console, LocDispStatus,2);
			sceDevConsPrintf(console, "free %4d", free);
		}
		else {
			sceDevConsLocate(console, LocDispStatus,1);
			sceDevConsPrintf(console, "         ");
			sceDevConsLocate(console, LocDispStatus,2);
			sceDevConsPrintf(console, "         ");
		}
		return 0;
	}
	return 1;
}


/* #mnGetDir --- �f�B���N�g���ꗗ�擾 */
int mnGetDir(int mode, int btn) {
	static sceMcTblGetDir table[99];
	static unsigned char path[1024];
	static int oldbtn=0;
	static int phase, ent, view, line;
	int ret, count, curbtn;
	static unsigned char result[80];

	/* ���������[�h */
	if(!mode) {
		oldbtn = btn;
		phase = 0;
		ent = 0;
		view = line = 0;
		*result = 0;
	}

	curbtn = btn & (oldbtn ^ btn);		/* up-edge ���������o */
	oldbtn = btn;

	ret = 0;

	/* select �{�^���������ꂽ�� get dir ���j���[�𔲂��� */
	if(curbtn & SCE_PADselect) {
		phase = 2;
		ret = -1;
		line = -1;
	}

	switch(phase) {
	case 0:
		/* �f�B���N�g���ꗗ�擾���� �o�^�^�J�n */
		ret = sceMcGetDir(mcport, mcslot, "*", 0, 99, table);
		/* ����������҂� */
		sceMcSync(0, 0, &ret);
		if(ret >=0) {
			ent = ret;
			++phase;
			view = line = 0;
			sceDevConsLocate(console, 0,MaxVertTitle+MaxDirView+5);
			sceDevConsPrintf(console, "total %d files found ",ent);
		}
		break;
	case 1:
		/* �J�����g�f�B���N�g�������擾 */
		ret = sceMcChdir(mcport, mcslot, ".", path);
		/* ����������҂� */
		sceMcSync(0, 0, &ret);
		if(ret) {
			break;
		}

		/* �G���g�����i�[�̈�̋󂫌��𒲂ׂ�  */
		ret = sceMcGetEntSpace(mcport, mcslot, ".");
		sceMcSync(0, 0, &ret);

		/* �J�����g�f�B���N�g�����\�� */
		if(ret >= 0) {
			++phase;
			sceDevConsClearBox(console, 0, MaxVertTitle+2, 79, 1);
			sceDevConsLocate(console, 0, MaxVertTitle +2);
			sceDevConsPrintf(console, "dir(%d) - %s", ret, path);
		}
		/* file status �\���̑Ώۂɂ���t�@�C���^�f�B���N�g����
		   �w�肳��Ă���Ε\�� */
		if(openid >= 0) {
			sceDevConsClearBox(console, 0, MaxVertTitle+1, 79, 1);
			sceDevConsLocate(console, 0, MaxVertTitle +1);
			sceDevConsPrintf(console, "cur file : %s", openname);
		}
		break;
	case 3:
		/* �f�B���N�g���ڂ̏�� start �{�^���������ꂽ��A����
		   �f�B���N�g���̒��ɓ��� */
		if(curbtn & SCE_PADstart &&
		table[view +line].AttrFile & sceMcFileAttrSubdir) {
			sceMcChdir(mcport, mcslot, table[view +line].EntryName, 0);
			sceMcSync(0, 0, &ret);
			if(!ret) {
				phase = 0;
			}
			break;
		}

		/* ���{�^���������ꂽ��A���ݑI�𒆂̃t�@�C���� file status
		   �\���̑Ώۂɂ���t�@�C���Ƃ��ċL������ */
		if(curbtn & SCE_PADRright) {
			openid = 1;
			strcpy(openname, table[view +line].EntryName);
			sceDevConsClearBox(console, 0, MaxVertTitle+1, 79, 1);
			sceDevConsLocate(console, 0, MaxVertTitle +1);
			sceDevConsPrintf(console, "cur file : %s", openname);
			break;
		}

		/* �~�{�^���������ꂽ�猻�ݑI�𒆂̃G���g�����폜���� */
		if(curbtn & SCE_PADRdown) {
			ret = sceMcDelete(mcport, mcslot, table[view +line].EntryName);
			sceMcSync(0, 0, &ret);
			if(!ret) {
				phase = 0;
			}
			break;
		}

		if(curbtn & SCE_PADL1) {
			if(view >0) {
				view -= 4;
				line += 4;
				if(line >= MaxDirView) {
					line = MaxDirView -1;
				}
			}
		}
		else if(curbtn & SCE_PADL2) {
			if( (view +MaxDirView) < ent) {
				view += 4;
				line -= 4;
				if(line < 0) {
					line = 0;
				}
			}
		}
		/* �I������G���g�������̂��̂Ɉړ� */
		else if(curbtn & SCE_PADLup) {
			if(line) {
				--line;
			}
			else if(view > 0) {
				view -= 4;
				line += 3;
			}
		}
		/* �I������G���g��������̂��̂Ɉړ� */
		else if(curbtn & SCE_PADLdown) {
			if(line < (MaxDirView -1) && (view + line) < (ent -1)){
				++line;
			}
			else if((view +MaxDirView) < ent) {
				view += 4;
				line -= 3;
			}
		}

		/* �{�^����������Ď��̃f�B���N�g���Ɉړ������Ƃ��� case 2
		   �����s���� redraw ���� */
		if( !(curbtn & (SCE_PADL1 | SCE_PADL2 | SCE_PADLup |
		SCE_PADLdown)) ){
			break;
		}
	case 2:
		if(phase==2) {
			++phase;
		}

		sceDevConsClearBox(console, 0, MaxVertTitle+3,
			80, MaxDirView+2);
		sceDevConsLocate(console, 0, MaxVertTitle+3);
		if(!view) {
			sceDevConsPrintf(console, "----------------------------------------------------\n");
		}
		else {
			sceDevConsPrintf(console, "-------------- more previous entries ---------------\n");
		}
		for(count=0; count<MaxDirView; ++count) {
			if((count + view) >= ent) {
				sceDevConsPrintf(console, "\n");
				continue;
			}
			if(count==line) {
				sceDevConsAttribute(console, 4);
			}

			if(table[count+view].AttrFile& sceMcFileAttrSubdir) {
				sceDevConsPrintf(console,
				"%2d: %s/   &7\n", count +view +1,
				table[count +view].EntryName);
			}
			else {
				sceDevConsPrintf(console,
					"%2d: %-20s   &7%7d byte \n",
					count +view +1,
					table[count +view].EntryName,
					table[count +view].FileSizeByte);
			}
		}
		if((count+ view) >= ent) {
			sceDevConsPrintf(console, "----------------------------------------------------\n");
		}
		else {
			sceDevConsPrintf(console, "-------------- more entries afterwards -------------\n");
		}
		break;
	}

	if(ret <0){
		sceDevConsAttribute(console, 6);
		if(ret == -2) {
			sceDevConsMessage(console, 25,MaxVertTitle+3," No Format ");
		}
		else if(ret == -4) {
			sceDevConsMessage(console, 25,MaxVertTitle+3," Directory not found ");
		}
		else if(ret == -5) {
			sceDevConsMessage(console, 25,MaxVertTitle+3," Directory not empty ");
		}
		else if(ret != -1) {
			sceDevConsMessage(console, 25,MaxVertTitle+3, " No Card ");
		}
		sceDevConsAttribute(console, 7);
		return 0;
	}
	return 1;
}



int mnDefaultEnt(int mode, int btn) {
	static sceMcIconSys icon;
	static sceVu0IVECTOR bgcolor[4] = {	/* 0x80 ���ő�P�x */
		{ 0x80,    0,    0, 0 },		/* �w�i�F(����) */
		{    0, 0x80,    0, 0 },		/* �w�i�F(�E��) */
		{    0,    0, 0x80, 0 },		/* �w�i�F(����) */
		{ 0x80, 0x80, 0x80, 0 },		/* �w�i�F(�E��) */
	};
	static sceVu0FVECTOR lightdir[3] = {
		{ 0.5, 0.5, 0.5, 0.0 },
		{ 0.0,-0.4,-0.1, 0.0 },
		{-0.5,-0.5, 0.5, 0.0 },
	};
	static sceVu0FVECTOR lightcol[3] = {
		{ 0.48, 0.48, 0.03, 0.00 },
		{ 0.50, 0.33, 0.20, 0.00 },
		{ 0.14, 0.14, 0.38, 0.00 },
	};
	static sceVu0FVECTOR ambient = { 0.50, 0.50, 0.50, 0.00 };
	unsigned char *iconName[3] ={"slime1.ico", "slime2.ico", "slime3.ico"};
	static int oldbtn;
	static int phase;
	int fd, src, count;
	static unsigned char fname[64] = "/BISLPS-00000";
	static unsigned char hostName[32] = "host0:";
	int ret, len, curbtn, size;

	/* ���������[�h */
	if(!mode) {
		oldbtn = btn;
		phase = 0;

		sceDevConsLocate(console, 0,7);
		sceDevConsPrintf(console, "input title code: ");
		inpstr(fname +1, 20, 11, 0, btn);
	}

	curbtn = btn & (oldbtn ^ btn);		/* up-edge ���������o */
	oldbtn = btn;
	ret = 0;
	len = strlen(fname);

	switch(phase) {
	case 0:
		/* �^�C�g�������� */
		sceDevConsLocate(console, 0,7);
		sceDevConsPrintf(console, "input title code: ");
		inpstr(fname +1, 20, -1, 0, btn);
		if(strlen(fname) && curbtn & SCE_PADstart) {
			sceDevConsLocate(console, 18,7);
			inpstr(fname +1, -20, 0, 0, btn);
			++phase;
		}
		if(curbtn & SCE_PADselect) {
			sceDevConsLocate(console, 18,7);
			inpstr(fname +1, -20, 0, 0, btn);
			return 0;
		}
		break;
	case 1:
		len = strlen(fname);

		/* �^�C�g�����Ƃ̃f�B���N�g������ */
		ret = sceMcMkdir(mcport, mcslot, fname);
		sceMcSync(0, 0, &ret);

		/* icon.sys �V�K�쐬 */  
		strcpy(fname +len, "/icon.sys");
		ret = sceMcOpen(mcport, mcslot, fname, SCE_RDWR |SCE_CREAT);
		sceMcSync(0, 0, &ret);
		if(ret < 0) {
			break;
			}

		fd = ret;

		/* icon.sys �̏������� */
		memset(&icon, 0, sizeof(icon));
		strcpy(icon.Head, "PS2D");
		strcpy(icon.TitleName,
			"�A�C�R���@�A�j���[�V�����i�e�N�X�`���E�^�C�v�j");
		icon.OffsLF = 24;	/* �^�C�g���� �܂�Ԃ��I�t�Z�b�g */
		icon.TransRate = 0x60;	/* �w�i�̓����x 25% */
		memcpy(icon.BgColor, bgcolor, sizeof(bgcolor));
		memcpy(icon.LightDir, lightdir, sizeof(lightdir));
		memcpy(icon.LightColor, lightcol, sizeof(lightcol));
		memcpy(icon.Ambient, ambient, sizeof(ambient));

		strcpy(icon.FnameView, iconName[0]);
		strcpy(icon.FnameCopy, iconName[1]);
		strcpy(icon.FnameDel, iconName[2]);

		ret = sceMcWrite(fd, &icon, sizeof(icon));
		sceMcSync(0, 0, &ret);

		sceMcClose(fd);
		sceMcSync(0, 0, &ret);


		/* �A�C�R���f�[�^�̃R�s�[ */
		for(count = 0; count < 3; ++count) {
			strcpy(hostName +6, iconName[count]);
			src = sceOpen(hostName, SCE_RDWR);
			if(src < 0) {
				sceDevConsAttribute(console, 6);
				sceDevConsMessage(console, 15,10,
					" can't open icon file ");
				sceDevConsAttribute(console, 7);
				return 0;
			}

			fname[len] = '/';
			strcpy(fname +len +1, iconName[count]);
			sceMcOpen(mcport, mcslot, fname, SCE_RDWR | SCE_CREAT);
			sceMcSync(0, 0, &ret);
			if(ret < 0) {
				break;
			}
			fd = ret;

			/* 1kB���Ƃɓǂݍ��݁A�����o�����J��Ԃ� */  
			size = BuffSizeRw;
			while(size == BuffSizeRw) {
				size = sceRead(src, rwbuff, BuffSizeRw);
				sceMcWrite(fd, rwbuff, size);
				sceMcSync(0, 0, &ret);
				if(ret < 0) {
					break;
				}
			}
			if(ret < 0) {
				break;
				}

			sceClose(src);
			sceMcClose(fd);
			sceMcSync(0, 0, &ret);
			if(ret < 0) {
				break;
			}
		}

		if(!ret) {
			sceDevConsLocate(console, 5, 10);
			sceDevConsPrintf(console, "icon data ready");
			fname[len] = 0;
			return 0;
		}
	}

	if(ret < 0) {
		dispError(ret);
		fname[len] = 0;
		return 0;
	}
	return 1;
}



typedef struct {
	char X, Y;
	char Type, Len;
	unsigned char *Buff;
	} StatParam;

#define StatXs		5
#define StatYs		(MaxVertTitle +4)


/* #dispStatus --- sceMcSetFileInfo() �ɓn�����߂̕ҏW�����e��\�� */
void dispStatus(sceMcTblGetDir *info, StatParam *setting) {
	int line, count;

	line = 0;
	sceDevConsLocate(console, StatXs, StatYs + line);
	sceDevConsPrintf(console, "               1p   l d cxwr");

	for(count = 0; count < 16; ++count) {
		if(count==3 || count==4 || count==8
		|| count==10 || count >= 12) {
			setting[0].Buff[count] =
				(info->AttrFile & (0x8000 >> count))? 'o':'x';
		}
		else {
			setting[0].Buff[count] = ' ';
		}
	}
	line += 1;
	sceDevConsLocate(console, StatXs, StatYs + line);
	sceDevConsPrintf(console, "attribute:  &5%s&7", setting[0].Buff);

	line += 2;
	sceDevConsLocate(console, StatXs, StatYs + line);
	sceDevConsPrintf(console, "file size: %d byte", info->FileSizeByte);

	sprintf(setting[1].Buff, "%4d", info->CYear);
	sprintf(setting[2].Buff, "%2d", info->CMonth);
	sprintf(setting[3].Buff, "%2d", info->CDay);
	sprintf(setting[4].Buff, "%2d", info->CHour);
	sprintf(setting[5].Buff, "%02d", info->CMin);
	sprintf(setting[6].Buff, "%02d", info->CSec);
	line += 2;
	sceDevConsLocate(console, StatXs, StatYs + line);
	sceDevConsPrintf(console,
		"create:    &5%s&7/&5%s&7/&5%s&7  &5%s&7:&5%s&7:&5%s&7",
		setting[1].Buff, setting[2].Buff, setting[3].Buff,
		setting[4].Buff, setting[5].Buff, setting[6].Buff);

	sprintf(setting[7].Buff, "%4d", info->MYear);
	sprintf(setting[8].Buff, "%2d", info->MMonth);
	sprintf(setting[9].Buff, "%2d", info->MDay);
	sprintf(setting[10].Buff, "%2d", info->MHour);
	sprintf(setting[11].Buff, "%02d", info->MMin);
	sprintf(setting[12].Buff, "%02d", info->MSec);
	line += 2;
	sceDevConsLocate(console, StatXs, StatYs + line);
	sceDevConsPrintf(console,
		"modify:    &5%s&7/&5%s&7/&5%s&7  &5%s&7:&5%s&7:&5%s&7",
		setting[7].Buff, setting[8].Buff, setting[9].Buff,
		setting[10].Buff, setting[11].Buff, setting[12].Buff);
}


/* #mnFileInfo --- mnGetDir�Ŏw�肳�ꂽ�t�@�C���̃t�@�C������\���^�C������*/
int mnFileInfo(int mode, int btn) {
	static int oldbtn;
	static int pos, phase, ret;
	int curbtn, val;
	static sceMcTblGetDir info;

	static StatParam param[] = {
		{ 12, 1, 3, 16, "oooooooooooooooo" },
		{ 11, 5, 1, 4, "1999" },
		{ 16, 5, 1, 2, "01" },
		{ 19, 5, 1, 2, "02" },
		{ 23, 5, 1, 2, "03" },
		{ 26, 5, 1, 2, "04" },
		{ 29, 5, 1, 2, "05" },
		{ 11, 7, 1, 4, "2000" },
		{ 16, 7, 1, 2, "06" },
		{ 19, 7, 1, 2, "07" },
		{ 23, 7, 1, 2, "08" },
		{ 26, 7, 1, 2, "09" },
		{ 29, 7, 1, 2, "10" },
		};


	/* ���������[�h */
	if(!mode) {
		oldbtn = btn;
		phase = 0;
		pos = 0;

		ret = sceMcGetDir(mcport, mcslot, openname, 0, 1, &info);
		sceMcSync(0, 0, &ret);

		sceDevConsLocate(console, 0, StatYs -2);
		sceDevConsPrintf(console, "status for: %s", openname);

		dispStatus(&info, param);

		sceDevConsLocate(console,
			param[pos].X + StatXs, param[pos].Y +StatYs);
		inpstr(param[pos].Buff,param[pos].Len, 0,param[pos].Type, btn);
	}

	curbtn = btn & (oldbtn ^ btn);		/* up-edge ���������o */
	oldbtn = btn;

	switch(phase) {
	case 0:
		sceDevConsLocate(console,
			param[pos].X + StatXs, param[pos].Y +StatYs);
		val = inpstr(param[pos].Buff, param[pos].Len, -1,
			param[pos].Type, btn);

		if(curbtn&(SCE_PADstart |SCE_PADselect |SCE_PADL1 |SCE_PADL2)){
			switch(pos) {
			case 0:
				info.AttrFile = val;
				break;
			case 1:
				info.CYear = val;
				break;
			case 7:
				info.MYear = val;
				break;
			default:
				if(pos < 8) {
					((unsigned char *)&info._Create)[7 - pos] = val;
				}
				else {
					((unsigned char *)&info._Modify)[13 - pos] = val;
				}
				break;
			}

			sceDevConsLocate(console,
				param[pos].X + StatXs, param[pos].Y +StatYs);
			inpstr(param[pos].Buff,-1, 0, param[pos].Type, btn);
		}


		/* start �{�^���������ꂽ��C�����e�𔽉f���� */
		if(curbtn & SCE_PADstart) {
			ret = sceMcSetFileInfo(mcport, mcslot, openname,
				(unsigned char *)&info, 0xF);
			sceMcSync(0, 0, &ret);

			sceDevConsLocate(console, 10, StatYs +11);
			sceDevConsPrintf(console, "status changing being processed");
			return 0;
		}
		/* select�{�^���������ꂽ��C�����e��j�����ă��j���[�𔲂���*/
		if(curbtn & SCE_PADselect) {
			sceDevConsLocate(console, 10, StatYs +11);
			sceDevConsPrintf(console, "status changing canceled");
			return 0;
		}

		/* L1,L2 �{�^���ŏC���Ώۂ̍��ڂ��ړ� */
		if((curbtn & SCE_PADL1) && pos) {
			--pos;
		}
		else if((curbtn & SCE_PADL2) && pos < 13) {
			++pos;
		}
		if(curbtn & (SCE_PADL1 | SCE_PADL2)) {
			sceDevConsLocate(console,
				param[pos].X + StatXs, param[pos].Y +StatYs);
			inpstr(param[pos].Buff, param[pos].Len,
				0, param[pos].Type, btn);
		}
		break;
	case 1:
	}

	if(ret < 0) {
		dispError(ret);
		return 0;
	}
	return 1;
}



int mnUnFormat(int mode, int btn) {
	static int oldbtn;
	int curbtn, ret;

	if(!mode) {
		oldbtn = btn;
	}

	curbtn = btn & (oldbtn ^ btn);		/* up-edge ���������o */
	oldbtn = btn;

	/* �A���t�H�[�}�b�g�J�n�̌x���\�� */
	sceDevConsLocate(console, 10, MaxVertTitle +3);
	if((curbtn & SCE_PADRright) || (frame % 60)== 20) {
		sceDevConsPrintf(console,
			"&2unformat sequence going to be stated&7");
	}
	else if(!(frame % 60)) {
		sceDevConsPrintf(console,
			"                                    ");
	}

	sceDevConsLocate(console, 10, MaxVertTitle +5);
	if( ((frame % 60) == 30) || curbtn) {
		sceDevConsPrintf(console,"                          ");
		sceDevConsLocate(console, 10, MaxVertTitle +6);
		sceDevConsPrintf(console,"                          ");
	}

	/* ���������ꂽ��A���t�H�[�}�b�g���s */
	if(curbtn & SCE_PADRright) {
		/* �A���t�H�[�}�b�g(�f�o�b�O�p�֐�) */
		ret = sceMcUnformat(mcport, mcslot);
		sceMcSync(0, 0, &ret);

		if(ret) {
			dispError(ret);
			}
		else {
			sceDevConsLocate(console, 10, MaxVertTitle +5);
			sceDevConsPrintf(console, "Unformat completed");
			}

		return 0;
	}
	/* �~�������ꂽ��A���t�H�[�}�b�g �L�����Z�� */
	else if(curbtn) {
		sceDevConsLocate(console, 10, MaxVertTitle +5);
		sceDevConsPrintf(console, "unformat canceled");
		return 0;
	}
	else if((frame % 60) == 50) {
		sceDevConsPrintf(console,"confirm --> press O button");
		sceDevConsLocate(console, 10, MaxVertTitle +6);
		sceDevConsPrintf(console,"cancel  --> other button");
	}
	return 1;
}



int mnFormat(int mode, int btn) {
	static int oldbtn, phase;
	int ret, curbtn;

	if(!mode) {
		oldbtn = btn;
		phase = 0;
	}

	curbtn = btn & (oldbtn ^ btn);		/* up-edge ���������o */
	oldbtn = btn;


	switch(phase) {
	case 0:
		sceDevConsLocate(console, 10, MaxVertTitle +3);
		if((curbtn & SCE_PADRright) || (frame % 60)== 20) {
			sceDevConsPrintf(console,
				"&2format sequence going to be stated&7");
		}
		else if(!(frame % 60)) {
			sceDevConsPrintf(console,
				"                                  ");
		}

		sceDevConsLocate(console, 10, MaxVertTitle +5);
		if( ((frame % 60) == 30) || curbtn) {
			sceDevConsPrintf(console,"                          ");
			sceDevConsLocate(console, 10, MaxVertTitle +6);
			sceDevConsPrintf(console,"                          ");
		}

		/* ���������ꂽ��t�H�[�}�b�g���s */
		sceDevConsLocate(console, 10, MaxVertTitle +5);
		if(curbtn & SCE_PADRright) {
			sceDevConsPrintf(console, "format started");
			++phase;
		}
		/* �~�������ꂽ��t�H�[�}�b�g �L�����Z�� */
		else if(curbtn) {
			sceDevConsPrintf(console, "format canceled");
			return 0;
		}
		else if((frame % 60) == 50) {
			sceDevConsPrintf(console,"confirm --> press O button");
			sceDevConsLocate(console, 10, MaxVertTitle +6);
			sceDevConsPrintf(console,"cancel  --> other button");
		}
		break;
	case 1:
		/* �t�H�[�}�b�g���� �o�^�^�J�n */
		sceMcFormat(mcport, mcslot);
		++phase;
		return 1;
	case 2:
		sceDevConsLocate(console, 10, MaxVertTitle +7);

		/* �t�H�[�}�b�g����������܂ő҂� */
		if(!sceMcSync(1, 0, &ret)) {
			if((frame % 60) == 20) {
				sceDevConsPrintf(console, "formatting");
			}
			else if( !(frame % 60) ) {
				sceDevConsPrintf(console, "          ");
			}
			break;
		}

		sceDevConsLocate(console, 10, MaxVertTitle +9);
		if(!ret) {
			sceDevConsPrintf(console, "format completed");
		}
		else {
			dispError(ret);
		}
		return 0;
	}
	return 1;
}



int mnRename(int mode, int btn) {
	static int oldbtn;
	static int exec, phase;
	int curbtn, count, ret;
	static unsigned char fname[32];

	/* ���������[�h */
	if(!mode) {
		oldbtn = btn;
		exec = 0;
		phase = 0;

		if(!openname[0]) {
			sceDevConsAttribute(console, 6);
			sceDevConsMessage(console, 15,10," No file selected ");
			sceDevConsAttribute(console, 7);
			return 0;
			}

		strcpy(fname, openname);

		sceDevConsLocate(console, 0,7);
		sceDevConsPrintf(console, "old name: %s \n\n", openname);
		sceDevConsPrintf(console, "new name: ");
		inpstr(fname, 31, strlen(fname)-1, 0, btn);
		}

	curbtn = btn & (oldbtn ^ btn);		/* up-edge ���������o */
	oldbtn = btn;
	ret = 0;

	switch(phase) {
	case 0:
		sceDevConsLocate(console, 0,7);
		sceDevConsPrintf(console, "old name: %s \n\n", openname);
		sceDevConsPrintf(console, "new name: ");
		inpstr(fname, 31, -1, 0, btn);
		if(strlen(fname) && curbtn & (SCE_PADstart | SCE_PADselect)) {
			sceDevConsLocate(console, 10,9);
			inpstr(fname, -31, 0, 0, btn);
			}
		if(curbtn & SCE_PADstart) {
			++phase;
			}
		else if(curbtn & SCE_PADselect) {
			return 0;
			}
		break;
	case 1:
		sceMcRename(mcport, mcslot, openname, fname);
		sceMcSync(0, 0, &ret);
		if(ret >= 0) {
			sceDevConsLocate(console, 0, 11);
			sceDevConsPrintf(console,"rename completed");
			return 0;
			}
		}

	if(ret < 0) {
		dispError(ret);
		return 0;
		}
	return 1;
	}


/* #inpstr --- pos:>0 �J�[�\���ʒu�������A<0 �O��̈ʒu�Ɠ���
	���͍ő啶������ maxlen �Ȃ̂ŕ����z��� maxlen +1 �ȏ�K�v
	maxlen < 0 �̏ꍇ�͏I�������� unselect �F�ŕ\�����邾��
	digit --- 0:��������́A1:10�i���͌�A10�i�l��Ԃ��A2:16�i������
		3:boolean���� 2�i���Ƃ��Ă̒l��Ԃ� */
int inpstr(unsigned char *buff, int maxlen, int pos, int digit, int btn) {
	static int curs, oldbtn;
	int count, curbtn, ret;

	/* ���͊m���ɃA���Z���N�g�F�ŕ`�悵�����Ƃ� */
	if(maxlen < 0) {
		sceDevConsAttribute(console, 5);
		sceDevConsPrintf(console, buff);
		sceDevConsAttribute(console, 7);
		return 0;
	}

	/* �J�[�\���̏����ʒu��ݒ肷��Ƃ� */
	if(pos >=0) {
		oldbtn = btn;
		curs = pos;
	}

	curbtn = btn & (oldbtn ^ btn);		/* up-edge ���������o */
	oldbtn = btn;

	if(curbtn & SCE_PADLleft) {
		--curs;
	}
	else if(curbtn & SCE_PADLright) {
		++curs;
	}

	if(curs < 0) {
		curs = 0;
	}
	else if(curs >= maxlen) {
		curs = maxlen -1;
	}
	if(curs > strlen(buff)) {
		curs = strlen(buff);
	}

	buff[maxlen] = 0;

	if(curbtn & (SCE_PADLup | SCE_PADLdown | SCE_PADRup | SCE_PADRleft | SCE_PADRright) && !buff[curs]) {
		buff[curs +1] = 0;
	}
	/* 10�i�����̓��[�h */
	if(digit == 1) {
		if(curbtn & SCE_PADLup) {
			++buff[curs];
			if(!isdigit(buff[curs])) {
				buff[curs] = '0';
			}
		}
		if(curbtn & SCE_PADLdown) {
			--buff[curs];
			if(!isdigit(buff[curs])) {
				buff[curs] = '9';
			}
		}
	}
	/* 16�i�����̓��[�h */
	else if(digit == 2) {
		if(curbtn & SCE_PADLup) {
			++buff[curs];
			if(buff[curs]==('9'+1)) {
				buff[curs] = 'A';
			}
			if(!isxdigit(buff[curs])) {
				buff[curs] = '0';
			}
		}
		if(curbtn & SCE_PADLdown) {
			--buff[curs];
			if(buff[curs]==('A'-1)) {
				buff[curs] = '9';
			}
			if(!isxdigit(buff[curs])) {
				buff[curs] = 'F';
			}
		}
	}
	/* ���^�~ ���̓��[�h */
	else if(digit == 3) {
		if(curbtn & (SCE_PADLup | SCE_PADLdown)) {
			if(buff[curs]=='o') {
				buff[curs] = 'x';
			}
			else {
				buff[curs] = 'o';
			}
		}
	}
	else {
		/* 41[A-Z]5A --> 61[a-z]7A --> 30[0123456789:;<=>?@]40
		-->20[ !"#$%&'()*+,-./]2F --> 5B[[\]^_`]60 --> 7B[{|}~]7E */
		if(curbtn & SCE_PADLup) {
			++buff[curs];
			switch(buff[curs]-1) {
			case 0x5A:	/* �啶���̏I��Ȃ珬������ */
				buff[curs] = 0x61; break;
			case 0x7A:	/* �������̏I��Ȃ琔���� */
				buff[curs] = 0x30; break;
			case 0x40:	/* @ �̎��� �� */
				buff[curs] = 0x20; break;
			case 0x2F:	/* / �̎��� [ */
				buff[curs] = 0x5B; break;
			case 0x60:	/* ` �̎��� { */
				buff[curs] = 0x7B; break;
			case 0x7E:	/* ~ �̎��� �啶�� */
				buff[curs] = 0x41; break;
			}
			if(buff[curs] < 0x20 || buff[curs] >= 0x7F) {
				buff[curs] = 0x30;
			}
		}
		/* 41[A-Z]5A <-- 61[a-z]7A <-- 30[0123456789:;<=>?@]40
		<--20[ !"#$%&'()*+,-./]2F <-- 5B[[\]^_`]60 <-- 7B[{|}~]7E */
		else if(curbtn & SCE_PADLdown) {
			--buff[curs];
			switch(buff[curs]+1) {
			case 0x61:	/* �������̏I��Ȃ�啶���� */
				buff[curs] = 0x5A; break;
			case 0x30:	/* �������̏I��Ȃ珬���� */
				buff[curs] = 0x7A; break;
			case 0x20:	/* �󔒂̎��� @ */
				buff[curs] = 0x40; break;
			case 0x5B:	/* [ �̎��� / */
				buff[curs] = 0x2F; break;
			case 0x7B:	/* { �̎��� ` */
				buff[curs] = 0x60; break;
			case 0x41:	/* �啶�� �̎��� ~ */
				buff[curs] = 0x7E; break;
			}
			if(buff[curs] < 0x20 || buff[curs] >= 0x7F) {
				buff[curs] = 'z';
			}
		}
		/* ���{�^���ŋ����I�� 0 �ɂ��� */
		if(curbtn & SCE_PADRleft) {
			buff[curs] = 0x30;
		}
		/* ���{�^���ŋ����I�� A �ɂ��� */
		else if(curbtn & SCE_PADRup) {
			buff[curs] = 0x41;
		}
		/* ���{�^���ŋ����I�� a �ɂ��� */
		else if(curbtn & SCE_PADRright) {
			buff[curs] = 0x61;
		}
	}

	/* ��������͂̂Ƃ��̂� �~�{�^���Ńo�b�N�X�y�[�X */
	if(curbtn & SCE_PADRdown && curs && !digit) {
		--curs;
		for(count=curs; count < maxlen; ++count) {
			buff[count] = buff[count+1];
		}
	}
	/* ��������͂̂Ƃ��̂� R1�{�^���ŋ󔒂��C���T�[�g */
	else if(curbtn & SCE_PADR1 && !digit) {
		for(count= maxlen -1; count > curs; --count) {
			buff[count] = buff[count -1];
		}
		buff[maxlen] = 0;
		buff[curs] = ' ';
	}

	/* \0 �̕����͍��F�̃A���_�[�o�[�ŕ\�� */
	for(count=0; count < maxlen; ++count) {
		if(count==curs && ((frame % 30) < 10)) {
			sceDevConsPut(console, '_', 7);
		}
		else if(count < strlen(buff)) {
			sceDevConsPut(console, buff[count], 4);
		}
		else {
			sceDevConsPut(console, '_', 0);
		}
	}

	sceDevConsAttribute(console, 7);

	/* �v������Ă���`�ɕϊ����ĕԂ� */
	if(digit == 1) {
		return atoi(buff);
	}
	else if(digit == 2) {
		return atox(buff);
	}
	else if(digit == 3) {
		ret = 0;
		for(count = 0; count < maxlen; ++count) {
			ret = (ret << 1) | (buff[count] != 'x');
		}
		return ret;
	}
	return 0;
}



unsigned atox(unsigned char *data) {
	unsigned ret;

	ret = 0;
	while(isxdigit(*data)) {
		if(isdigit(*data)) {
			ret = ret * 16 + *data - '0';
		}
		else {
			ret = ret * 16 + toupper(*data) - 'A' +10;
		}
		++data;
	}

	return ret;
}



void remapColorLut() {

	sceDevConsSetColor(console, 0, 0x00,0x00,0x00);		/* black */
	sceDevConsSetColor(console, 1, 0x80,0x90,0xFF);		/* blue */
	sceDevConsSetColor(console, 2, 0xFF,0x80,0x80);		/* red */
	sceDevConsSetColor(console, 3, 0xFF,0x60,0xFF);		/* gamenta */
	sceDevConsSetColor(console, 4, 0x60,0xFF,0x60);		/* green */
	sceDevConsSetColor(console, 5, 0x60,0xFF,0xFF);		/* cyan */
	sceDevConsSetColor(console, 6, 0xFF,0xFF,0x60);		/* yellow */
	sceDevConsSetColor(console, 7, 0xFF,0xFF,0xFF);		/* white */
}



void loadIopModule() {

	sceSifInitRpc(0);

	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
			0, NULL) < 0){
		printf("Can't load module sio2man\n");
	}
#if UseMultiTap
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/mtapman.irx",
			0, NULL) < 0){
		printf("Can't load module mtapman\n");
	}
#endif
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/mcman.irx",
			0, NULL) < 0){
		printf("Can't load module mcman\n");
	}
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/mcserv.irx",
			0, NULL) < 0){
		printf("Can't load module mcserv\n");
	}
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
			0, NULL) < 0){
		printf("Can't load module padman\n");
	}
}
