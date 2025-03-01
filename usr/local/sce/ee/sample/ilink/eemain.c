/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              i.LINK socket Library Sample
 *
 *	                    Shift-JIS
 *
 *      Copyright (C) 2000 Sony Computer Entertainment Inc.
 *	                 All Rights Reserved.
 *
 *			    eemain.c
 *
 *	Version		Date		Design	Log
 *  --------------------------------------------------------------------
 *	1.0		05/12/2000	sim	1st version for Release 1.6
 *  --------------------------------------------------------------------
 */
#define SEND_NO_WAIT
#define NV_CNT		7 // number of packets Tx/Rx per V-frame

#include <eekernel.h>
#include <eetypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <libpkt.h>
#include <sifdev.h>
#include <libdev.h>
#include <sif.h>
#include <sifcmd.h>
#include <sifrpc.h>
#include <ilink.h>
#include <ilsock.h>

#define DBGPRINTF       printf

extern void sceDevConsClearBox(int cd, int x, int y, u_int w, u_int h);
extern void sceDevConsAttribute(int cd, u_char col);

#define SUNIT		0x01
#define PACKETSIZE	(0x100*SUNIT)
#define WORKSIZE      	(0x80)
#define WORKBASE      	0x70000000
#define SUPPER		(WORKBASE+WORKSIZE)
#define SLOWER		(WORKBASE+WORKSIZE+PACKETSIZE)

#define SCREEN_WIDTH    512
#define SCREEN_HEIGHT   224
#define OFFX		(((4096-SCREEN_WIDTH)/2)<<4)
#define OFFY		(((4096-SCREEN_HEIGHT)/2)<<4)
#define ConsW		(SCREEN_WIDTH/8) // consoleの最大文字数（横）
#define ConsH		26		 // consoleの最大文字数（縦）under scanしないと、27行はつらい。

/* フォント表示用 */
static int console;
extern void sceDevConsLocate(int cd, u_int lx, u_int ly);

/* 描画環境 */
static sceGsDBuff db;
static int Gs_Initialize(void);

// //////////////////////////////////////////////////////////////////////
// 		for DecCons
// //////////////////////////////////////////////////////////////////////
#define DevConsAttr_BLACK	0 // 0: 黒
#define DevConsAttr_BLUE	1 // 1: 青
#define DevConsAttr_RED		2 // 2: 赤
#define DevConsAttr_MAGENTA	3 // 3: マゼンタ
#define DevConsAttr_GREEN	4 // 4: 緑
#define DevConsAttr_CYAN	5 // 5: シアン
#define DevConsAttr_YELLOW	6 // 6: 黄
#define DevConsAttr_WHITE	7 // 7: 白

// //////////////////////////////////////////////////////////////////////
// 		for sceSocket
// //////////////////////////////////////////////////////////////////////
#define BASE_priority        100
#define PORT_RX           0x1234
#define TX_SIZE              492 // in bytes (= SCEILSOCK_MAX_PAYLOAD_SIZE)
#define N_RxBuf               16 // Rx FIFO size (in packets. MUST be 2^N)
#define PAYLOAD_SIZE	 TX_SIZE
#define NV_CNT_MAX             7 // max number of packets Tx/Rx per V-frame

static int MyThID;
static int sdBc; // socket discripter for Broadcasting
static int sdTx; // socket discripter for Transmitting data
static int sdRx; // socket discripter for Receiving data
static struct sceILsock_addr self;
static struct sceILsock_addr partner;
static struct sceILsock_addr everyone;
static sceILsock_addr_t myeui64[ 2 ];
static sceILsock_addr_t MyGUID[ 2 ]; // host byte order
static sceILsock_addr_t PartnerGUID[ 2 ]; // host byte order
static int MyNodeID;

static int TxData[ NV_CNT_MAX ][ TX_SIZE / 4 ];
static int RxData[ NV_CNT_MAX ][ TX_SIZE / 4 ];
static int RxBuf [ N_RxBuf ][ TX_SIZE / 4 ]; // Rx FIFO

static int MySyncStatus;
static int MyFrame, RxFrame;

static char Tx_mes_data[80];
static char Rx_mes_data[80];
static char title[80];

static int pRxBuf = 0;      // RxFIFO 書きこみindex
static int tRxBuf = 0;      // RxFIFO 取りだしindex
static int nRxBuf = 0;      // number of Rx data in RxBuf[]
static int nRemHDPkts = 0;  // number of Removed Half-done packets
static int nRetryCnt = 0;   // Tx-retry count

#define ILINKSYNC_SINGLE  0
#define ILINKSYNC_CONNECT 1
#define ILINKSYNC_RESET   2
#define ILINKSYNC_SYNC    3
static char* SyncStateMsg[4] =
{ "Single", "Connect", "Reset", "Sync" };

extern int sce1394GetMyNodeID();
extern int sceILsockSendNoWait(int sock, char *buf, int len, int flags);

static void RemoveHalfDonePackets()
{
  while( nRxBuf > 0 && RxBuf[tRxBuf][1] > 0 )
  {
    nRxBuf--;
    tRxBuf = (tRxBuf + 1) & (N_RxBuf - 1);
  }
}

int TakeoutRxData(char* p, int Npkt) // p に Npkt の RxData を置く
{
  int ret = 0;
  int i;
  char* q = p;

  ChangeThreadPriority( MyThID, BASE_priority - 2 ); // th_Receiver への切替を禁止
again:
  if( nRxBuf >= Npkt ) // enough packets ?
  {
    for( i = 0; i < Npkt; i++ )
    {
      if( RxBuf[tRxBuf][1] != i ) // Check serial
      {
        RemoveHalfDonePackets();
        nRemHDPkts++;
        goto again;
      }
      memcpy( q, (char*)RxBuf[tRxBuf], TX_SIZE );
      nRxBuf--;
      tRxBuf = (tRxBuf + 1) & (N_RxBuf - 1);
      q += TX_SIZE;
    }
    ret = Npkt;
  }    
  ChangeThreadPriority( MyThID, BASE_priority ); // th_Receiver への切替を許可
  return ret;
}

void ResetGS()
{
  DBGPRINTF( "RESET GS\n" );

  MyFrame = 0;

  /* フォントシステム終了 */
  if( console ) sceDevConsClose( console );

  /* GS初期化 */
  Gs_Initialize();

  /* フォントシステム初期化 */
  sceDevConsInit();
  console = sceDevConsOpen(OFFX + (8<<4), OFFY + (8<<4), ConsW, ConsH);

  sceGsSyncPath(0, 0);
}

void th_Receiver()
{
  int ret;
  int fromlen;
  struct sceILsock_addr sender;

  while( 1 )
  {
    if( MySyncStatus != ILINKSYNC_SINGLE )
    // 相手が見つかっている
    {
      ret = sceILsockRecv( sdRx, (char*)RxBuf[pRxBuf], TX_SIZE, 0 );
      if( ret == TX_SIZE ) // Rxed correctly
      {
        nRxBuf++;
        pRxBuf = (pRxBuf + 1) & (N_RxBuf - 1);
      } else {
        DBGPRINTF( "sceILsockRecv error. ret=%d\n", ret );
      }
    } else { // 相手がまだみつかっていない
      fromlen = sizeof( sender );
      ret = sceILsockRecvFrom( sdRx, (char*)RxBuf[pRxBuf], TX_SIZE, 0, &sender, &fromlen );
      if( ret == TX_SIZE )
      {
        if( (sender.sock_addr.eui64_hi == myeui64[0] && sender.sock_addr.eui64_lo == myeui64[1])
            ||  RxBuf[pRxBuf][1] != 0 ) 
        {
        // ignore Rx data sent from myself
        } else { // partner found
          partner.sock_addr.eui64_hi = sender.sock_addr.eui64_hi;
          partner.sock_addr.eui64_lo = sender.sock_addr.eui64_lo;
          PartnerGUID[0] = sceILsockNtoHl( partner.sock_addr.eui64_hi ); 
          PartnerGUID[1] = sceILsockNtoHl( partner.sock_addr.eui64_lo ); 
          ret = sceILsockConnect( sdTx, &partner, sizeof( partner ) ); // connect sdTx to PORT_RX of partner
          if( ret == SCEILSOCKERR_OK )
          {
            MySyncStatus = ILINKSYNC_CONNECT;
            nRxBuf++;
            pRxBuf = (pRxBuf + 1) & (N_RxBuf - 1);
            DBGPRINTF( "connected to partner %08x:%08x\n", PartnerGUID[0], PartnerGUID[1] );
          } else {
            DBGPRINTF( "line: %d, Error: %d at sceILsockConnect: sdTx partner %08x:%08x\n", 
	      __LINE__, ret, PartnerGUID[0], PartnerGUID[1] );
            PartnerGUID[0] = PartnerGUID[1] = 0;
          }
        }
      } else {
        DBGPRINTF( "sceILsockRecvFrom error. ret=%d\n", ret );
      }
    }
  }
}

void sceSocketPrepare()
{
  int id;
  int ret;
  int retry;
  static struct ThreadParam pt;
  static char stack[ 8192 ] __attribute__ ((aligned(16)));

  // 受信スレッドよりもメインの優先度を下げる
  MyThID = GetThreadId();
  ChangeThreadPriority( MyThID, BASE_priority );

  sdRx = sceILsockOpen( SCEILSOCK_PF, SCEILSOCK_DGRAM, 0 ); // for Receive
  sdTx = sceILsockOpen( SCEILSOCK_PF, SCEILSOCK_DGRAM, 0 ); // for Transmit
  sdBc = sceILsockOpen( SCEILSOCK_PF, SCEILSOCK_DGRAM, 0 ); // for Broadcast

  self.sock_len = sizeof( self );
  self.sock_family = SCEILSOCK_AF;
  self.sock_port = sceILsockHtoNs( PORT_RX );
  self.sock_addr.eui64_hi = SCEILSOCK_ADDR_ANY_HI;
  self.sock_addr.eui64_lo = SCEILSOCK_ADDR_ANY_LO;

  partner.sock_len = sizeof( partner );
  partner.sock_family = SCEILSOCK_AF;
  partner.sock_port = sceILsockHtoNs( PORT_RX );

  everyone.sock_len = sizeof( everyone );
  everyone.sock_family = SCEILSOCK_AF;
  everyone.sock_port = sceILsockHtoNs( PORT_RX );
  everyone.sock_addr.eui64_hi = SCEILSOCK_ADDR_BROADCAST_HI;
  everyone.sock_addr.eui64_lo = SCEILSOCK_ADDR_BROADCAST_LO;

  ret = SCEILSOCKERR_ERROR;
  for( retry = 0; retry < 5 && ret != SCEILSOCKERR_OK; retry++ )    
    ret = sceILsockBind( sdRx, &self, sizeof( self ) ); // bind sdRx to PORT_RX
  if( retry >= 5 )
    DBGPRINTF( "eemain.c line:%d, Error: %d at sceILsockBind: sdRx self\n", __LINE__, ret );

  self.sock_port = 0; // SCEILSOCK_PORT_ANON

  ret = SCEILSOCKERR_ERROR;
  for( retry = 0; retry < 5 && ret != SCEILSOCKERR_OK; retry++ )    
    ret = sceILsockBind( sdTx, &self, sizeof( self ) ); // bind sdTx to SCEILSOCK_PORT_ANON
  if( retry >= 5 )
    DBGPRINTF( "eemain.c line:%d, Error: %d at sceILsockBind: sdTx self\n", __LINE__, ret );

  ret = SCEILSOCKERR_ERROR;
  for( retry = 0; retry < 5 && ret != SCEILSOCKERR_OK; retry++ )    
    ret = sceILsockBind( sdBc, &self, sizeof( self ) ); // bind sdBc to SCEILSOCK_PORT_ANON
  if( retry >= 5 )
    DBGPRINTF( "eemain.c line:%d, Error: %d at sceILsockBind: sdBc self\n", __LINE__, ret );

  ret = SCEILSOCKERR_ERROR;
  for( retry = 0; retry < 5 && ret != SCEILSOCKERR_OK; retry++ )    
    ret = sceILsockConnect( sdBc, &everyone, sizeof( everyone ) ); // connect sdBc to PORT_RX of receiver
  if( retry >= 5 )
    DBGPRINTF( "eemain.c line:%d, Error: %d at sceILsockConnect: sdBc everyone\n", __LINE__, ret );

  pt.entry = (void*)th_Receiver;
  pt.stack = stack;
  pt.stackSize = sizeof( stack );
  pt.initPriority = BASE_priority - 1;
  pt.gpReg = &_gp;

  id = CreateThread( &pt );
  if( id >= 0 )
  {
    StartThread( id, NULL );
  } else {
    DBGPRINTF( "eemain.c line:%d, Error at Create thread: th_Receiver", __LINE__ );
  }
}

/* GS初期化 */
static int Gs_Initialize(void)
{
  int sindex;
  sceVif1Packet packet[2];
  sceDmaEnv env;
  sceDmaChan *p1;
  u_long giftagAD[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1),
    0x000000000000000eL };
  sceDevVif0Reset();
  sceDevVu0Reset();
  sceGsResetPath();
  sceDmaReset(1);

  sceVif1PkInit(&packet[0], (u_long128 *)SUPPER);
  sceVif1PkInit(&packet[1], (u_long128 *)SLOWER);

  sceDmaGetEnv(&env);
  env.notify = 1<<SCE_DMA_VIF1; /* notify channel */
  sceDmaPutEnv(&env);

  p1 = sceDmaGetChan(SCE_DMA_VIF1);
  p1->chcr.TTE = 1;

  sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
  sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT,
    SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);

  db.clear0.rgbaq.R = 0x40;
  db.clear0.rgbaq.G = 0x40;
  db.clear0.rgbaq.B = 0x80;

  db.clear1.rgbaq.R = 0x40;
  db.clear1.rgbaq.G = 0x40;
  db.clear1.rgbaq.B = 0x80;

  FlushCache(0);

  sindex = 0;
  sceVif1PkReset(&packet[sindex]);
  sceVif1PkCnt(&packet[sindex], 0);
  sceVif1PkOpenDirectCode(&packet[sindex], 0);
  sceVif1PkOpenGifTag(&packet[sindex], *(u_long128*)&giftagAD[0]);

  sceVif1PkReserve(&packet[sindex],
    sceGsSetDefAlphaEnv((sceGsAlphaEnv *)packet[sindex].pCurrent,0) * 4);
  sceVif1PkCloseGifTag(&packet[sindex]);
  sceVif1PkCloseDirectCode(&packet[sindex]);
  sceVif1PkEnd(&packet[sindex], 0);
  sceVif1PkTerminate(&packet[sindex]);

  /* kick Gs initialize packet */
  sceDmaSend(p1,(u_int *)(((u_int)packet[sindex].pBase&0x0fffffff)|0x80000000));

  /* display next in odd field when interlace */
  while(!sceGsSyncV(0));
}

static char errmsg[80];
int LoadModuleChk( char* path )
{
  if( sceSifLoadModule( path, 0, "" ) < 0 )
  {
    sprintf( errmsg, "Load module failed:\n%s\n", path );
    return 1;
  }
  return 0;
}

int main(void)
{
  int i;
  int ret;
  
  sceSifInitRpc( 0 );

  /* load ilink.irx */
  if( LoadModuleChk( "host0:/usr/local/sce/iop/modules/ilink.irx" ) ) goto DISP_ERROR;

  /* load ilsock.irx */
  if( LoadModuleChk( "host0:/usr/local/sce/iop/modules/ilsock.irx" ) ) goto DISP_ERROR;

  /* load ilsample.irx */
  if( LoadModuleChk( "host0:/usr/local/sce/iop/sample/ilink/ilsample.irx" ) ) goto DISP_ERROR;

  /* i.LINK socket初期化 */
  ret = sceILsockInit( 3, PAYLOAD_SIZE );
  if( ret != SCEILSOCKERR_OK )
  {
    sprintf( errmsg, "eemain.c line: %d, Error: %d at sceILsockInit\n", __LINE__, ret );
    goto DISP_ERROR;
  }

  sce1394SbEui64( MyGUID );
  myeui64[0] = sceILsockHtoNl( MyGUID[0] );
  myeui64[1] = sceILsockHtoNl( MyGUID[1] );
  sceSocketPrepare();

  /* make title on screen */
  sprintf( title, "TxFrm " );
  sprintf( title + strlen( title ), "nRx RxFrm " );
  for( i = 1; i <= NV_CNT; i++ )
    sprintf( title + strlen( title ), "Sum%d ", i );

  ResetGS();
  sceGsSyncPath( 0, 0 );
  while( 1 )
  {
    int i;

    sceGsSwapDBuff( &db, MyFrame );
    MyFrame++;

    sce1394SbNodeId(); /* NOWAITでSIFNUMsce1394SbNodeId()をcall。結果は後でsce1394GetMyNodeID()で読む。*/

    /* Setup TxData */
    sprintf( Tx_mes_data, "%02d:%02d ", (MyFrame / 60) % 100, MyFrame % 60 );

    for( i = 0; i < NV_CNT; i++ )
    {
      int j;
      unsigned int sum = 0;

      TxData[i][0] = MyFrame;
      TxData[i][1] = i; // fragment serial number
      for( j = 2; j < TX_SIZE / 4 - 1; j++ )
      {
        TxData[i][j] = (MyFrame << 16) | (i << 8) | j;
        sum += TxData[i][j];
      }
      TxData[i][TX_SIZE/4-1] = sum;
    }

    // Send TxData
    if( MySyncStatus == ILINKSYNC_CONNECT )
    {  // partner is valid
      for( i = 0; i < NV_CNT; i++ )
      {
#ifdef SEND_NO_WAIT
        sceILsockSendNoWait( sdTx, (char*)TxData[i], TX_SIZE, 0 ); // send & NO_WAIT
#else
retry:
        ret = sceILsockSend( sdTx, (char*)TxData[i], TX_SIZE, 0 ); // send & WAIT
        if( ret != TX_SIZE )
        {
          DBGPRINTF( "eemain.c line: %d, Error: %d at sceILsockSend\n", __LINE__, ret ); 
          if( ret != SCE1394ERR_RETRY_LIMIT )
          {
            nRetryCnt++;
            goto retry;
          }
        }
#endif
      }
    } else { // partner not found
      for( i = 0; i < NV_CNT; i++ )
      {
        ret = sceILsockSend( sdBc, (char*)TxData[i], TX_SIZE, 0 ); // broadcast
        if( ret != TX_SIZE )
          DBGPRINTF( "eemain.c line: %d, Error: %d at sceILsockSend\n", __LINE__, ret ); 
      }
    }

    MyNodeID = sce1394GetMyNodeID();

    ret = TakeoutRxData( (char*)RxData, NV_CNT ); // take out NV_CNT packets from RxBuf[]
    if( ret == 0 )
    { // no packet received
      strcpy( Rx_mes_data, "" );
    } else {
      int i, j;
      unsigned int sum = 0;

      RxFrame = RxData[0][0];
      sprintf( Rx_mes_data, "%02d:%02d ", (RxFrame / 60) % 100, RxFrame % 60 );
      for( i = 0; i < NV_CNT; i++ )
      {
        sum = 0;
        for( j = 2; j < TX_SIZE / 4 - 1; j++ ) sum += RxData[i][j];
        if( sum != 0 && RxData[i][TX_SIZE/4-1] == sum )
	{
          sprintf( Rx_mes_data + strlen( Rx_mes_data ), " ok  " );
	} else {
          sprintf( Rx_mes_data + strlen( Rx_mes_data ), "*NG* " );
        }
      }
    }

    /* フォント表示 */
    sceDevConsLocate( console, 0, ConsH );
    sceDevConsAttribute( console, DevConsAttr_WHITE );
    sceDevConsPrintf(console, "%s%3d %s\n", Tx_mes_data, nRxBuf, Rx_mes_data );

    sceDevConsClearBox( console, 0, 0, ConsW, 5 );
    sceDevConsLocate( console, 0, 0 );
    sceDevConsPrintf( console, "i.LINK sample(v1.0 %s %s) NodeID=%2d\n", 
      __DATE__, __TIME__, MyNodeID );
    sceDevConsPrintf( console, "Src=%08x:%08x, Dest=%08x:%08x\n",
      MyGUID[0], MyGUID[1], PartnerGUID[0], PartnerGUID[1] ); 
    sceDevConsPrintf(console, "Status: %s, ", 
      SyncStateMsg[MySyncStatus] );
    sceDevConsPrintf( console, "Elapsed: %04dh%02dm%02ds%02d\n",
      (MyFrame / (60*60*60)), (MyFrame / (60*60)) % 60, (MyFrame / 60) % 60, MyFrame % 60 );
#ifdef SEND_NO_WAIT
    sceDevConsPrintf( console, "nRemHDPkt=%d\n", nRemHDPkts );
#else
    sceDevConsPrintf( console, "nRemHDPkt=%d, nRetryCnt=%d\n", nRemHDPkts, nRetryCnt );
#endif
    sceDevConsPrintf( console, "%s\n", title );

    sceDevConsDraw( console );

    if( MyFrame & 1 )
    {	/* interrace half pixcel adjust */
      sceGsSetHalfOffset( &db.draw1, 2048, 2048, sceGsSyncV( 0 ) ^ 0x01 );
    } else {
      sceGsSetHalfOffset( &db.draw0, 2048, 2048, sceGsSyncV( 0 ) ^ 0x01 );
    }
    FlushCache( 0 );
    sceGsSyncPath( 0, 0 );
  }

  while( 1 );

/***************************************************************/

DISP_ERROR:
  ResetGS();
  sceGsSyncPath( 0, 0 );
  while( 1 )
  {
    sceGsSwapDBuff( &db, MyFrame );
    MyFrame++;

    /* フォント表示 */
    sceDevConsLocate( console, 0, 0 );
    sceDevConsAttribute( console, DevConsAttr_YELLOW );
    sceDevConsPrintf(console, "%s", errmsg );
    sceDevConsDraw( console );

    if( MyFrame & 1 )
    {	/* interrace half pixcel adjust */
      sceGsSetHalfOffset( &db.draw1, 2048, 2048, sceGsSyncV( 0 ) ^ 0x01 );
    } else {
      sceGsSetHalfOffset( &db.draw0, 2048, 2048, sceGsSyncV( 0 ) ^ 0x01 );
    }
    FlushCache( 0 );
    sceGsSyncPath( 0, 0 );
  }

  return 0;
}
