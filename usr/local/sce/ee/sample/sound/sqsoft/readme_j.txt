[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

�\�t�g�E�F�A�E�V���Z�T�C�U�ɂ��MIDI�Đ�

<�T���v���̉��>
	MIDI�V�[�P���T�imodmidi�j�ƃ\�t�g�E�F�A�E�V���Z�T�C�U�imodssyn�j��EE���C���A�E�g
	(liblout)�̃T���v���ł��B

	���̃T���v���́AIOP���̃T���v���ƑΉ����Ă���A����������ċ@�\
        ���܂��BEE���̃v���O�����������I��IOP�������[�h���܂��̂ŁA�O��
	����IOP���̃��W���[���iiop/sample/sound/sqsoft)���r���h���Ă�
	���Ă��������B

	�\�t�g�E�F�A�E�V���Z�T�C�U�̉��f�f�[�^�́Adata/sound/sce.ssb ���g�p����
        ���܂��B����͂܂��]���łŁA����Ƀ��t�@�C������\��ł��B

	GM�܂���GS�����̋ȃf�[�^�ł���΁A�قƂ�ǂ��̂܂܉��t���邱�Ƃ�
        �ł��܂����A�h�h�����p�[�g�h�̊T�O���������߂ɁA�h�����p�[�g��
        �`�����l����BankChange(MSB) = 120 ��ݒ肵�Ă������Ƃ��K�v�ł��B
        �ڂ����͉��f�f�[�^��readme�������������B


	�v���O�����̓���͈ȉ��̂悤�ɂȂ��Ă��܂��B

       �EEE���v���O�������N�����AIOP���ɕK�v�ȃ��W���[�������[�h����B

       �EEE���̏��������s�Ȃ��B

       �EEE������IOP���̃��[�U�[�v���O���������[�h���ċN������B�Ȍ��EE��
	 ��IOP����̃f�[�^�҂���ԂɂȂ�B

       �EIOP���ł�sq�����[�h���A�V�[�P���X���X�^�[�g������Bmodmidi�̏o��
	 ��Stream MIDI ��modssyn�ɓ��͂����ƁA�^�C���R�[�h��t������EE��
	 �ɓ]�������B�i�f�[�^�͓Ǝ��`���j

       �EMIDI�f�[�^�������Ă���ƁAEE���͂�������Ƃɉ��Z���AStream-PCM
	 �f�[�^�Ƃ��ďo�͂���B

       �E����PCM Stream�f�[�^��liblout�̓��͂ƂȂ�ASPU2�̓��o�̓u���b�N��
	 �]�������B������libsdr�̋@�\���g�p���Ă���B


<�t�@�C��>
	main.c	

<�N�����@>

	% make		�FIOP���̃R���p�C���iiop/sample/sound/sqsoft/)
	% cd  ???	�FEE���̃f�B���N�g���Ɉړ��iee/sample/sound/sqsoft/)
	% make		�FEE���̃R���p�C��
	% make run	�F���s

	�Ȃ����t�����ΐ���B

<�R���g���[���̑�����@>
	�Ȃ�


