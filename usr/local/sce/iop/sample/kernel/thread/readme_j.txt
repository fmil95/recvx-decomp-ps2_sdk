[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�}���`�X���b�h�T���v��

<�T���v���̉��>

�X���b�h�̐����ƋN���A�X���b�h�̃v���C�I���e�B�̑���A�X���b�h�Ԃ̓�������
��{�I�ȃT���v���ł��B

<�t�@�C��>

	createth.c		�X���b�h�̐����ƋN�����@�̊�{�I�ȃT���v��
	sleepth.c		SleepThread()/WakeupThread() �̃T���v��
	eventth.c		SetEventFlag()/WaitEventFlag() �̃T���v��
	startthargs.c		�X���b�h�̋N���T���v��

<�N�����@>

	% make					�F�R���p�C��

    createth �̎��s

	�܂��A�ʂ̃E�C���h�E��
	% dsicons 
	�����s���Ă����܂��B

	% dsreset 0 2
	% dsistart createth.irx

	dsicons �����s�����E�C���h�E�� 6 �̃X���b�h�̋N���̗l�q��
	�\������܂��B

	�ȉ��̕��@�ł����s�\
	% dsidb
	> reset 0 2 ; mstart createth.irx


    sleepth �̎��s

	�܂��A��E�C���h�E��p�ӂ����ꂼ���
	% dsicons 0
	��
	% dsicons 1-9
	�����s���Ă����܂��B

	�����āA����ɕʂ̂R�߂̃E�C���h�E�ňȉ������s���܂�
	% dsreset 0 2
	% dsistart sleepth.irx

	dsicons 0 �����s�����E�C���h�E�� '0..5 or a..f or A..F > ' �Ƃ���
	�v�����v�g���\������܂��B
	���̎��A�v�����v�g��\�����ē��͑҂��ɂȂ��Ă���R�}���h����
	�X���b�h�ƁA���̑��� 6 �̃X���b�h���N�����Ă��܂��B
	�v�����v�g��\�����ē��͑҂��ɂȂ��Ă���X���b�h�͑��� 6 ��
	�X���b�h�����v���C�I���e�B�������Ȃ��Ă��܂����A�L�[���͑҂���
	WAIT��Ԃɓ����Ă���̂ŁA���� �U�̃X���b�h�͂��ꂼ����s����
	�l�����X�^�[�g���b�Z�[�W��\�����܂��B
	

	0 ���� 5 �̐�������͂��āARETURN �L�[����͂���ƑΉ�����
	�X���b�h���N��(WakeupThread)�����܂��B

	a ���� f �̕�������͂��āARETURN �L�[����͂���ƑΉ�����
	0 ���� 5 �Ԃ̃X���b�h�� 2 ��N��(WakeupThread)�����܂��B

	A ���� F �̕�������͂��āARETURN �L�[����͂���ƑΉ�����
	0 ���� 5 �Ԃ̃X���b�h�� 4 ��N��(WakeupThread)�����܂��B

	�ȉ��̕��@�ł����s�\
	% dsidb
	> reset 0 2 ; mstart sleepth.irx


	�E�C���h�E������ɂ�������p�ӂ��A���ꂼ���
	% dsicons 1
	% dsicons 2
	% dsicons 3
	% dsicons 4
	% dsicons 5
	���N�����Ă����ƁA�e�X���b�h�� open ���� TTY ���ɕʂ̃E�C���h�E��
	�\�����邱�Ƃ��ł��܂��B

    eventth �̎��s

	�܂��A��E�C���h�E��p�ӂ����ꂼ���
	% dsicons 0
	��
	% dsicons 1-9
	�����s���Ă����܂��B

	�����āA����ɕʂ̂R�߂̃E�C���h�E�ňȉ������s���܂�
	% dsreset 0 2
	% dsistart eventth.irx

	dsicons 0 �����s�����E�C���h�E�� '0..5,a > ' �Ƃ���
	�v�����v�g���\������܂��B
	���̎��A�v�����v�g��\�����ē��͑҂��ɂȂ��Ă���R�}���h����
	�X���b�h�ƁA���̑��� 6 �̃X���b�h���N�����Ă��܂��B
	�v�����v�g��\�����ē��͑҂��ɂȂ��Ă���X���b�h�͑��� 6 ��
	�X���b�h�����v���C�I���e�B�������Ȃ��Ă��܂����A�L�[���͑҂���
	WAIT��Ԃɓ����Ă���̂ŁA���� �U�̃X���b�h�͂��ꂼ����s����
	�l�����X�^�[�g���b�Z�[�W��\�����܂��B

	0 ���� 5 �̐�������͂��āARETURN �L�[����͂���ƃC�x���g�t���O��
	�Ή�����r�b�g�𗧂Ă܂��B

	a ����͂��āARETURN �L�[����͂���ƃC�x���g�t���O�̑S���̃r�b�g
	�𗧂Ă܂��B

	�ȉ��̕��@�ł����s�\
	% dsidb
	> reset 0 2 ; mstart eventth.irx

	�E�C���h�E������ɂ�������p�ӂ��A���ꂼ���
	% dsicons 1
	% dsicons 2
	% dsicons 3
	% dsicons 4
	% dsicons 5
	���N�����Ă����ƁA�e�X���b�h�� open ���� TTY ���ɕʂ̃E�C���h�E��
	�\�����邱�Ƃ��ł��܂��B

    startthargs �̎��s

	�܂��A�ʂ̃E�C���h�E��
	% dsicons 
	�����s���Ă����܂��B

	% dsreset 0 2
	% dsistart startthargs.irx

	dsicons �����s�����E�C���h�E�� �X���b�h�̋N�������̗l�q��
	�\������܂��B

	�ȉ��̕��@�ł����s�\
	% dsidb
	> reset 0 2 ; mstart startthargs.irx
