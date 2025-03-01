/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - <file> -
 *
 *                         Version <0.10>
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
 *      0.10            Mar,26,1999     hakamatani  first version
 */


#include <eekernel.h>
#include <eeregs.h>
#include <sifdev.h>
#include <stdio.h>

/* ================================================================
 *
 *      Program
 *
 * ================================================================ */

#define ERR_STOP while(1)

#define RW_SIZE 0x8000

static unsigned char  buffer[RW_SIZE];

int main()
{
	int rfd,wfd,len,ret,i,*ip,is_end,err;
	unsigned char tmp[0x100],*cp;

	printf("sample start.\n");
	/* その１ */
	/* このファイルのコピーを作るサンプル */
	printf("\tsample1:copy main.c to copy.c.\n");

	/* ファイルの読み込みオープン */
	printf("\t\topen file main.c.\n");
	rfd = sceOpen("host:main.c", SCE_RDONLY);
	if (rfd < 0 ) {
		/* エラー処理 */
		printf("Can't open file main.c.\n");
		ERR_STOP;
	}
	/* ファイルの書き込みオープン */
	printf("\t\topen file copy.c.\n");
	wfd = sceOpen("host:copy.c", SCE_WRONLY|SCE_TRUNC|SCE_CREAT);
	if (wfd < 0 ) {
		/* エラー処理 */
		printf("Can't open file copy.c.\n");
		ERR_STOP;
	}
	/* ファイルサイズの確認 */
	len = sceLseek(rfd, 0, SCE_SEEK_END); /* サイズを知って*/
	sceLseek(rfd, 0, SCE_SEEK_SET); /* offset を戻しておく */
	printf("\t\tsize of main.c is %d.\n", len);
	while(len > 0) {
		/* データの読み込み */
		ret = sceRead(rfd, tmp, 0x100);
		/* データの書きみ込み */
		sceWrite(wfd, tmp ,ret);

		len -= ret;
	}
	printf("\t\tmain.c and copy are closed.\n");
	sceClose(rfd);
	sceClose(wfd);

	/* その２ */
	/* inc data を作成/書き込み後,読んで検証 */
	printf("\tsample2:inc data is wrote to file,then read/verified.\n");

	/* ファイルのライトオープン */
	printf("\t\topen file incdata.\n");
	wfd = sceOpen("host:incdata", SCE_RDWR|SCE_TRUNC|SCE_CREAT);
	if (wfd < 0 ) {
		/* エラー処理 */
		printf("Can't open file incdata.\n");
		ERR_STOP;
	}

	/* 4バイトで１増えるデータを作成 */
	printf("\t\tcreate inc data.\n");
	ip = (int *) & buffer[0];
	for (i = 0; i < RW_SIZE / 4; i ++)
	  *(ip + i) = i;

	/* データの書込み(一括の例・分割でも可能) */
	printf("\t\twrite inc data.\n");
	ret = sceWrite(wfd, (void *)ip , RW_SIZE);
	if (ret != RW_SIZE) {
		/* エラー処理 */
		printf("Can't write data size %d ,but %d.\n",RW_SIZE,ret);
		ERR_STOP;
	}
	/* ファイルのクローズ */
	printf("\t\tclose file incdata.\n");
	sceClose(wfd);

	/* ファイルのリードオープン */
	printf("\t\topen file incdata.\n");
	rfd = sceOpen("host:incdata", SCE_RDONLY);
	if (rfd < 0 ) {
		/* エラー処理 */
		printf("Can't open file incdata\n");
		ERR_STOP;
	}

	/* バッファを０クリア */
	printf("\t\tclear buffer.\n");
	for (i = 0; i < RW_SIZE ; i ++)
	  buffer[i] = 0;

	/* データの読み込み(分割例・一括でも可能) */
	printf("\t\tread data.\n");
	i = 0;
	cp = (unsigned char *) & buffer[0];
	while(1) {
		ret = sceRead(rfd, (void *)(cp + i) , 0x112);
		if (ret <= 0) break;
		i += ret;
	}
	/* ファイルのクローズ */
	printf("\t\tclose file incdata.\n");
	sceClose(rfd);

	/* データの比較 */
	printf("\t\tverify data.\n");
	ip = (int *) & buffer[0];
	err = 0;
	for (i = 0; i <	(RW_SIZE / 4); i ++)
		if (*(ip + i) != i) {
			printf("verify error src %x dst %x\n",*(ip + i),i);
			err = 1;
		}
	if (err) {
		/* エラー処理 */
		printf("Verify error.\n");
		ERR_STOP;
	}
	
	/* その３ */
	/* inc data を作成/書き込み後,読んで検証 */
	printf("\tsample3:no_wait read.\n");
	/* ファイルの NoWaitリードオープン */
	printf("\t\topen file incdata by no_wait.\n");
	rfd = sceOpen("host:incdata", SCE_RDONLY|SCE_NOWAIT);
	if (rfd < 0 ) {
		/* エラー処理 */
		printf("Can't open file incdata\n");
			ERR_STOP;
	}

	/* バッファを０クリア */
	printf("\t\tclear buffer.\n");
	for (i = 0; i < RW_SIZE ; i ++)
	  buffer[i] = 0;

	/* データの読み込み(一括・待ちなし) */
	printf("\t\tread data by no_wait.\n");
	ret = sceRead(rfd, (void *)&buffer[0] , RW_SIZE);
	
	/* データの読み込み(一括・待ちなし) */
	printf("\t\twait data read ..");
	is_end = 1;
	i = 0;
	while(is_end) {
	  sceIoctl(rfd, SCE_FS_EXECUTING, &is_end);
	  i ++;
	}
	printf("end. wait loop = %d\n",i);

	/* ファイルのクローズ */
	printf("\t\tclose file incdata.\n");
	sceClose(rfd);

	/* データの比較 */
	printf("\t\tverify data.\n");
	ip = (int *) & buffer[0];
	err = 0;
	for (i = 0; i <	(RW_SIZE / 4); i ++)
		if (*(ip + i) != i) {
			printf("verify error src %x dst %x\n",*(ip + i),i);
			err = 1;
		}
	if (err) {
		/* エラー処理 */
		printf("Verify error.\n");
			ERR_STOP;
	}

	printf("sample end.\n");

	return 0;
}

/* end of file.*/
