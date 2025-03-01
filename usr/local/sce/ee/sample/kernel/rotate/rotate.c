/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *               Emotion Engine Library Sample Program
 *
 *                           - <rotate> -
 *
 *                          Version <1.00>
 *                               SJIS
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                            <rotate.c>
 *                 <rotation of thread ready queue>
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
#define PRINT_THREAD_PRIO 1

#define COUNT 4

int main_thread_id;
int print_thread1_id;
int print_thread2_id;
int print_thread3_id;
char print_thread1_stack[STACK_SIZE] __attribute__((aligned(16)));
char print_thread2_stack[STACK_SIZE] __attribute__((aligned(16)));
char print_thread3_stack[STACK_SIZE] __attribute__((aligned(16)));

int CreateStartThread( void (*entry)( void* ), void *stack, int stacksize, int prio,  void *arg );
void PrintThread(void *p);

int main(void){

  sceSifInitRpc(0);

  main_thread_id = GetThreadId();

  print_thread1_id = CreateStartThread(PrintThread, print_thread1_stack, sizeof(print_thread1_stack), PRINT_THREAD_PRIO, NULL);
    print_thread2_id = CreateStartThread(PrintThread, print_thread2_stack, sizeof(print_thread2_stack), PRINT_THREAD_PRIO, NULL);
    print_thread3_id = CreateStartThread(PrintThread, print_thread3_stack, sizeof(print_thread3_stack), PRINT_THREAD_PRIO, NULL);
  

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

/* PrintThread
 * デバッグコンソールに文字列を出力する関数
 * 
 *
 */
void PrintThread(void *p){
  int id = GetThreadId();
  int cnt = COUNT;
  while(cnt--){
    printf("Print Thread Running ID=%d\n",id);
            RotateThreadReadyQueue(PRINT_THREAD_PRIO);
  }
  ExitDeleteThread();
}
