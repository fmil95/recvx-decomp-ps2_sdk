[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

i.LINK(IEEE1394)�ŒʐM���s�Ȃ��T���v��

<�T���v���̉��>

	���̃v���O�����́ASCE�Ǝ��̃v���g�R���ł���i.LINK�\�P�b�g���g��
	�����������ꂽ�f�[�^���Q���"PlayStation 2"�����݂ɑ���M���A��M
	���ʂ̐��ۂ����j�^�ɕ\�����܂��B

	���̃T���v����IOP���̃T���v���ƑΉ����Ă���A����������ċ@�\���܂��B
        EE���̃v���O�����������I��IOP���̃��W���[�������[�h���܂��̂ŁA
        �O������IOP���̃T���v���iiop/sample/ilink)���r���h���Ă��������B

	�E�ڑ��܂ł̎菇
	�ŏ��͑���ƂȂ�"PlayStation 2"��GUID(1394 Global Unique ID)���s��
	�Ȃ̂Ńu���[�h�L���X�g�ő��M���܂��B
	�����Ɏ�M�X���b�h�̓\�P�b�g���Ď����A��M���ꂽ�p�P�b�g�̑��M����
	GUID�𒲂ׁA���ꂪ�����ȊO�̃m�[�h����̂��̂ł���΁A����GUID��
	�ʐM����Ƃ��ēo�^���Aconnect���܂��B
	����ȍ~��connect�����m�[�h�Ɍ������Ă̂݁A�p�P�b�g�𑗐M���܂��B
	���A���̃T���v���ł͒ʐM����i.LINK���ؒf����邱�Ƃ͑z�肵�Ă��܂���B

	�E�\��
	��M�f�[�^�ɕt�������`�F�b�N�T���̐��ۂ�\�����܂��B
	��M�͔񓯊��Ȃ̂ŁA��M���ׂ��f�[�^���Ȃ��Ƃ��͋�s���\������A
	�]�����ꍇ��FIFO�ɓ�����A�Â����̂��珇�Ɏ��o����܂��B

	�E�T���v���̕⑫����
	eemain.c�̐擪�́u#define SEND_NO_WAIT�v��L���ɂ���Ɣ񓯊����M�A
	�����ɂ���Ɠ������M�ƂȂ�܂��B
	�����E�񓯊��̈Ⴂ��socketee.c��RPC���s���Ƃ���SIF_RPCM_NOWAIT��
	���邩�A���Ȃ����̈Ⴂ�����ł��B
	��M�͏�ɔ񓯊���M�ł��B

	�Ei.LINK�\�P�b�g�h���C�o�ɂ���
	i.LINK�\�P�b�g���g�����u���[�h�L���X�g��IOP�ւ̕��ׂ��傫��������
	1394�o�X�ɑ��݂���m�[�h���ɔ�Ⴕ�đ�������̂ŁA�ǂ����Ă��K�v��
	�ꍇ�ɂ��������p���������B

	i.LINK�\�P�b�g���g�����Ƃ��A�P�p�P�b�g�ő���M�ł���ő�y�C���[�h
	�T�C�Y�i���[�U�[�����R�Ɏg����T�C�Y�j��492�o�C�g�ł��B
	���݂ɃT���v���ł͂P�u�ɂV�p�P�b�g�𑗎�M���Ă���̂ŁA�f�[�^�]��
	���[�g��492�~7�~60=206640�o�C�g�^�b�ƂȂ�܂��B

	sceILsockRecv�Ȃǂ��g����i.LINK�\�P�b�g�h���C�o�����M�p�P�b�g��
	���o�����������A�p�P�b�g������������������ƃh���C�o�����ɂ���
	FIFO�����܂��B����FIFO�̑傫���̓f�t�H���g�łV�p�P�b�g�ł��B
	����𒴂����p�P�b�g��FIFO�Ɋi�[���ꂸ�A�p������܂��B

<�t�@�C��>
	eemain.c        ���C���v���O����
	socketee.c      RPC client

<�N�����@>
	�Q���"PlayStation 2"��S400�Ή���i.LINK�P�[�u���Őڑ����܂��B
	�i�\�j�[�@�u�l�b�|�h�k�S�S�P�T�A�S�S�R�T�Ȃǁj

	% make		�FIOP ���̃R���p�C���iiop/sample/ilink)

	EE���̃T���v���f�B���N�g���Ɉړ��iee/sample/ilink)
	% make		�FEE ���̃R���p�C��

	�R���p�C����A�e�X��"PlayStation 2" �� dsedb ���N�����܂��B

	% dsedb
	> run eemain.elf

	�����s���܂��B

<�R���g���[���̑�����@>
	�Ȃ�

����ʕ\���̐�����
	NodeID	�F���@�̃m�[�hID
	Src	�F���@��GUID (64bit)
	Dest	�F�G�@��GUID (64bit)
	Status	�F���@�̃����N�̏��
			Single:  ���ڑ�
			Connect: �ڑ�
	Elapsed	�F���s�J�n��̌o�ߎ��ԁi���F���F�b�F�t���[���j
	nRemHDPkt�Fhalf done packet(*)���A�v����FIFO����폜������
		�i*�F�P�u�ɕ����̃p�P�b�g����M���Ă���Ƃ��A��M�p�P�b�g
		�@�̘A�����Ɉُ�����o������A�����̃p�P�b�g��FIFO����
		�@�폜���Ă��܂��j
	nRetryCnt�F�������M���ɉ��炩�̃G���[�̂��߂ɍđ��M������
		�@�񓯊����M�̏ꍇ�ɂ͕\������܂���B
	TxFrm	�F���M�t���[���ԍ��i�b�F1/60�b�j
	nRx	�F��MFIFO�Ɏc���Ă���p�P�b�g��
	RxFrm	�F��M�t���[���ԍ��i�b�F1/60�b�j
	SumN...	�F��m�p�P�b�g�̃`�F�b�N�T���̌���

<���l>
	�Ȃ�
