[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�Z�}�t�H�̃T���v��

<�T���v���̉��>

���̃v���O�����́AVSync���荞�݂ƃZ�}�t�H���g���ăX���b�h�̓���𐧌䂷��
�T���v���ł��B

<�e�X���b�h����>

main �v���C�I���e�B0:
    semamain,sema1,sema2,sema3���쐬����B
    PrintThread1,PrintThread2,PrintThread3���쐬���ċN�����AREADY��Ԃɂ�
    ��B����VSyncStart���荞�݂�L���ɂ��āAWaitSema��WATI��Ԃֈڍs���܂��B

PrintThread1 �v���C�I���e�B1:
    �Z�}�t�Hsema1���l���������ƁAdsedb �R���\�[���փX���b�hID���o�͂���
    ������J��Ԃ��܂��B

PrintThread2 �v���C�I���e�B1:
    �Z�}�t�Hsema2���l���������ƁAdsedb �R���\�[���փX���b�hID���o�͂���
    ������J��Ԃ��܂��B

PrintThread3 �v���C�I���e�B1:
    �Z�}�t�Hsema3���l���������ƁAdsedb �R���\�[���փX���b�hID���o�͂���
    ������J��Ԃ��܂��B

<���荞��>

���̊��荞�݂̂��тɃZ�}�t�Hsema1,sema2,sema3��ԋp���܂��B

<�e�Z�}�t�H>

�Ή��֌W
main         -- semamain    main���I�������邽�߂̃Z�}�t�H
PrintThread1 -- sema1       PrintThread1�𓮍삳����Z�}�t�H
PrintThread2 -- sema2       PrintThread2�𓮍삳����Z�}�t�H
PrintThread3 -- sema3       PrintThread3�𓮍삳����Z�}�t�H

<�v���O�����̓���>

PrintThread1,2,3�𐳏�ɏI����������Amain�𐳏�ɏI�������邽�߂�
main�X���b�h�̃v���C�I���e�B��PrintThread1,2,3��艺����B
main�X���b�h��VSyncStart�̊��荞�݂�L���ɂ������WAIT��ԂɂȂ�܂��B

main�X���b�h��WAIT��ԂɂȂ�����ɁAVSyncStart�̊��荞�݂��������܂��B
���̊��荞�݂̂��тɃZ�}�t�Hsema1,sema2,sema3��ԋp���܂��B

main�X���b�h����A�N�����ꂽPrintThread1,2,3��RUN��ԂɂȂ�AWaitSema
�֐���WAIT��ԂɂȂ�܂��BVSyncStart���荞�݂̂��тɃZ�}�t�H
sema1,sema2,sema3���ԋp����܂��B���荞�݃n���h�����甲�������A
PrintThread1,2,3�́A�D��x�������Ȃ̂�READY��ԂɂȂ�������
�X���b�h���f�B�L���[�ɓo�^����A���ԂɎ��s����܂��B

4��J��Ԃ�����A�Ō��RUN��ԂɂȂ�PrintThread3��ExitDeleteThread�֐���
�Ăяo���O�ɁAmain�X���b�h��semamain�Z�}�t�H��������܂��B�������邱�ƂŁA
PrintThread3���I��������ɁA�D��x���Ⴂmain�X���b�h�֖߂�v���O������
����ɏI�����܂��B

<�t�@�C��>

	semaphore.c

<�N�����@>

	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run semaphore.elf

<�R���g���[���̑�����@>

	�Ȃ�
