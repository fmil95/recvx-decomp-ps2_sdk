/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *                          Version 0.11
 *                           Shift-JIS
 *
 *      Copyright (C) 2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *	                  EzADPCM - main.c
 *		            main routine
 *
 *	Version		Date		Design	Log
 *  --------------------------------------------------------------------
 *	0.10		Feb. 3, 2000	kaol
 *	0.11		Feb.27, 2000	kaol	BUFFER_SIZE is modified
 */

#include <eekernel.h>
#include <libsdr.h>
#include <sdrcmd.h>
#include <libdev.h>
#include <libpad.h>
#include "ezadpcm.h"

// IOP 側のバッファサイズ
#define BUFFER_SIZE      499712

#define _RUN  1
#define _STOP 0
#define _L 0
#define _R 1
#define _LR(x) ((x) << 16 | (x))

#define _VOLUME 0x3fff

char gFilename[2][64];		// 波形データファイル名

// in rpc.c
extern int ezAdpcm_RPCinit (void);
extern int ezAdpcm_RPC (int command, int arg);

//  IOP 使用モジュール
#define _MOD_ROOT    "host0:/usr/local/sce/iop/modules/"
#define _MOD_SIO2MAN "sio2man.irx"
#define _MOD_PADMAN  "padman.irx"
#define _MOD_LIBSD   "host0:/usr/local/sce/iop/modules/libsd.irx"
#define _MOD_EZADPCM "host0:/usr/local/sce/iop/sample/sound/ezadpcm/ezadpcm.irx"

u_long128 pad_dma_buf [scePadDmaBufferMax]  __attribute__((aligned (64)));

// モジュールのロード
void
load_modules (void)
{
    sceSifInitRpc (0);

    printf ("loading modules ...\n");

    // Pad用
    while (sceSifLoadModule (_MOD_ROOT _MOD_SIO2MAN, 0, NULL) < 0) {
	printf ("  loading %s failed\n", _MOD_SIO2MAN);
    }
    while (sceSifLoadModule (_MOD_ROOT _MOD_PADMAN, 0, NULL) < 0) {
	printf ("  loading %s failed\n", _MOD_PADMAN);
    }
    // libsd
    while (sceSifLoadModule(_MOD_LIBSD, 0, NULL) < 0) {
	printf("  loading libsd.irx failed\n");
    }
    // EzADPCM 用ドライバモジュール
    while (sceSifLoadModule (_MOD_EZADPCM, 0, NULL) < 0) {
	printf("  loading ezadpcm.irx failed\n");
    }

    printf ("loading modules ... done.\n");

    return;
}

// IOP 側のセットアップ:
//   ファイルオープン、ボイス指定、ボリューム設定など
int
init_adpcm (void)
{
    int info;

    if (ezAdpcm_RPCinit () < 0)
	return (-1);
    // 以下の 3 行の設定は libsdr を使っても可
    ezAdpcm_RPC (EzADPCM_SDINIT, SD_INIT_COLD); // = sceSdInit()
    ezAdpcm_RPC (SD_CORE_0 | EzADPCM_SETMASTERVOL, _LR (0x3fff)); // CORE0 master vol
    ezAdpcm_RPC (SD_CORE_1 | EzADPCM_SETMASTERVOL, _LR (0x3fff)); // CORE1 master vol

    // file open
    info = ezAdpcm_RPC (_L | EzADPCM_OPEN, (int) gFilename[_L]);
    printf ("  VB:%1d %s  %d byte(body)\n", _L, gFilename[_L], info);

    info = ezAdpcm_RPC (_R | EzADPCM_OPEN, (int) gFilename[_R]);
    printf ("  VB:%1d %s  %d byte(body)\n", _R, gFilename[_R], info);

    // メモリ確保
    ezAdpcm_RPC (EzADPCM_INIT, BUFFER_SIZE);

    // ボイス設定
    ezAdpcm_RPC (_L | EzADPCM_SETVOICE, 22); // ボイス 22
    ezAdpcm_RPC (_R | EzADPCM_SETVOICE, 23); // ボイス 23
    ezAdpcm_RPC (EzADPCM_SETVOL, _LR(_VOLUME));

    return 0;
}

// メインループ
//	コントローラ入力から IOP へ RPC
int
main_loop (void)
{
    int run_status, i, running = _STOP;
    u_int paddata, oldpad, ch;
    u_char rdata [32];
    int vol = _VOLUME;		// initial
    int isstop = 0;		// program termination

    oldpad = paddata = 0;
    // ch が不定
    while (1) {
	// Controller
#define _PAD_DATA (0xffff ^ ((rdata[2] << 8) | rdata[3]))
	if (scePadRead (0, 0, rdata) > 0) paddata = _PAD_DATA;
	else                              paddata = 0;

#ifdef DEBUG
	if (paddata != 0) printf ("%08x\n", paddata);
#endif
	// Controller operation
	if (paddata != oldpad) {
	    run_status = ezAdpcm_RPC (EzADPCM_GETSTATUS, 0);
	    if (paddata & SCE_PADstart) {
		// START/STOP
		if (running == _STOP) {
		    ezAdpcm_RPC (EzADPCM_PRELOADSTART, 0);
		    running = _RUN;
		} else {
		    if (run_status == EzADPCM_STATUS_RUNNING) {
			ezAdpcm_RPC (EzADPCM_STOP, 0);
			running = _STOP;
		    }
		}
	    } else if (paddata & SCE_PADselect) {
		// QUIT
		ezAdpcm_RPC (EzADPCM_SETVOL, 0);
		ezAdpcm_RPC (EzADPCM_STOP,   0);
		isstop ++;
	    }
	}
#define _VOL_DELTA 0x80
	if (paddata & SCE_PADLup) {
	    vol += _VOL_DELTA;	// volume up
	    if (vol > 0x3fff) vol = 0x3fff;
	    ezAdpcm_RPC (EzADPCM_SETVOLDIRECT, _LR (vol));
	} else if (paddata & SCE_PADLdown) {
	    vol -= _VOL_DELTA;	// volume down
	    if (vol < 0) vol = 0;
	    ezAdpcm_RPC (EzADPCM_SETVOLDIRECT, _LR (vol));
	}

	if (isstop) {
	    if (ezAdpcm_RPC (EzADPCM_GETSTATUS, 0) == EzADPCM_STATUS_IDLE) {
		ezAdpcm_RPC (EzADPCM_CLOSE, 0);
		ezAdpcm_RPC (SD_CORE_0 | EzADPCM_SETMASTERVOL, 0); // master vol core0
		ezAdpcm_RPC (SD_CORE_1 | EzADPCM_SETMASTERVOL, 0); // master vol core1
		ezAdpcm_RPC (EzADPCM_QUIT, 0);
		return;
	    }
	}
	VSync();
	oldpad = paddata;
    }
    return;
}


// スタートエントリ
int
main (int argc, char *argv [])
{

    // モジュールのロード
    load_modules ();

    scePadInit (0);
    scePadPortOpen (0, 0, pad_dma_buf);

    printf ("EzADPCM initialize ...\n");

    // 引数チェック
    if (argc != 3) {
	printf("\n\nusage: %s <L-ch file>.vb <R-ch file>.vb\n\n", argv [0]);
	return 0;
    }

    // 波形データ ファイル名
    strcpy (gFilename [_L], argv [_L + 1]);
    strcpy (gFilename [_R], argv [_R + 1]);

    // IOP 側のセットアップ
    init_adpcm ();

    printf ("EzADPCM initialize ... done\n");

    // 再生をコントロールする
    main_loop ();

    printf("EzADPCM finished...\n");
    return 0;
}

/* ----------------------------------------------------------------
 *	End on File
 * ---------------------------------------------------------------- */
/* This file ends here, DON'T ADD STUFF AFTER THIS */
