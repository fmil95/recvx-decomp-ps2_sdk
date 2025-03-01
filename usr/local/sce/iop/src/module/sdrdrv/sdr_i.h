/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                       libsdr - sdr_i.h
 *                    internal headder for libsdr
 *
 *   Version   Date            Design    Log
 *  --------------------------------------------------------------------
 *   0.60      Oct.12.1999     morita    first checked in
 */


/* ----------------------------------------------
   moduke ID number
   最上位ビットが1のIDはSCE R&D専用。
   カスタマイズした場合は変更すること。
  -----------------------------------------------*/
#define sce_SDR_DEV   0x80000701
#define sce_SDRST_CB  0x80000704

extern int  _sce_sdrDMA0CallBackProc( void* common );
extern int  _sce_sdrDMA1CallBackProc( void* common );
extern int  _sce_sdrIRQCallBackProc( void* common );

extern int _sce_sdrDMA0IntrHandler (int core, void *common);
extern int _sce_sdrDMA1IntrHandler (int core, void *common);
extern int _sce_sdrSpu2IntrHandler (int core_bit, void *common);

//--- コールバックのステータス受け渡し用
typedef struct {
    int		mode;	
    int		voice_bit;
    int		status;	
    int		opt;
    int         pad[12];  // キャッシュラインサイズ(64Byte)になるように
} SdrEECBData;

extern int initial_priority_main;	// sdr module main thread priority
extern int initial_priority_cb;		// sdr callback thread priority

//#define PRINTF(x) printf x
#define PRINTF(x) 

#define PRIORITY_MODULE_SDRDRV 24
