/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *                      Emotion Engine Library
 *                          Version 0.1.0
 * 
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 * 
 *                         client.c
 * 
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.1.0
 */

#include <stdio.h>
#include <kernel.h>

/* �������ɏ풓���Ȃ��Ƃ�, �ȉ��̂悤�Ƀ��W���[������
 * ���W���[���o�[�W������t���邱�Ƃ͉\�ł��B
 */
ModuleInfo Module = {"myprogram", 0x0101 };

extern void libentry1(int i);
extern void libentry2(int i);

int main(int argc, char *argv[])
{
    printf("Resident library call sample\n");
    libentry1(argc);
    libentry2(argc*2);
    return 0;
}
