/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                       libsdr - sdrcmd.h
 *                    common constant for libsdr
 *
 */

#ifndef _SDRCMD_H_
#define _SDRCMD_H_

/* ----------------------------------------------------------------
 *	COMMAND 
 * ---------------------------------------------------------------- */
#define rSdInit                  0x8000
#define rSdSetParam              0x8010
#define rSdGetParam              0x8020
#define rSdSetSwitch             0x8030
#define rSdGetSwitch             0x8040
#define rSdSetAddr               0x8050
#define rSdGetAddr               0x8060
#define rSdSetCoreAttr           0x8070
#define rSdGetCoreAttr           0x8080
#define rSdNote2Pitch            0x8090
#define rSdPitch2Note            0x80a0
#define rSdProcBatch             0x80b0
#define rSdProcBatchEx           0x80c0
#define rSdVoiceTrans            0x80d0
#define rSdBlockTrans            0x80e0
#define rSdVoiceTransStatus      0x80f0
#define rSdBlockTransStatus      0x8100
#define rSdSetTransCallback      0x8110
#define rSdSetIRQCallback        0x8120
#define rSdSetEffectAttr         0x8130
#define rSdGetEffectAttr         0x8140
#define rSdClearEffectWorkArea   0x8150
#define rSdSetTransIntrHandler   0x8160
#define rSdSetSpu2IntrHandler    0x8170

#define rSdChangeThreadPriority  0x8f10

/* ----------------------------------------------------------------
 *	End on File
 * ---------------------------------------------------------------- */
#endif /* _SDRCMD_H_ */
/* DON'T ADD STUFF AFTER THIS */
