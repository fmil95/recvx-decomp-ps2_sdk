[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�}���`�X���b�h�T���v��

<�T���v���̉��>

	���̃v���O�����̓}���`�X���b�h��p���ăI�u�W�F�N�g�̕`����s��
	�T���v���ł��B�X���b�h�̐ؑւ��ɂ̓Z�}�t�H���g�p���A�Q�̐���
	�p�X���b�h�ƁA�P��DMA �L�b�N�X���b�h�A����ѕ����̃I�u�W�F�N�g
	�X���b�h����\������܂��B�P�̃I�u�W�F�N�g�X���b�h���A�P��
	�I�u�W�F�N�g��\�����܂��B

<�t�@�C��>

	thread.c

<�N�����@>

	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run thread.elf

<�R���g���[���̑�����@>

	��,�~�{�^��		�F�X���b�h�̐����ƍ폜
	���{�^��		�F��]�p�𗐐��ɂ��Đݒ�
	���{�^��		�F��]�p�̏�����
	R1,R2�{�^��		�F�I�u�W�F�N�g�Ԋu�ύX
	��,���{�^��		�F�I�u�W�F�N�g�ʒu�w���ړ�
	��,���{�^��		�F�I�u�W�F�N�g�ʒu�x���ړ�
	L1,L2�{�^��		�F�I�u�W�F�N�g�ʒu�y���ړ�
	start�{�^��		�F�|�[�Y
	select+���{�^��		�F������(����)
	select+���{�^��		�F������(����)
	select+R1�{�^��		�F�I�u�W�F�N�g�Ԋu��0�ɐݒ�
	select+R2�{�^��		�F�I�u�W�F�N�g�Ԋu������
	select+��,���{�^��	�F�I�u�W�F�N�g�ʒu�w��������
	select+��,���{�^��	�F�I�u�W�F�N�g�ʒu�x��������
	select+L1,L2�{�^��	�F�I�u�W�F�N�g�ʒu�y��������

[�Z�}�t�H]

    packet_sema�F
	�p�P�b�g�փf�[�^��ǉ��\���ǂ����������Ă��܂��B
	�i�����ۂɂ͂Ȃ��Ă����삵�܂��j
    send_signal�F
	�p�P�b�g�ւ̃f�[�^�ǉ����I���DMA���M�\�ł��鎖�������܂��B
    rotate_signal�F
	�������I���A�ʃI�u�W�F�N�g�֐�����ڂ��ėǂ����������܂��B

[�e�X���b�h����]

�I�u�W�F�N�g�X���b�h�F
packet_sema���擾���鎖���o������p�P�b�g�փf�[�^��ǉ����Apacket_sema��
�ԋp�����̂�rotate_signal�𑗂�܂��B

�R���g���[���X���b�h�F
���䂪����Ă�����kick_signal�𑗐M�����̂���rotate_signal�𑗂�܂��B
���䂪����Ă������́A��ʂ�̃I�u�W�F�N�g�̏������I���������\���܂��B

DMA �L�b�N�X���b�h:
�p�P�b�g�̏���������packet_sema��o�^���Ă���kick_signal��҂��܂��B
kick_signal��������p�P�b�g�̌㏈�������āADMA���M���s���܂��B

�I�u�W�F�N�g���[�e�[�g�X���b�h:
rotate_signal��҂��A�V�O�i����������priority2�̃X���b�h�̐���Ώۂ���]
�����A�ʂ̃I�u�W�F�N�g�֐�����ڂ��܂��B

[�X���b�h�\���}]	

     priority
	0	�I�u�W�F�N�g���[�e�[�g�X���b�h
	1	DMA �L�b�N�X���b�h
	2	�I�u�W�F�N�g�X���b�h
		�I�u�W�F�N�g�X���b�h
		�F
		�R���g���[���X���b�h

