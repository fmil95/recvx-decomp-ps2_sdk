/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
/*scale.h*/
#define SCALE		1.0

#define ROAD_HIGHT      (200*SCALE)
#define ROAD_WIDTH      (800.0*SCALE)

#define TEDGE           (20000.0*SCALE)
#define TEDGEH		((int)(TEDGE*0.5))	//10000		//(int)TEDGE/2
#define TERX            (-TEDGE*1.5)
#define TERZ            (-TEDGE*1.5)
#define MOUNT_MAX	((int)(5000*SCALE))

#define WATER_LEVEL	(-600.0*SCALE)

#define FogNear         (40000.0*SCALE)
#define FogFar          (60000.0*SCALE)
#define NEARZ           (64.0*SCALE)            //near clip Z, near Zbuf Z
#define FARZ            (200000.0*SCALE)        //far clip Z, far Zbuf Z

#define CLCENTX		((int)(50000*SCALE))
#define CLCENTY		((int)(-1500*SCALE))
#define CLCENTZ		((int)(-10000*SCALE))

#define GSPEED		(800.0*SCALE)

#define WPOSX		(0.0*SCALE)
#define WPOSY		(200.0*SCALE)
#define WPOSZ		(2500.0*SCALE)

#define VIEWX		(0.0*SCALE)
#define VIEWY		(-2000.0*SCALE)
#define VIEWZ		(-2500.0*SCALE)

#define TARGETX		(0.0*SCALE)
#define TARGETY		(-1000.0*SCALE)
#define TARGETZ		(0.0*SCALE)

#define VHEIGHT		(350.0*SCALE)

#define CLOUDX		(4000.0*SCALE)
#define CLOUDY		(1000.0*SCALE)
#define CLOUDZ		(4000.0*SCALE)

#define CLOUD_SPEED	(5.0*SCALE)
#define CLOUD_SHAPE	(1000.0*SCALE)

#define DVHEIGHT	(50.0*SCALE)
#define DCLCENTZ	(100.0*SCALE)

#define SNOWLEVEL	(4000.0*SCALE)

#define GRADRAND	((int)(2000*SCALE))

#define TARGETDIST      (5000.0*SCALE)

#define CLIPZ1          (-1500.0*SCALE)
#define CLIPZ2          (1000.0*SCALE)

#define CONSTRUCT	(200.0*SCALE)

#define BBLOCKH		(2000.0*SCALE)
