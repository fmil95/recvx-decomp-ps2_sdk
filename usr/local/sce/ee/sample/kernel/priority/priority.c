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
 * �X���b�h���쐬��,�N�����܂��B
 * 
 * void (*entry)(void*) ���s���J�n����֐��̃A�h���X
 * void *stack �X���b�h���g�p����X�^�b�N�̈�
 * int stacksize �X�^�b�N�T�C�Y
 * int prio �D��x
 * void *arg ����
 *
 * �߂�l �X���b�hID
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
 * �t�@�C���̃T�C�Y��Ԃ��֐�
 *
 * int fd �t�@�C�����ʎq
 *
 * �߂�l �t�@�C���T�C�Y
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
 * �t�@�C����ǂފ֐�
 *
 * void *p �t�@�C�����ւ̃|�C���^
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
 * �f�o�b�O�R���\�[���ɕ�������o�͂���֐�
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
