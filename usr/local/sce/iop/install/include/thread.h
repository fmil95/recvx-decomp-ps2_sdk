/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* $Id: thread.h,v 1.11 1999/11/25 17:59:07 tei Exp $ */

/*
 *                     I/O Processor System Services
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         thread.h
 *                         thread manager defines
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.00           1999/10/12      tei
 *       1.01           1999/11/20      tei
 */

#ifndef _THREAD_H_
#define _THREAD_H_

#include <sys/types.h>

#define TH_SELF		0
#define TPRI_RUN	0
 
/*
 * Thread priority convention
 *
 *  HIGHEST .. HIGHSYS_RESERVED		reserved for debug system
 *  MODULE_INIT-3 .. MODULE_INIT	for module manager
 *  MODULE_INIT				for each module initialize routine
 *
 *  USER_HIGHEST..USER_LOWEST		for user & drivers
 *
 *  LOWSYS_RESERVED .. LOWEST		reserved for debug system
 */

#define	LOWEST_PRIORITY			(32*4-2)
#define	HIGHEST_PRIORITY		1

#define	HIGHSYS_RESERVED_PRIORITY	4
#define	MODULE_INIT_PRIORITY		8
#define	USER_HIGHEST_PRIORITY		9

#define	LOWSYS_RESERVED_PRIORITY	(LOWEST_PRIORITY-2)
#define	USER_LOWEST_PRIORITY		(LOWSYS_RESERVED_PRIORITY-1)


/* For Thread Management */
struct ThreadParam {
    u_int	attr;		/* ���� 			*/
    u_int	option;		/* ���[�U�p�t�����  		*/
    void	*entry;		/* �G���g���A�h���X 		*/
    int		stackSize;	/* �X�^�b�N�T�C�Y 		*/
    int		initPriority;	/* �D�揇�� �����l 		*/
};

struct ThreadInfo {
    u_int	attr;		/* ���� 			*/
    u_int	option;		/* ���[�U�p�t�����  		*/
    int		status;		/* �X���b�h�̏�� 		*/
    void	*entry;		/* �G���g���A�h���X 		*/
    void	*stack;		/* �X�^�b�N�̈�̐擪 		*/
    int		stackSize;	/* �X�^�b�N�T�C�Y 		*/
    void	*gpReg;		/* gp ���W�X�^�l 		*/
    int		initPriority;	/* �D�揇�� �����l 		*/
    int		currentPriority;/* �D�揇�� ���ݒl 		*/
    int		waitType;	/* �҂���Ԃ̎��		*/
    int		waitId;		/* waittype �̑҂��Ώۂ� ID	*/
    int		wakeupCount;	/* �������� wakeup ��		*/
    u_long	*regContext;
    int		reserved1;
    int		reserved2;
    int		reserved3;
    int		reserved4;
};

/* ThreadInfo.status */
#define	THS_RUN		0x01
#define THS_READY	0x02
#define THS_WAIT	0x04
#define THS_SUSPEND	0x08
#define THS_WAITSUSPEND	0x0c
#define THS_DORMANT	0x10
#define THS_DEAD	0x20

/* ThreadInfo.waitType */
#define TSW_SLEEP	1	/* SleepThread()�ɂ��҂����		*/
#define TSW_DELAY	2	/* DelayThread()�ɂ��҂����		*/
#define TSW_SEMA	3	/* �Z�}�t�H�̑҂����			*/
#define TSW_EVENTFLAG	4	/* �C�x���g�t���O�̑҂����		*/
#define TSW_MBX		5	/* ���b�Z�[�W�{�b�N�X�̑҂����		*/
#define TSW_VPL		6	/* �ϒ��������u���b�N�l���̑҂����	*/
#define TSW_FPL		7	/* �Œ蒷�������u���b�N�l���̑҂����	*/

/* ThreadParam.attr, ThreadInfo.attr */
#define TH_ASM		0x01000000
#define TH_C		0x02000000
#define TH_COP1		0x20000000	/* Coprocessor 1 usable */
#define TH_COP2		0x40000000	/* Coprocessor 2 usable */
#define TH_COP3		0x80000000	/* Coprocessor 3 usable */
#define TH_UMODE	0x00000008
#define TH_NO_FILLSTACK	0x00100000

extern int CreateThread( struct ThreadParam *param );
extern int DeleteThread( int thid );
extern int StartThread( int thid, u_long arg );
extern int StartThreadArgs( int thid, int args, void *argp );
extern int ExitThread();
//	extern int ExitDeleteThread();
extern int TerminateThread( int thid );
extern int iTerminateThread( int thid );
//	extern int DisableDispatchThread();
//	extern int EnableDispatchThread();
extern int ChangeThreadPriority( int thid, int priority );
extern int iChangeThreadPriority( int thid, int priority );
extern int RotateThreadReadyQueue( int priority );
extern int iRotateThreadReadyQueue( int priority );
extern int ReleaseWaitThread( int thid );
extern int iReleaseWaitThread( int thid );
extern int GetThreadId();
extern int ReferThreadStatus( int thid, struct ThreadInfo *info );
extern int iReferThreadStatus( int thid, struct ThreadInfo *info );
extern int CheckThreadStack();

/* ------ �����E�ʐM�@�\ ------ */

/* XXXX->attr common */
#define AT_THFIFO	0
#define AT_THPRI	1
#define AT_SINGLE	0
#define AT_MULTI	2
#define AT_MSFIFO	0
#define AT_MSPRI	4

/* --- Thread direct --- */
extern int SleepThread();
extern int WakeupThread( int thid );
extern int iWakeupThread( int thid );
extern int CancelWakeupThread( int thid );
extern int iCancelWakeupThread( int thid );
//	extern int SuspendThread( int thid );
//	extern int iSuspendThread( int thid );
//	extern int ResumeThread( int thid );
//	extern int iResumeThread( int thid );

/* --- Semaphore --- */
struct SemaParam {
    u_int	attr;		/* �Z�}�t�H�̑���		*/
    u_int	option;		/* ���[�U�p�t�����  		*/
    int		initCount;	/* �Z�}�t�H�̏����l		*/
    int		maxCount;	/* �Z�}�t�H�̍ő�l		*/
};

struct SemaInfo {
    u_int	attr;		/* �Z�}�t�H�̑���		*/
    u_int	option;		/* ���[�U�p�t�����  		*/
    int		initCount;	/* �Z�}�t�H�̏����l		*/
    int		maxCount;	/* �Z�}�t�H�̍ő�l		*/
    int		currentCount;	/* �Z�}�t�H�̌��ݒl		*/
    int		numWaitThreads;	/* �Z�}�t�H�҂��X���b�h��	*/
    int		reserved1;
    int		reserved2;
};

/* SemaParam->attr, SemaInfo->attr */
#define SA_THFIFO	AT_THFIFO
#define SA_THPRI	AT_THPRI
#define SA_IHTHPRI	0x100

extern int CreateSema( struct SemaParam *param );
extern int DeleteSema( int semid );
extern int SignalSema( int semid );
extern int iSignalSema( int semid );
extern int WaitSema( int semid );
extern int PollSema( int semid );
extern int ReferSemaStatus( int semid, struct SemaInfo *info );
extern int iReferSemaStatus( int semid, struct SemaInfo *info );

/* --- EventFlag --- */
struct EventFlagParam {
    u_int	attr;		/* �C�x���g�t���O�̑���		*/
    u_int	option;		/* ���[�U�p�t�����  		*/
    u_int	initPattern;	/* �C�x���g�t���O�̏����l	*/
};

struct EventFlagInfo {
    u_int	attr;		/* �C�x���g�t���O�̑���		*/
    u_int	option;		/* ���[�U�p�t�����  		*/
    u_int	initPattern;	/* �C�x���g�t���O�̏����l	*/
    u_int	currentPattern;	/* �C�x���g�t���O�̌��ݒl	*/
    int		numWaitThreads;	/* �C�x���g�t���O�҂��X���b�h�� */
    int		reserved1;
    int		reserved2;
};

/* EventFlagParam->attr, EventFlagInfo->attr */
#define EA_SINGLE	AT_SINGLE
#define EA_MULTI	AT_MULTI

/* waitmode */
#define EW_AND		0
#define EW_OR		1
#define EW_CLEAR	0x10

extern int CreateEventFlag( struct EventFlagParam *param );
extern int DeleteEventFlag( int evfid );
extern int SetEventFlag( int evfid, u_long bitpattern );
extern int iSetEventFlag( int evfid, u_long bitpattern );
extern int ClearEventFlag( int evfid, u_long bitpattern );
extern int iClearEventFlag( int evfid, u_long bitpattern );
extern int WaitEventFlag( int evfid, u_long bitpattern, int waitmode,
			   u_long *resultpat );
extern int PollEventFlag( int evfid, u_long bitpattern, int waitmode,
		   u_long *resultpat );
extern int ReferEventFlagStatus( int evfid, struct EventFlagInfo *info );
extern int iReferEventFlagStatus( int evfid, struct EventFlagInfo *info );

/* --- MessageBox --- */
struct MbxParam {
    u_int	attr;		/* ���b�Z�[�W�{�b�N�X�̑���	*/
    u_int	option;		/* ���[�U�p�t�����  		*/
};

struct MbxInfo {
    u_int	attr;		/* ���b�Z�[�W�{�b�N�X�̑���	*/
    u_int	option;		/* ���[�U�p�t�����  		*/
    int		numWaitThreads;	/* ���b�Z�[�W�҂��X���b�h��	*/
    int		numMessage;	/* ����҂����b�Z�[�W��	*/
    struct MsgPacket	*topPacket; /* ����҂��擪���b�Z�[�W	*/
    int		reserved1;
    int		reserved2;
};

struct MsgPacket {
    struct MsgPacket *next;
    u_char	      msgPriority;
    u_char	      dummy[3];
};

/* MbxParam->attr, MbxInfo->attr */
#define MBA_THFIFO	AT_THFIFO
#define MBA_THPRI	AT_THPRI
#define MBA_MSFIFO	AT_MSFIFO
#define MBA_MSPRI	AT_MSPRI

extern int CreateMbx( struct MbxParam *param );
extern int DeleteMbx( int mbxid );
extern int SendMbx( int mbxid, struct MsgPacket *sendmsg );
extern int iSendMbx( int mbxid, struct MsgPacket *sendmsg );
extern int ReceiveMbx( struct MsgPacket **recvmsg, int mbxid );
extern int PollMbx( struct MsgPacket **recvmsg, int mbxid );
extern int ReferMbxStatus( int mbxid, struct MbxInfo *info );
extern int iReferMbxStatus( int mbxid, struct MbxInfo *info );

/* --- Variable zize memory pool --- */
struct VplParam {
    u_int	attr;		/* �ϒ��������v�[���̑���		*/
    u_int	option;		/* ���[�U�p�t�����			*/
    int		size;		/* �ϒ��������v�[���̃T�C�Y		*/
};

struct VplInfo {
    u_int	attr;		/* �ϒ��������v�[���̑���		*/
    u_int	option;		/* ���[�U�p�t�����  			*/
    int		size;		/* �ϒ��������v�[���̃T�C�Y		*/
    int		freeSize;	/* �ϒ��������v�[���̖��g�p�T�C�Y	*/
    int		numWaitThreads;	/* �������҂��X���b�h��			*/
    int		reserved1;
    int		reserved2;
    int		reserved3;
};

/* VplParam.attr, VplInfo.attr */
#define VA_THFIFO	AT_THFIFO
#define VA_THPRI	AT_THPRI
#define VA_MEMBTM	0x200

extern int  CreateVpl( struct VplParam *param );
extern int  DeleteVpl( int vplid );
extern void *AllocateVpl( int vplid, int size );
extern void *pAllocateVpl( int vplid, int size );
extern void *ipAllocateVpl( int vplid, int size );
extern int  FreeVpl( int vplid, void *block );
extern int  ReferVplStatus( int vplid, struct VplInfo *info );
extern int  iReferVplStatus( int vplid, struct VplInfo *info );

/* --- Fixed zize memory pool --- */
struct FplParam {
    u_int	attr;		/* �Œ蒷�������v�[���̑���		*/
    u_int	option;		/* ���[�U�p�t�����  			*/
    int		blockSize;	/* �Œ蒷�������v�[���̃u���b�N�T�C�Y	*/
    int		numBlocks;	/* �Œ蒷�������v�[���̃u���b�N��	*/
};

struct FplInfo {
    u_int	attr;		/* �Œ蒷�������v�[���̑���		*/
    u_int	option;		/* ���[�U�p�t�����  			*/
    int		blockSize;	/* �Œ蒷�������v�[���̃u���b�N�T�C�Y	*/
    int		numBlocks;	/* �Œ蒷�������v�[���̃u���b�N��	*/
    int		freeBlocks;	/* �Œ蒷�������v�[���̖��g�p�u���b�N	*/
    int		numWaitThreads;	/* �������҂��X���b�h��			*/
    int		reserved1;
    int		reserved2;
    int		reserved3;
    int		reserved4;
};

/* FplParam.attr, FplInfo.attr */
#define FA_THFIFO	AT_THFIFO
#define FA_THPRI	AT_THPRI
#define FA_MEMBTM	0x200

extern int  CreateFpl( struct FplParam *param );
extern int  DeleteFpl( int fplid );
extern void *AllocateFpl( int fplid );
extern void *pAllocateFpl( int fplid );
extern void *ipAllocateFpl( int fplid );
extern int FreeFpl( int fplid, void *block );
extern int ReferFplStatus( int fplid, struct FplInfo *info );
extern int iReferFplStatus( int fplid, struct FplInfo *info );

/* ����/�^�C�}�Ǘ��@�\ */
extern int DelayThread(unsigned int usec);

struct SysClock {
    u_int	low;
    u_int	hi;
};

typedef u_int (*AlarmHandler)(void*);

extern int GetSystemTime(struct SysClock *clock);

extern int SetAlarm( struct SysClock *clock, AlarmHandler handler,
		     void *common);
extern int iSetAlarm( struct SysClock *clock, AlarmHandler handler,
		      void *common);
extern int CancelAlarm( AlarmHandler handler, void *common );
extern int iCancelAlarm( AlarmHandler handler, void *common );

extern void USec2SysClock(unsigned int usec, struct SysClock *clock);
extern void SysClock2USec(struct SysClock *clock, int *sec, int *usec);

#endif /* _THREAD_H_ */
