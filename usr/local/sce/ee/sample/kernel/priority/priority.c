/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *               Emotion Engine Library Sample Program
 *
 *                           - <priority> -
 *
 *                          Version <1.00>
 *                               SJIS
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                            <priority.c>
 *                      <thread preempt sample>
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      1.00            Jul,31,2000     kumagae     first version
 */

#include <stdio.h>
#include <stdlib.h>
#include <libgraph.h>
#include <sifdev.h>
#include <assert.h>
#include <sifrpc.h>
#include <sifcmd.h>


#define STACK_SIZE 4096
#define MAIN_THREAD_PRIO 10
#define READ_THREAD_PRIO 1
#define PRINT_THREAD_PRIO 2
#define FILENAME "host0:priority.elf"

int main_thread_id;
int print_thread_id;
int read_thread_id;
char print_thread_stack[STACK_SIZE] __attribute__((aligned(16)));
char read_thread_stack[STACK_SIZE] __attribute__((aligned(16)));

int ReadEndSema;
int CreateStartThread( void (*entry)( void* ), void *stack, int stacksize, int prio,  void *arg );
int GetFileSize(int fd);
void ReadThread(void *p);
void PrintThread(void *p);

int main(void){
  struct SemaParam sp;
  
  sp.initCount = 0;
  sp.maxCount = 1;  
  ReadEndSema = CreateSema(&sp);
 
  main_thread_id = GetThreadId();

  print_thread_id = CreateStartThread(PrintThread, print_thread_stack, sizeof(print_thread_stack), PRINT_THREAD_PRIO, NULL);
    read_thread_id = CreateStartThread(ReadThread, read_thread_stack, sizeof(read_thread_stack), READ_THREAD_PRIO, FILENAME);
  
    ChangeThreadPriority(main_thread_id, MAIN_THREAD_PRIO);
    
    sceSifExitCmd();

  return 0;
}

/* CreateStartThread
 * スレッドを作成し,起動します。
 * 
 * void (*entry)(void*) 実行を開始する関数のアドレス
 * void *stack スレッドが使用するスタック領域
 * int stacksize スタックサイズ
 * int prio 優先度
 * void *arg 引数
 *
 * 戻り値 スレッドID
 */
int CreateStartThread( void (*entry)( void* ), void *stack, int stacksize, int prio,  void *arg )
{
  struct ThreadParam tp;
  int id;
  tp.entry = entry;
  tp.stack = stack;
  tp.stackSize = stacksize;
  tp.initPriority = prio;
  tp.gpReg = &_gp;
  id = CreateThread(&tp);
  StartThread(id,arg);
  return id;
}

/* GetFileSize
 * ファイルのサイズを返す関数
 *
 * int fd ファイル識別子
 *
 * 戻り値 ファイルサイズ
 */
int GetFileSize(int fd)
{
  int curpos;
  int filesize;
  assert(fd >= 0);
  if((curpos = sceLseek(fd, 0, SCE_SEEK_CUR))<0)
    return 0;
  if((filesize = sceLseek(fd, 0, SCE_SEEK_END)) < 0)
    return 0;
  if(sceLseek(fd, curpos, SCE_SEEK_SET)< 0)
    return 0;
  return filesize;
}
/* ReadThread
 * ファイルを読む関数
 *
 * void *p ファイル名へのポインタ
 */
void ReadThread(void *p)
{
  char *pData = NULL;
  char *pfilename = (char*)p;
  int filesize;
  int fd;
  assert(pfilename);
  printf("Read Thread Running Priority=%d\n", READ_THREAD_PRIO);
  printf("Read Thread File Open\n");
  if((fd = sceOpen(pfilename, SCE_RDONLY)) <0){
    printf("Cannot open %s\n", pfilename);
  }else{
    filesize = GetFileSize(fd);
    pData = malloc(filesize);
    if(pData!=NULL){
      printf("Read Thread File Read Start\n");
      if(sceRead(fd, pData, filesize) < 0){
	printf("sceRead fail(%s)\n",pfilename);
      }
      printf("Read Thread File Read End\n");
    }
    if(pData != NULL)
      free(pData); 
    if(fd >= 0)
      sceClose(fd);
  }
  SignalSema(ReadEndSema);
  ExitDeleteThread();
}

/* PrintThread
 * デバッグコンソールに文字列を出力する関数
 * 
 *
 */
void PrintThread(void* p)
{
  while(PollSema(ReadEndSema)!=ReadEndSema){
    printf("Print Thread Running Priority=%d\n",PRINT_THREAD_PRIO);
    sceGsSyncV(0);
  }
  ExitDeleteThread();
}
