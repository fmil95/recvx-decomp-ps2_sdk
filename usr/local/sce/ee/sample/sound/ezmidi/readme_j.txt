[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

EzMIDI - MIDI�ɂ��y�Ȃƌ��ʉ��̉��t


<�T���v���̉��>
	csl_hsyn�Acsl_midi�Acsl_msin��p���āA�y�Ȃƌ��ʉ������t���܂��B

	���̃T���v���́AIOP���̃T���v���ƑΉ����Ă���A����������ċ@�\����
	���BEE���̃v���O�����������I��IOP�������[�h���܂��̂ŁA�O������IOP
	���̃��W���[���iiop/sample/sound/ezmidi)���r���h���Ă����Ă��������B

	���ʉ������t����ɂ́Alibsd���g�p������@������܂����A���̃T���v��
	�ł�csl_hsyn���g���Ă��܂��BEE����csl_msin��p����midi-stream�𐶐�
	���A�����IOP���ɓ]�����邱�ƂŁAcsl_hsyn�����t���s�Ȃ��܂��B

	���̕��@�̗��_�́AJAM�̃t�@�C�������p�ł��邱�ƂƁASPU2�̃{�C�X�Ǘ�
	��EE���ōs�Ȃ��K�v���������Ƃł��BBGM�p�ƌ��ʉ��p�̂Q��midi-stream
	���̓|�[�g�ɑ΂��āA�ő剹���ƗD��x��ݒ肷�邱�ƂŁA
	�h���ʉ��p�ɍő傎���܂ŕۏ؂��A�c��́i�S�W�|���j�`�S�W���Ŋy�Ȃ���
	�t����h
	�Ƃ��������䂪�\�ł��B

	EE�EIOP�Ԃ̒ʐM���ŏ����ɂ��邽�߂ɁA�V���Ȍ��ʉ������N�G�X�g���ꂽ
	���̂݁A1�t���[����1�񂾂��AIOP�Ɍ��ʉ��p��midi-stream�o�b�t�@��]��
	���܂��B���̌��ʂɂ��Ă�EE���͊֒m���܂���B�{�C�X�Ǘ���csl_hsyn��
	�C�����܂����AIOP���̃��[�v�̕������{��������Ă��邽�߁i240Hz�j�A
	�o�b�t�@����肱�ڂ��S�z����������ł��B

	EzBGM��BD�̕������[�h�ɑΉ����Ă��܂��BIOP�̃��������������邱�ƂȂ�
	�ASPU2�̃��[�J���������ɑ傫��BD��]�����邱�Ƃ��\�ł��B

	BD��SPU2�������ւ̓]���ɂ́ADMA�̃`�����l��1���g���Ă��܂��B���o�̓u
	���b�N�]���isceSdBlockTrans�j���𓯎��ɍs�Ȃ��ꍇ�ɂ́A�g�p�`�����l
	�����Ԃ���Ȃ��悤�ɒ��ӂ��Ă��������B


<�t�@�C��>
	main.c	
	midi_rpc.c
	ezmidi_i.h
	sakana.sq
	sakana.hd
	sakana.bd
	eff.hd
	eff.bd

<�N�����@>
	% make		�FIOP���̃R���p�C���iiop/sample/sound/ezmidi/)
	% cd  ???	�FEE���̃f�B���N�g���Ɉړ��iee/sample/sound/ezmidi/)
	% make		�FEE���̃R���p�C��
	% make run	�F���s

	�Ȃ����t����A�����L�[�������Ό��ʉ�����������܂��B

<�R���g���[���̑�����@>
        �~�{�^��     :   �y�Ȃ̉��t�ĊJ
        ���{�^��     :   �y�Ȃ̉��t��~
	���{�^��     :   ���ʉ��i�e���t�H���j
	���{�^��     :   ���ʉ��i�R���f�B�E�E�j
	���{�^��     :   ���ʉ��i�R���f�B�E���j
	���{�^��     :   ���ʉ��i�O���X�j
        START�{�^��  :   �y�ȉ��t�̈ꎞ��~�E�ĊJ
	SELECT�{�^�� :   �v���O�����I��

<���l>
	MIDI���b�Z�[�W�͉��y�p�̃t�H�[�}�b�g�ł��̂ŁA���ʉ�����p�Ƃ��Č���
	�ꍇ�A�ȉ��̌��_������܂��B

	�E�����{�C�X�̓`�����l���ԍ��ƃL�[�ԍ��݂̂ŊǗ������̂ŁA�����L�[
	�@��A�ł��ČʂɃL�[�I�t����悤�Ȃ��Ƃ͏o���Ȃ��B
	�E�p���|�b�g���̃R���g���[���̓`�����l���S�̂ɂ������Ă��܂��B

	�������������邽�߂ɂ������Ǝ��̊g�����s�Ȃ����h�g��MIDI���b�Z�[
	�W�h���`���Ă��܂��B�t�H�[�}�b�g�ɂ��Ă� CSL�T�v���A
        API�isceMSIn_PutHsMsg���j�ɂ��Ă� CSL MIDI Stream �������t�@�����X��
        �Q�Ƃ��Ă��������B

