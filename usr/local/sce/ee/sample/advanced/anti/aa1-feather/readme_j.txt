[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved


�A���`�G�C���A�V���O���ʂ̃T���v���v���O����
�i�y�\�[�g�����j

<�T���v���̉��>
	���̃v���O�����ł́AAA1 �ɂ��A���`�G�C���A�V���O���ʂƂ���
	�g������������܂��B

	AA1 �̌��ʂ��ő���ɔ�������ɂ́A�`�悷��|���S������ʉ�����
	�߂��̕����փ\�[�g����K�v������܂��B�������Ȃ���\�[�e�B���O
	�͎��Ԃ�������A�ꍇ�ɂ���Č����I�ȉ����ł͂���܂���B

	���̃T���v���́A�\�[�e�B���O���o���Ȃ��ꍇ�ɁA���ɔj�]���ڗ���
	�g�Ԃꂽ�h�|���S���Ɋւ��� AA1 �� OFF ���邱�ƂŎ��p��\����
	���ʂ������邱�Ƃ������܂��B

	�܂��AAA1 LINE �� Z Buffer �ɒl���������܂Ȃ��������������
	�� �P�x�ڂ� AA1 �� off ���� Z Buffer �ɕ`�悵�A�Q�x�ڂ� Z Test
	���s���Ȃ��� AA1 LINE ��`�悷�邱�Ƃ� Z Buffer �𗘗p�\�ł��B

	�܂��A������� AA �̕��@�Ƃ��āA1 field �O�� frame buffer ��
	���e�� bilinear filter �������Ȃ��猻�݂� frame buffer �� �� 
	Blend �`�悷��@�\����������Ă��܂��B

<�t�@�C��>
	aa1.c		�F���C���v���O����
	devinit.c	�F�f�o�C�X�̏��������[�`��
	make_packet.c	�F�p�P�b�g�������[�`��
	*.h		�F�e�N�X�`���f�[�^��
	work.dsm	�F�I�u�W�F�N�g�f�[�^(DMA packet)
	work.vsm	�FVU1�}�C�N���v���O����

<�N�����@>
	% make		�F�R���p�C��

	�R���p�C����A�ȉ��̕��@�Ŏ��s�\
	% dsedb
	> run aa1.elf

<�R���g���[���̑�����@>
	����(�����L�[�j	�F���_�؂�ւ�
	���{�^��	�F�I�u�W�F�N�g������
	�~�{�^��	�F�I�u�W�F�N�g������
	���{�^��	�F���C�g�����i�Â��Ȃ�j
	���{�^��	�F���C�g�����i���邭�Ȃ�j

	R1�{�^��	�FAA1 ON <-> OFF
	R2�{�^��	�F�I�u�W�F�N�g�؂�ւ� triangle <-> line

	L1�{�^��	�F1 field �O�̉摜�Ƃ̃u�����h�ɂ�� AA
	L2�{�^��	�F1 field �O�̉摜�Ƃ̃u�����h�ɂ�� AA�i�������邢�j

<���l>
	L1�{�^���AL2�{�^�� �̌��ʂ� AA1 �̋@�\�Ƃ͓Ɨ��ł��B

	�܂��AAA1 �̕���p����菜�����߂ɁA���_���猩�ĂԂꂽ
	�|���S������ї������̃|���S���ł� AA1 �� OFF ���Ă��܂��B
	���̂��߂̃R�[�h�� work.vsm �Ɋ܂܂�Ă��܂��B

	�v�Z�̊T�v�͈ȉ��̒ʂ�ł��B

	1. VF05 �� view ���W�n�ł̒��_���W���v�Z����
	2. VF06 �� view ���W�n�ł̃|���S���̖@�����v�Z����
	3. ���� VF05*VF06 ���v�Z�� VF29x �ɕۑ�
	4. ���_���璸�_�܂ł̋����� VF05z �ő�p�� VF08x �ɕۑ�
	5. VF29x*8.0f - VF08x ���v�Z
	6. �����ɂ��APRMODE �� AA1 �� ON-OFF �����R�}���h�� VF07 �ɏ���
	7. 1���_��`�悷��x�� PRMODE �� AA1 �� ON-OFF

	�����x�N�g���Ɩʂ̖@���Ƃ� cos �� 1/8 �ȉ��̎� AA1 �� OFF ���܂��B


