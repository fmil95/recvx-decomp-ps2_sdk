/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *	    i.LINK socket Library Sample
 *
 *	                    Shift-JIS
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *	                 All Rights Reserved.
 *
 *                      socketee.c
 *
 *	Version		Date            Design	Log
 *  --------------------------------------------------------------------
 *	0.10		01/14/2000      sim	Initial
 *	1.02		04/22/2000      sim	add sceILsockSendNoWait()
 *	1.03		04/24/2000      sim	RPC DMA size optimizedÅiflags==0Ç»ÇÃÇ≈IOPÇ…ìnÇ≥Ç»Ç¢Åj
 *	1.04		05/09/2000      sim	RPCÇ…égÇ§ïœêîÇÇ∑Ç◊ÇƒstaticÇ…ÇµÇΩ 
 *	1.06		05/12/2000      sim	Selectable USE_CYCLETIMEV
 *  --------------------------------------------------------------------
 */
#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <eeregs.h>
#include <sifrpc.h>
#include <ilink.h>
#include <ilsock.h>
#include <ilsocksf.h>

//#define DEBUG_PRINT
#define DBGPRINTF   printf
//#define USE_CYCLETIMEV

typedef struct {
  sceSifClientData  cd_sceILsockInit;
  sceSifClientData  cd_sceILsockReset;
  sceSifClientData  cd_sceILsockOpen;
  sceSifClientData  cd_sceILsockClose;
  sceSifClientData  cd_sceILsockBind;
  sceSifClientData  cd_sceILsockConnect;
  sceSifClientData  cd_sceILsockSend;
  sceSifClientData  cd_sceILsockSendTo;
  sceSifClientData  cd_sceILsockRecv;
  sceSifClientData  cd_sceILsockRecvFrom;
  sceSifClientData  cd_sce1394SbEui64;
  sceSifClientData  cd_sce1394SbNodeId;
  sceSifClientData  cd_sce1394CycleTimeV;
} ILINK_SOCKET_SIF_MANAGER;

static ILINK_SOCKET_SIF_MANAGER sif;
static int isInit = 0;

static int BufInit[4] __attribute__((aligned(64)));
enum SceILsockErrorCode	sceILsockInit(int maxsock, int maxsize)
{
  int i;
  int ret;

  if (isInit)
  {
    printf( "WARRNING : sceILsockInit() already initialized\n" );
    return SCEILSOCKERR_OK;
  }

  /***** Bind sceSocket services *****/
  while( 1 )
  {
    ret = sceSifBindRpc( &sif.cd_sceILsockInit, SIFNUM_sceILsockInit, 0 );
    if( sif.cd_sceILsockInit.serve != 0 ) break;
    i = 0x10000;
    while( i-- );
  }

  while( 1 )
  {
    ret = sceSifBindRpc( &sif.cd_sceILsockReset, SIFNUM_sceILsockReset, 0 );
    if( sif.cd_sceILsockReset.serve != 0 ) break;
    i = 0x10000;
    while( i-- );
  }

  while( 1 )
  {
    ret = sceSifBindRpc( &sif.cd_sceILsockOpen, SIFNUM_sceILsockOpen, 0 );
    if( sif.cd_sceILsockOpen.serve != 0 ) break;
    i = 0x10000;
    while( i-- );
  }

  while( 1 )
  {
    ret = sceSifBindRpc( &sif.cd_sceILsockClose, SIFNUM_sceILsockClose, 0 );
    if( sif.cd_sceILsockClose.serve != 0 ) break;
    i = 0x10000;
    while( i-- );
  }

  while( 1 )
  {
    ret = sceSifBindRpc( &sif.cd_sceILsockBind, SIFNUM_sceILsockBind, 0 );
    if( sif.cd_sceILsockBind.serve != 0 ) break;
    i = 0x10000;
    while( i-- );
  }

  while( 1 )
  {
    ret = sceSifBindRpc( &sif.cd_sceILsockConnect, SIFNUM_sceILsockConnect, 0 );
    if( sif.cd_sceILsockConnect.serve != 0 ) break;
    i = 0x10000;
    while( i-- );
  }

  while( 1 )
  {
    ret = sceSifBindRpc( &sif.cd_sceILsockSend, SIFNUM_sceILsockSend, 0 );
    if( sif.cd_sceILsockSend.serve != 0 ) break;
    i = 0x10000;
    while( i-- );
  }

  while( 1 )
  {
    ret = sceSifBindRpc( &sif.cd_sceILsockSendTo, SIFNUM_sceILsockSendTo, 0 );
    if( sif.cd_sceILsockSendTo.serve != 0 ) break;
    i = 0x10000;
    while( i-- );
  }

  while( 1 )
  {
    ret = sceSifBindRpc( &sif.cd_sceILsockRecv, SIFNUM_sceILsockRecv, 0 );
    if( sif.cd_sceILsockRecv.serve != 0 ) break;
    i = 0x10000;
    while( i-- );
  }

  while( 1 )
  {
    ret = sceSifBindRpc( &sif.cd_sceILsockRecvFrom, SIFNUM_sceILsockRecvFrom, 0 );
    if( sif.cd_sceILsockRecvFrom.serve != 0 ) break;
    i = 0x10000;
    while( i-- );
  }

  while( 1 )
  {
    ret = sceSifBindRpc( &sif.cd_sce1394SbEui64, SIFNUM_sce1394SbEui64, 0 );
    if( sif.cd_sce1394SbEui64.serve != 0 ) break;
    i = 0x10000;
    while( i-- );
  }

  while( 1 )
  {
    ret = sceSifBindRpc( &sif.cd_sce1394SbNodeId, SIFNUM_sce1394SbNodeId, 0 );
    if( sif.cd_sce1394SbNodeId.serve != 0 ) break;
    i = 0x10000;
    while( i-- );
  }

#ifdef USE_CYCLETIMEV
  while( 1 )
  {
    ret = sceSifBindRpc( &sif.cd_sce1394CycleTimeV, SIFNUM_sce1394CycleTimeV, 0 );
    if( sif.cd_sce1394CycleTimeV.serve != 0 ) break;
    i = 0x10000;
    while( i-- );
  }
#endif

  /***** execute sceILsockInit() *****/

  BufInit[0] = maxsock;
  BufInit[1] = maxsize;
  ret = sceSifCallRpc( &sif.cd_sceILsockInit, SIFNUM_sceILsockInit, 0,
    (void*)BufInit, 16, (void*)BufInit, 16, 0, 0 );

  if( ret < 0 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockInit(): rpc error\n" );
#endif
    return ret;
  }

  isInit = 1;

#ifdef DEBUG_PRINT
  DBGPRINTF( "sceILsockInit( maxsock=%d, maxsize=%d ) = %d\n", maxsock, maxsize, BufInit[0] );
#endif

  return BufInit[0];
}

void sceILsockReset(void)
{
  int ret;

  ret = sceSifCallRpc( &sif.cd_sceILsockReset, SIFNUM_sceILsockReset, 0,
    "", 0, "", 0, 0, 0 );

  if( ret < 0 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockReset(): rpc error\n" );
#endif
  }
}

static int BufOpen[4] __attribute__((aligned(64)));
int sceILsockOpen(int domain, int type, int protocol)
{
  int ret;

  BufOpen[0] = domain;
  BufOpen[1] = type;
  BufOpen[2] = protocol;
  ret = sceSifCallRpc( &sif.cd_sceILsockOpen, SIFNUM_sceILsockOpen, 0,
    (void*)BufOpen, 16, (void*)BufOpen, 16, 0, 0 );

  if( ret < 0 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockOpen(): rpc error\n" );
#endif
    return ret;
  }

  return BufOpen[0];
}

static int BufClose[4] __attribute__((aligned(64)));
enum SceILsockErrorCode	sceILsockClose(int sock)
{
  int ret;

  BufClose[0] = sock;
  ret = sceSifCallRpc( &sif.cd_sceILsockClose, SIFNUM_sceILsockClose, 0,
    (void*)BufClose, 16, (void*)BufClose, 16, 0, 0 );

  if( ret < 0 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockClose(): rpc error\n" );
#endif
    return ret;
  }

  return BufClose[0];
}

static int BufBind[8] __attribute__((aligned(64)));
enum SceILsockErrorCode	sceILsockBind(int sock, struct sceILsock_addr *name, int namelen)
{
  int ret;

  if( namelen != 16 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockBind(): namelen invalid size\n" );
#endif
    return SCEILSOCKERR_INVALID_SIZE;
  }

  BufBind[0] = sock;
  BufBind[1] = namelen;
  memcpy( &BufBind[2], name, namelen );

  ret = sceSifCallRpc( &sif.cd_sceILsockBind, SIFNUM_sceILsockBind, 0,
    (void*)BufBind, 32, (void*)BufBind, 16, 0, 0 );

  if( ret < 0 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockBind(): rpc error\n" );
#endif
    return ret;
  }

  return BufBind[0];
}

static int BufCnct[8] __attribute__((aligned(64)));
enum SceILsockErrorCode	sceILsockConnect(int sock, struct sceILsock_addr *name, int namelen)
{
  int ret;

  if( namelen > 16 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockConnect(): namelen too long\n" );
#endif
    return SCEILSOCKERR_INVALID_SIZE;
  }

  BufCnct[0] = sock;
  BufCnct[1] = namelen;
  memcpy( &BufCnct[2], name, namelen );

  ret = sceSifCallRpc( &sif.cd_sceILsockConnect, SIFNUM_sceILsockConnect, 0,
    (void*)BufCnct, 32, (void*)BufCnct, 16, 0, 0 );

  if( ret < 0 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockConnect(): rpc error\n" );
#endif
    return ret;
  }

  return BufCnct[0];
}

static int BufSend[128] __attribute__((aligned(64)));
int sceILsockSend(int sock, char *buf, int len, int flags)
{
  int ret;

  if( len > SCEILSOCK_MAX_PAYLOAD_SIZE )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockSend(): data too long\n" );
#endif
    return SCEILSOCKERR_INVALID_SIZE;
  }

  if( sceSifCheckStatRpc((sceSifRpcData*)&sif.cd_sceILsockSend) == 1 )
    DBGPRINTF("line: %d in socketee.c, sceSifCheckStatRpc() == 1 in sceILsockSend(sock=%d)\n", __LINE__, sock );

  BufSend[0] = sock;
  BufSend[2] = len;
  memcpy( &BufSend[3], buf, len );

  ret = sceSifCallRpc( &sif.cd_sceILsockSend, SIFNUM_sceILsockSend, 0,
    (void*)BufSend, (len + 12 + 15) & 0xffff0, (void*)BufSend, 16, 0, 0 );

  if( ret < 0 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockSend(): rpc error\n" );
#endif
    return ret;
  }

  return BufSend[0]; // Txed data size in bytes
}

static int BufSendNW[128] __attribute__((aligned(64)));
int sceILsockSendNoWait(int sock, char *buf, int len, int flags)
{
  int ret;
  int limit = 1000000;

  if( len > SCEILSOCK_MAX_PAYLOAD_SIZE )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockSendNoWait(): data too long\n" );
#endif
    return SCEILSOCKERR_INVALID_SIZE;
  }

  while( limit-- > 0 && sceSifCheckStatRpc((sceSifRpcData*)&sif.cd_sceILsockSend) == 1 );
  if( limit < 0 ) DBGPRINTF("line: %d in socketee.c, sceSifCheckStatRpc() == 1 in sceILsockSendNoWait(sock=%d)\n", __LINE__, sock );

  BufSendNW[0] = sock;
  BufSendNW[2] = len;
  memcpy( &BufSendNW[3], buf, len );

  ret = sceSifCallRpc( &sif.cd_sceILsockSend, SIFNUM_sceILsockSend, SIF_RPCM_NOWAIT,
    (void*)BufSendNW, (len + 12 + 15) & 0xffff0, (void*)BufSendNW, 16, 0, 0 );

  if( ret < 0 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockSendNoWait(): rpc error\n" );
#endif
    return ret;
  }

  return BufSendNW[0]; // Txed data size in bytes
}

static int BufSendTo[128] __attribute__((aligned(64)));
int sceILsockSendTo(int sock, char *buf, int len, int flags, struct sceILsock_addr *to, int tolen)
{
  int ret;

  if( len > SCEILSOCK_MAX_PAYLOAD_SIZE )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockSendTo(): data size too long\n" );
#endif
    return SCEILSOCKERR_INVALID_SIZE;
  }

  if( tolen != 16 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockSendTo(): tolen invalid size\n" );
#endif
    return SCEILSOCKERR_INVALID_SIZE;
  }

  BufSendTo[0] = sock;
  memcpy( &BufSendTo[1], to, 12 ); // without sock_zero[4]
  BufSendTo[4] = len;
  memcpy( &BufSendTo[5], buf, len ); // data to be Txed

  ret = sceSifCallRpc( &sif.cd_sceILsockSendTo, SIFNUM_sceILsockSendTo, 0,
    (void*)BufSendTo, (len + 20 + 15) & 0xffff0, (void*)BufSendTo, 16, 0, 0 );

  if( ret < 0 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockSendTo(): rpc error\n" );
#endif
    return ret;
  }

  return BufSendTo[0];
}

static int BufRcv[128] __attribute__((aligned(64)));
int sceILsockRecv(int sock, char *buf, int len, int flags)
{
  int ret;

  if( len > SCEILSOCK_MAX_PAYLOAD_SIZE )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockRecv(): data too long\n" );
#endif
    return SCEILSOCKERR_INVALID_SIZE;
  }

  if( sceSifCheckStatRpc((sceSifRpcData*)&sif.cd_sceILsockRecv) == 1 )
    DBGPRINTF("line: %d in socketee.c, sceSifCheckStatRpc() == 1 in sceILsockRecv(sock=%d)\n", __LINE__, sock );

  BufRcv[0] = sock;
  BufRcv[1] = len;

  SyncDCache( (void*)BufRcv, (void*)BufRcv + len + 4 - 1 );

  ret = sceSifCallRpc( &sif.cd_sceILsockRecv, SIFNUM_sceILsockRecv, 0, // WAIT
    (void*)BufRcv, 16, (void*)BufRcv, (len + 4 + 15) & 0xffff0, 0, 0 );

  if( ret < 0 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockRecv(): rpc error\n" );
#endif
    return ret;
  }

  ret = BufRcv[0]; // Rxed data size in bytes
  if( 0 < ret && ret <= len ) // Rxed length is OK
    memcpy( buf, (char*)&BufRcv[1], ret );
  
  return ret; // actually Rxed size in bytes
}

static int BufRcvF[128] __attribute__((aligned(64)));
int sceILsockRecvFrom(int sock, char *buf, int len, int flags, struct sceILsock_addr *from, int *fromlen)
{
  int ret;
  int fromlen2;

  if( len > SCEILSOCK_MAX_PAYLOAD_SIZE )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockRecvFrom(): data too long\n" );
#endif
    return SCEILSOCKERR_INVALID_SIZE;
  }

  if( *fromlen != 16 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockRecvFrom(): *fromlen invalid size\n" );
#endif
    return SCEILSOCKERR_INVALID_SIZE;
  }

  if( sceSifCheckStatRpc((sceSifRpcData*)&sif.cd_sceILsockRecvFrom) == 1 )
    DBGPRINTF("line: %d in socketee.c, sceSifCheckStatRpc() == 1 in sceILsockRecvFrom(sock=%d)\n", __LINE__, sock );

  BufRcvF[0] = sock;
  BufRcvF[1] = len;

  SyncDCache( (void*)BufRcvF, (void*)BufRcvF + len + 20 - 1 );

  ret = sceSifCallRpc( &sif.cd_sceILsockRecvFrom, SIFNUM_sceILsockRecvFrom, 0,
    (void*)BufRcvF, 16, (void*)BufRcvF, (len + 20 + 15) & 0xffff0, 0, 0 );

  if( ret < 0 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sceILsockRecvFrom(): rpc error\n" );
#endif
    return ret;
  }

  ret = BufRcvF[4]; // Rxed size in bytes
  if( ret > 0 && ret <= len ) 
    memcpy( buf, (char*)&BufRcvF[5], ret );

  fromlen2 = BufRcvF[0]; // effective fromlen
  if( fromlen2 > 0 && fromlen2 <= 16 )
    memcpy( from, &BufRcvF[1], fromlen2 );
  *fromlen = fromlen2;

  return ret; // actually Rxed size in bytes
}

static int BufEui[4] __attribute__((aligned(64)));
enum SceILsockErrorCode sce1394SbEui64( unsigned int *buf )
{
  int ret;

  ret = sceSifCallRpc( &sif.cd_sce1394SbEui64, SIFNUM_sce1394SbEui64, 0,
    (void*)BufEui, 16, (void*)BufEui, 16, 0, 0 );

  if( ret < 0 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sce1394SbEui64(): rpc error\n" );
#endif
    return ret;
  }

  memcpy( buf, &BufEui[1], 8 );

  return BufEui[0];
}

static int BufSbNodeId[4] __attribute__((aligned(64)));
int sce1394SbNodeId()
{
  int limit = 1000000;

  while( limit-- > 0 && sceSifCheckStatRpc((sceSifRpcData*)&sif.cd_sce1394SbNodeId) == 1 );
  if( limit < 0 ) DBGPRINTF("line: %d in socketee.c, sceSifCheckStatRpc() == 1 in sce1394SbNodeId()\n", __LINE__ );

  sceSifCallRpc( &sif.cd_sce1394SbNodeId, SIFNUM_sce1394SbNodeId, SIF_RPCM_NOWAIT,
   "", 0, (void*)BufSbNodeId, 16, 0, 0 );

  return 0;
}

int sce1394GetMyNodeID()
{
  return BufSbNodeId[0] & 0x3f;
}

#ifdef USE_CYCLETIMEV
static int BufCycT[4] __attribute__((aligned(64)));
unsigned int sce1394CycleTimeV()
{
  int ret;

  ret = sceSifCallRpc( &sif.cd_sce1394CycleTimeV, SIFNUM_sce1394CycleTimeV, 0,
    "", 0, (void*)BufCycT, 16, 0, 0 );

  if( ret < 0 )
  {
#ifdef DEBUG_PRINT
    DBGPRINTF( "sce1394CycleTimeV(): rpc error\n" );
#endif
    return 0;
  }

  return BufCycT[0];
}
#endif
