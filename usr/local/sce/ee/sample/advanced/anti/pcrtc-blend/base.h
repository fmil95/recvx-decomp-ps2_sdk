/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
// VIFcode
#define V4_32   0x6c
#define V4_16   0x6d
#define S_32M   0x70
#define VIFNOP  0
// DMA code
#define DMArefe (0<<28)
#define DMAcnt  (1<<28)
#define DMAnext (2<<28)
#define DMAref  (3<<28)
#define DMArefs (4<<28)
#define DMAcall (5<<28)
#define DMAret  (6<<28)
#define DMAend  (7<<28)

#define PIf	3.14159f
// fractal
#define PN 	1
#define N 	8			/*  LM = 2 ^ N	 */
#define SN 	2			/*  LM = 2 ^ N	 */
#define LM	(1 << N)		/*  地形TEX配列の幅 */
#define SLM     (1 << (N-SN))		/*  地形Mesh配列の幅 */
#define MESH    (1 << (N-SN))		/*  地形Mesh配列の幅 */
#define VN 	((N-SN)-1)		/*  for OCTAMesh	*/
#define LVN	(1 << VN)		/*  for OCTAMesh	*/
#define HN 	(N-1)			/*  for OCTAMesh	*/
#define HLM	(1<<(N-1))		/*  for OCTAMesh	*/
#define TXWDTH	(LM)
#define TXW	(LM)
#define TXHIGH	(LM)
#define TH	((LM)+1)	// 8bit:TW=2*N 4bit:TW=4*N
#define TW	((LM)+2)        // 
#define HTH	((LM/2)+1)	// 8bit:HTW=2*N 4bit:HTW=4*N
#define HTW	((LM/2)+2)      // 
#define M	(N) //(N+1)
// GS Memory Map
#define DISPWIDTH 640
#define DISPHEIGHT 448
#define DBUF0	(0) 		/* Display&Work Image Area */
#define DBUF1	(140) 
#define ZBUF	(280)
#define INBUF	(420)*32	/* Input Image Area */
#define TXDP	(420)*32
#define TX8DP	(420)*32
#define CLTDP	(510)*32
#define CLTWDTH	16
#define TXWW	(640)

// GS Initial code
#define sceGsInterMode 0
#define GS_SMODE1        ((volatile u_long *)(0x12000010))
#define GS_SYNCH1        ((volatile u_long *)(0x12000040))
#define GS_SYNCH2        ((volatile u_long *)(0x12000050))
#define GS_SYNCV         ((volatile u_long *)(0x12000060))
#define GS_SRFSH         ((volatile u_long *)(0x12000030))
// Matrix 
#define SCALE		1.0
#define FogNear         (20000.0*SCALE)
#define FogFar          (40000.0*SCALE)
#define NEARZ           (64.0*SCALE)            //near clip Z, near Zbuf Z
#define FARZ            (200000.0*SCALE)        //far clip Z, far Zbuf Z
// PSMatrix
#define	ZMIN		0.0
#define	ZMAX		16000000.0 	//if(ZMAX==2^24=16777216.0) overflow? in float calculation
#define	SCRZ		512.0
#define OFX		2048.0
#define	OFY		2048.0

#define	ZA		((ZMAX*NEARZ-0)/(NEARZ-FARZ))
#define ZB 		(-(ZMAX*FARZ*NEARZ-0)/(NEARZ-FARZ))


// Data Struct

typedef struct {
	float vx;
	float vy;
	float vz;
	float vw;
} SVECTOR __attribute__((aligned(16)));

typedef struct {
	float vx;
	float vy;
	float vz;
	float vw;
	float nx;
	float ny;
	float nz;
	float nw;
	float u;
	float v;
	float n0;
	float n1;
	float r;
	float g;
	float b;
	float a;
} PVECTOR __attribute__((aligned(16)));

typedef struct {
	char	tex8[HTH][HTW] __attribute__((aligned(16)));	
} TEX8M __attribute__((aligned(16)));

typedef struct {
	PVECTOR vct[(MESH/2)+1][(MESH/2)+1] __attribute__((aligned(16)));	
} PVECTORM __attribute__((aligned(16)));

