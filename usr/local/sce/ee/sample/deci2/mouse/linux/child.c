/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
// Version 0.00
// ASCII
// 
// Copyright (C) 1999 Sony Computer Entertainment Inc. All Rights Reserved.
// 
// Version        Date            Design      Log
// --------------------------------------------------------------------
// 0.00           10/13/99        koji        1st version

#include "main.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define WIDTH	640
#define HEIGHT	224*2

static Display *disp;
static Window  root_wd, wd;

static GC gcBlack, gcGreen;
XGCValues gv;
Colormap  cmap;
XColor    col;

void ChildProcess(int fd) {
	long b_px;
	XEvent event;
	int xyb[3];
	char buf[256];

	bzero(buf, sizeof(buf));
	if ((disp = XOpenDisplay(NULL)) == NULL) {
		fprintf(stderr, "Cannot Open Display!\n");
		exit(1);
	}

	b_px = BlackPixel(disp, 0);
	root_wd = DefaultRootWindow(disp);
	wd = XCreateSimpleWindow(disp, root_wd, 0, 0, WIDTH, HEIGHT, 2, b_px, b_px);
	XMapWindow(disp, wd);

	cmap = DefaultColormap(disp, 0);
	XParseColor(disp, cmap, "green", &col);
	XAllocColor(disp, cmap, &col);
	gcGreen = XCreateGC(disp, wd, 0, &gv);
	gcBlack  = XCreateGC(disp, wd, 0, &gv);
	XSetForeground(disp, gcBlack, b_px);
	XSetForeground(disp, gcGreen, col.pixel);

	XSelectInput(disp, wd,
		(ButtonPressMask|ButtonReleaseMask|PointerMotionMask|KeyPressMask));
	xyb[2] = 0;
	while (1) {
		XNextEvent(disp, &event);					// this would block
		if (strlen(buf))
			XDrawString(disp, wd, gcBlack, 5, 10, buf, strlen(buf)-1);
		switch (event.type) {
			case MotionNotify:
				xyb[0] = event.xmotion.x-WIDTH/2;
				xyb[1] = event.xmotion.y-HEIGHT/2;
				sprintf(buf, "MotionNotify x:% 04d, y:% 04d, b:%d\n", xyb[0],
						xyb[1], xyb[2]);
				break;
			case ButtonPress:
				xyb[0] = event.xbutton.x-WIDTH/2;
				xyb[1] = event.xbutton.y-HEIGHT/2;
				xyb[2] = event.xbutton.button;
				sprintf(buf, "ButtonPress x:% 04d, y:% 04d, b:%d\n", xyb[0],
						xyb[1], xyb[2]);
				break;
			case ButtonRelease:
				xyb[0] = event.xbutton.x-WIDTH/2;
				xyb[1] = event.xbutton.y-HEIGHT/2;
				xyb[2] &= ~event.xbutton.button;
				sprintf(buf, "ButtonRelease x:% 04d, y:% 04d, b:%d\n", xyb[0],
						xyb[1], xyb[2]);
				break;
			case KeyPress: exit(0);
			default:
				sprintf(buf, "Unknown Event\n");
				continue;
		}
		XDrawString(disp, wd, gcGreen, 5, 10, buf, strlen(buf)-1);
		write(fd, xyb, sizeof(xyb));
	}
}

// Local variables:
// tab-width: 4
// End:
// vi:set tabstop=4:

