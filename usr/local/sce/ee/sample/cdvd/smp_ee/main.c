/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - CD/DVD -
 *
 *                         Version <0.02>
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            <sample.c>
 *                     <main function of file read>
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.01            Oct,13,1999     kashiwabara  first   Version
 *      0.02            Dec,04,1999     kashiwabara  rel_13  Version
 *      0.03            Jan,12,2000     kashiwabara  rel_134 Version
 *      0.04            Feb,02,2000     kashiwabara  rel_142 Version
 *      0.05            Mer,21,2000     kashiwabara  rel_16  Version
 */


#include <eekernel.h>
#include <eeregs.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <stdio.h>
#include <libcdvd.h>

#define MEDIA_CD

/* ================================================================
 *
 *      Program
 *
 * ================================================================ */

unsigned char  bf[2048 * 1024];
unsigned char  toc[2048];
int 	my_thid;

static u_char cb_stack[512 * 16] __attribute__ ((aligned(16)));
#define CB_STACK_SIZE (512 * 16)

void test_callback( int cb_reason )
{
        switch(cb_reason){
            case SCECdFuncRead:
                printf("SCECdFuncRead Ended\n"); break;
            case SCECdFuncSeek:
                printf("SCECdFuncSeek Ended\n"); break;
            case SCECdFuncStandby:
                printf("SCECdFuncStandby Ended\n"); break;
            case SCECdFuncStop:
                printf("SCECdFuncStop Ended\n"); break;
            case SCECdFuncPause:
                printf("SCECdFuncPause Ended\n"); break;
            default:
                printf("Other Ended \n"); break;
        }
        printf("error code= 0x%08x\n",sceCdGetError());
}

int main()
{
	int	 rfd, wfd, rsec, rs_flg, wait0, cnt0, ret, *old_func, lpcnt,
		 disk_type, last_track, readsize, trayflg, traycnt;
	unsigned char *cp;
	char *cw, *filename,wfilename[32];
	sceCdlFILE fp;
	sceCdRMode mode;
	sceCdlLOCCD cl;

#ifdef MDLCHG_RESET
	sceSifInitRpc(0);
	while(sceSifRebootIop("host0:/usr/local/sce/iop/modules/"IOP_IMAGE_file) == 0){
		printf("iop reset error\n");
	}
	while(sceSifSyncIop() == 0);
#endif

	printf("sample start.\n");
	sceSifInitRpc(0);

	printf(" sceCdInit\n");
	sceCdInit(SCECdINIT);
#ifdef MEDIA_CD
	sceCdMmode(SCECdCD);
#else
	sceCdMmode(SCECdDVD);
#endif

        sceCdInitEeCB(0, (void *)cb_stack, CB_STACK_SIZE);
        my_thid= GetThreadId();
        ChangeThreadPriority(my_thid, 1);

        old_func= sceCdCallback(test_callback);

	printf(" sceCdDiskReady\n");
	sceCdDiskReady(0);

	lpcnt= 0;
	while(1){	
	    if(lpcnt){
	        /* ���f�B�A���� */
	        traycnt= 0;
	        sceCdTrayReq(2,&trayflg);
	        printf("Change your PlayStation CD/DVD-ROM\n");
	        do{

		    for(wait0 =0; wait0 < 0x100000; wait0++);

		    while(!sceCdTrayReq(2,&trayflg));
		    traycnt+= trayflg;
                    disk_type= sceCdGetDiskType();
	        }while(!traycnt ||
		       (disk_type == SCECdNODISC) ||
		       (disk_type == SCECdDETCT )    );
	        sceCdDiskReady(0);
	    }

	    printf(" sceCdGetDiskType   ");
	    disk_type= sceCdGetDiskType();
	    switch(disk_type){
	        case SCECdIllgalMedia:
		    printf("Disk Type= IllgalMedia\n"); break;
	        case SCECdPS2DVD:
		    printf("Disk Type= PlayStation2 DVD\n"); break;
	        case SCECdPS2CD:
		    printf("Disk Type= PlayStation2 CD\n"); break;
	        case SCECdPS2CDDA:
		    printf("Disk Type= PlayStation2 CD with CDDA\n"); break;
	        case SCECdPSCD:
		    printf("Disk Type= PlayStation CD\n"); break;
	        case SCECdPSCDDA:
		    printf("Disk Type= PlayStation CD with CDDA\n"); break;
	        case SCECdDVDV:
		    printf("Disk Type= DVD video\n"); break;
	        case SCECdCDDA:
		    printf("Disk Type= CD-DA\n"); break;
	        case SCECdDETCT:
		    printf("Working\n"); break;
	        case SCECdNODISC: 
		    printf("Disk Type= No Disc\n"); break;
	        default:
		    printf("Disk Type= OTHER DISK\n"); break;
	    }

	    sceCdDiskReady(0);
	    printf("Get TOC ");
	    sceCdGetToc(toc);

	    cp= (u_char *)toc;
            last_track= (int)btoi(*(cp + 17));
            printf("track max= %d\n",last_track);
            for(cnt0= 0,cp+= 30; cnt0 < last_track; cnt0+= 1){
                printf("track No= %d abs Minute= %d Second= %d Frame= %d\n\n",
                  cnt0 + 1, btoi(*(cp + 7)), btoi(*(cp + 8)), btoi(*(cp + 9)));
                cp+= 10;
            }

	    /* �W�����o�͂Ńt�@�C����ǂ� */
            rfd = sceOpen("cdrom0:\\SYSTEM.CNF;1",SCE_RDONLY);
            if ( rfd < 0 ){
                printf("Can't open SYSTEM.CNF\n");
		lpcnt++;
                continue;
            }
            readsize = sceLseek(rfd, 0, SCE_SEEK_END);
    	    sceLseek(rfd, 0, SCE_SEEK_SET);
            sceRead(rfd, (u_int *)bf, readsize);
            sceClose(rfd);


	    /* PlayStation���s�̃t�@�C���������o���B*/
	    cw= (char *)bf;
	    for(cnt0= 0; (*cw != '\n') && (cnt0 < readsize); cnt0++) cw++;
	    if(cnt0 == readsize){
		 printf("Sorry: File name not Collect\n");
		 lpcnt++;
                 continue;
	    }
	    *cw= 0;
	    cw= (char *)bf;
	    for(cnt0= 0; (*cw != ':') && (cnt0 < readsize); cnt0++) cw++;
	    if(cnt0 == readsize){
		printf("Sorry: File name not Collect\n");
		lpcnt++;
                continue;
	    }
	    cw++; filename= cw;

	    sceCdDiskReady(0);

	    /* ���C�u�����֐��Ńt�@�C����ǂ� */
	    mode.trycount= 0; /* �G���[��������255���g���C		 */
	    mode.spindlctrl= SCECdSpinNom;
				/* �G���[�������͉�]���x�𗎂��ă��[�h	 */
	    mode.datapattern= SCECdSecS2048; /* �f�[�^�T�C�Y��2048byte   */
 
	    /* �t�@�C���̊i�[�ʒu���������� */
	    printf("Search Filename: %s\n",filename);
	    ret= sceCdSearchFile(&fp, filename);
	    if(!ret){
		printf("sceCdSearchFile cmd fail :%d\n",ret);
		lpcnt++;
                continue;
	    }

	    /* ���b�Z�N�^�ɕϊ����ĕ\�����Ă݂�BDVD�ł͈Ӗ������� */ 
	    sceCdIntToPos(fp.lsn, &cl);
	    printf("File pos Minute %d Second %d Sector %d\n",
				cl.minute,cl.second,cl.sector);
	    /* �u���b�N�i���o�ɕϊ����ĕ\������B */
	    printf("File pos lbn= %d\n",sceCdPosToInt(&cl));

	    /* �t�@�C���̓ǂݍ��݃Z�N�^�����Z�o���� */
	    rsec= fp.size / 2048; if(fp.size % 2048) rsec++;
	    if(rsec > 1024) rsec= 1024;
	    printf("CD_READ LSN= %d sectors= %d\n", fp.lsn,rsec);
	    sceCdDiskReady(0);

	    /* �t�@�C���̓ǂݍ��݂��J�n���� */
	    while(!sceCdRead(fp.lsn, rsec, (u_int *)bf, &mode))
	        printf("Read cmd fail\n");
	    printf("ReadSync ");
            for( rs_flg= 1; rs_flg; ){
                rs_flg= sceCdSync(1); 
			/* �t�@�C���̓ǂݍ��݃X�e�[�^�X�𓾂� 0:�I�� */
	        printf("Cur Read Position= %d\n",sceCdGetReadPos());
		for(wait0 =0; wait0 < 0x100000; wait0++);
	    }
	    printf("Disk error code= 0x%08x\n",sceCdGetError());

	    /* �V�[�N�e�X�g */
//	    printf("CdSeek Test\n");
//	    sceCdDiskReady(0);
//	    while(!sceCdSeek(fp.lsn)) printf("Seek cmd fail\n");
//	    sceCdSync(0); 

	    /* �X�^���h�o�C�e�X�g */
	    printf("CdStandby Test\n");
	    sceCdDiskReady(0);
	    while(!sceCdStandby()) printf("Standby cmd fail\n");
            sceCdSync(0); 

	    /* �|�[�Y�e�X�g */
	    printf("CdSPause Test\n");
	    sceCdDiskReady(0);
	    while(!sceCdPause()) printf("Pause cmd fail\n");
            sceCdSync(0); 

	    /* �e�X�g�̂��߃f�B�X�N���~�߂Ă݂� */
	    printf("CdStop Test\n");
	    sceCdDiskReady(0);
	    while(!sceCdStop()) printf("Stop cmd fail\n");
            sceCdSync(0); 

	    sprintf(wfilename,"host0:data%d.dat",lpcnt);
	    wfd = sceOpen(wfilename, SCE_WRONLY|SCE_TRUNC|SCE_CREAT);
	    if (wfd < 0 ) {
		printf("Can't open file data.dat\n");
		lpcnt++;
                continue;
	    }
	    sceWrite(wfd, (u_int *)bf, 2048 * rsec);
	    sceClose(wfd);

	    lpcnt++;
	}
}
/* end of file.*/
