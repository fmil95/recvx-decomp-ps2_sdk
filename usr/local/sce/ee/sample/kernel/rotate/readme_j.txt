[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�X���b�h���f�B�L���[�̉�]�̃T���v��

<�T���v���̉��>

���̃v���O�����́A�X���b�h���f�B�L���[�̓���𗝉����邽�߂̃T���v���ł��B

<�e�X���b�h����>

main �v���C�I���e�B0:
    PrintThread1,PrintThread2,PrintThread3���쐬���ċN�����A���ꂼ���
    READY��Ԃɂ��āA�D��x��10�ɉ����܂��B

PrintThread1 �v���C�I���e�B1:
    dsedb �R���\�[���փX���b�hID���o�͂��A�X���b�h�̃��f�B�L���[��
    ��]�����邱�Ƃ��J��Ԃ��܂��B

PrintThread2 �v���C�I���e�B1:
    dsedb �R���\�[���փX���b�hID���o�͂��A�X���b�h�̃��f�B�L���[��
    ��]�����邱�Ƃ��J��Ԃ��܂��B

PrintThread3 �v���C�I���e�B1:
    dsedb �R���\�[���փX���b�hID���o�͂��A�X���b�h�̃��f�B�L���[��
    ��]�����邱�Ƃ��J��Ԃ��܂��B

<�v���O�����̓���>

main����A�N�����ꂽPrintThread1,2,3��READY��Ԃɂ���A�D��x�������Ȃ̂�
�N���������ɃX���b�h���f�B�L���[�ɓo�^����܂��B

main�̗D��x��10�ɉ�����ƁAPrintThread1��RUN��ԂɈڂ�A�X���b�hID���o��
���A�X���b�h���f�B�L���[����]�����܂��BPrintThread1�̓X���b�h���f�B�L���[
�̍Ō�Ɉڂ�APrintThread2��RUN��ԂɂȂ�܂��B���ꂪPrintThread3�܂Ŏ��s
����4��J��Ԃ����̂��I�����܂��B�Ō��PrintThread3���I��������Amain��
���䂪�߂�A�v���O�����͐���ɏI�����܂��B

<�t�@�C��>

	rotate.c

<�N�����@>

	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run rotate.elf

<�R���g���[���̑�����@>

	�Ȃ�
