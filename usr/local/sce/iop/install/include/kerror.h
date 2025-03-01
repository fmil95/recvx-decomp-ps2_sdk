/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* $Id: kerror.h,v 1.10 2000/02/29 14:15:11 tei Exp $ */

/*
 *                     I/O Processor System Services
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         kerror.h
 *                         error code define
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.00           1999/10/12      tei
 *       1.01           2000/02/28      tei	add -160..-162
 */

#ifndef _KERROR_H
#define _KERROR_H

enum KernelErrorCode {
  KE_OK                  =    0, /* no error                               */
  KE_ERROR               =   -1, /* error                                  */
  KE_ILLEGAL_EXPCODE     =  -50, /* illegal exception code                 */
  KE_EXPHANDLER_NOUSE    =  -51, /* exception handler not use              */
  KE_EXPHANDLER_USED     =  -52, /* exception handler already used         */
  KE_ILLEGAL_CONTEXT     = -100, /* call from interrupt_handler/thread     */
  KE_ILLEGAL_INTRCODE    = -101, /* illegal INTRCODE                       */
  KE_CPUDI               = -102, /* CPU already interrupt disable          */
  KE_INTRDISABLE         = -103, /* INTRCODE already interrupt disable     */
  KE_FOUND_HANDLER       = -104, /* Hnadler already exist                  */
  KE_NOTFOUND_HANDLER    = -105, /* Handler not found                      */
  KE_NO_TIMER            = -150, /* �n�[�h�E�G�A�^�C�}�͎擾�ł��Ȃ�����   */
  KE_ILLEGAL_TIMERID     = -151, /* �n�[�h�E�G�A�^�C�}��ID���s��           */
  KE_UNIT_USED           = -160, /* unit number already used               */
  KE_UNIT_NOUSE          = -161, /* unit number not used                   */
  KE_NO_ROMDIR           = -162, /* rom directory not found                */
  KE_LINKERR             = -200, /* module link error                      */
  KE_ILLEGAL_OBJECT      = -201, /* illegal object format (not IRX)        */
  KE_UNKNOWN_MODULE      = -202, /* not found Module                       */
  KE_NOFILE              = -203, /* not found Module file                  */
  KE_FILEERR             = -204, /* Module file read error                 */
  KE_MEMINUSE            = -205, /* memory in use                          */
  KE_NO_MEMORY           = -400, /* no memory                              */
  KE_ILLEGAL_ATTR        = -401, /* illegal attr parameter                 */
  KE_ILLEGAL_ENTRY       = -402, /* illegal tread entry address            */
  KE_ILLEGAL_PRIORITY    = -403, /* illegal priority value                 */
  KE_ILLEGAL_STACK_SIZE  = -404, /* illegal stack size                     */
  KE_ILLEGAL_MODE        = -405, /* illegal mode                           */
  KE_ILLEGAL_THID        = -406, /* illegal thread ID                      */
  KE_UNKNOWN_THID        = -407, /* not found thread                       */
  KE_UNKNOWN_SEMID       = -408, /* not found semaphore                    */
  KE_UNKNOWN_EVFID       = -409, /* not found eventflag                    */
  KE_UNKNOWN_MBXID       = -410, /* not found messagebox                   */
  KE_UNKNOWN_VPLID       = -411, /* not found Vpool                        */
  KE_UNKNOWN_FPLID       = -412, /* not found Fpool                        */
  KE_DORMANT             = -413, /* thread already DORMANT                 */
  KE_NOT_DORMANT         = -414, /* thread is not DORMANT                  */
  KE_NOT_SUSPEND         = -415, /* thread is not SUSPEND                  */
  KE_NOT_WAIT            = -416, /* thread is not WAIT                     */
  KE_CAN_NOT_WAIT        = -417, /* now dispatch disabled                  */
  KE_RELEASE_WAIT        = -418, /* WAIT status release                    */
  KE_SEMA_ZERO           = -419, /* semaphore counter zero                 */
  KE_SEMA_OVF            = -420, /* semaphore counter overflow             */
  KE_EVF_COND            = -421, /* �C�x���g�t���O�̑҂��������s���������� */
  KE_EVF_MULTI           = -422, /* �C�x���g�t���O�͑��d�҂��������Ȃ�     */
  KE_EVF_ILPAT           = -423, /* �C�x���g�t���O�̑҂��p�^�[�����s��     */
  KE_MBOX_NOMSG          = -424, /* message box have no message            */
  KE_WAIT_DELETE         = -425, /* wait object deleted                    */
  KE_ILLEGAL_MEMBLOCK    = -426, /* illegal memory block                   */
  KE_ILLEGAL_MEMSIZE     = -427, /* illegal memory size                    */
};

#endif /* _KERROR_H */
