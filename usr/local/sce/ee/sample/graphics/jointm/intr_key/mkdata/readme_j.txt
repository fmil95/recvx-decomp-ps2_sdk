[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
�@�@�@�@�@�@�@�@�@Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

��]�����}�ɂ��L�[�t���[���}�g���N�X�A�j���[�V�����̃f�[�^�쐬�T���v��

<�T���v���̉��>
	intr_key �ȉ��̃v���O�����́A��]�����}�Ƃ������@�ŁA�֐߂̃}
	�g���N�X�ɂ��L�[�t���[���A�j���[�V�������s�Ȃ��T���v���ł��B

	���̃f�B���N�g�� mkdata �̃v���O�����́A���Ƃ̃L�[�t���[���}�g
	���N�X�V�[�P���X����A��]�����}�p�̃p�����[�^�V�[�P���X���쐬
	���t�@�C���ɏo�͂��܂��B

<�t�@�C��>
	main.c          : ���C���֐��A���o�͊֐�
	axis.c          : ��]�����}�֘A�֐�
	matrix.c        : �s�񉉎Z�֐�
	matseq.c        : ���̓f�[�^ (�L�[�t���[���}�g���N�X�V�[�P���X)
	axis.h		: �w�b�_�t�@�C��
	matrix.h	: �w�b�_�t�@�C��
	axisseq.ref	: �p�����[�^�V�[�P���X�f�[�^

<�N�����@>
	% make		�F�R���p�C��
	% make run	�F���s
	�������́A�ȉ��̕��@�ł����s�ł��܂�
	% dsedb
	> run *.elf
	
	�o�͂��ꂽ axisseq.dat �t�@�C���̓��e��	axisseq.ref �Ɠ���ł����
	����I���ł��B

<�R���g���[���̑�����@>
	�Ȃ�

<���l>
	�o�͂̓e�L�X�g�`���� axisseq.dat �t�@�C���ɏ����o���܂��B
	���̃f�[�^�� runtime �̃T���v���v���O�����ŗ��p������̂Ɠ����`��
	�ł��B
	�o�C�i���`���ł̏o�͂�����ɂ́AwriteMatrix(), writeAxis() �Ȃǂ�
	�K�X�ύX���Ă��������B

