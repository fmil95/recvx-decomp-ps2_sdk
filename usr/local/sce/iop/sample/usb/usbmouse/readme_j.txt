[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

USB�}�E�X�̃T���v���h���C�o

<�T���v���̉��>
        ���̃v���O�����́AUSB�}�E�X�̃T���v���h���C�o�ŁA
        USB�h���C�o(USBD)�̎g�����̃T���v���ɂȂ�܂��B

        ���̃h���C�o�́A�ȉ��̃f�B���N�g���ɂ���EE���̃T���v����
        �g�p����܂��B
        sce/ee/sample/usb/usbmouse

        �܂��A����IOP���̃h���C�o�̂ݎ��s����ꍇ�́Adsidb���g�p
        ���邱�Ƃŕ\�����m�F�ł��܂��B
        �v���O�����N���̍ۂɂ́A�ǂ��炩��USB�|�[�g��USB�}�E�X
        ��ڑ����Ă��������B

<�T���v���̎d�l>
        1. �����ɂW�܂ł�USB�}�E�X���ڑ��ł��܂��B
           ���̒l��"usbmouse.h"�� MAX_MOUSE_NUM �}�N���ŕύX�ł��܂��B
        2. �e�}�E�X���Ƀ����O�o�b�t�@���p�ӂ���Ă��邽�߁AEE����
           �f�[�^�̎�肱�ڂ���h�����Ƃ��ł��܂��B�����O�o�b�t�@��
           �T�C�Y��"usbmouse.h"�� RINGBUF_SIZE �}�N���Œ�`����Ă��܂��B

<�t�@�C��>

        usbmouse.c  ���C���v���O����
        usbmouse.h  �w�b�_�t�@�C��

<�N�����@>

        % make                                          �F�R���p�C��
        % dsreset 0 0                                   �F���Z�b�g
        % dsistart /usr/local/sce/iop/modules/usbd.irx
        % dsistart usbmouse.irx                         �F���s

        �\�����m�F����ɂ́A�ȉ��̕��@�Ŏ��s���܂��B
        % dsidb
        > reset 0 0
        > mstart /usr/local/sce/iop/modules/usbd.irx
        > mstart usbmouse.irx

<�\���ɂ���>
        
	[�ڑ�����̕\��]
          usbmouse0: attached (port=1)
          |_______|            |____|
              A                   B

          A : �}�E�X�ԍ��B���I�Ɋ��蓖�Ă���B
          B : �ڑ�����Ă���|�[�g�ԍ��B

        [�ڑ����̕\��]
	  usbmouse0: count=1 data=( 00 0f 0f )
          |_______|  |_____|        |______|
              C         D               E

          C : �ڑ����ɓ��I�Ɋ��蓖�Ă�ꂽ�}�E�X�ԍ��B
          D : �}�E�X����f�[�^����M�����񐔁B
          E : �}�E�X���瑗���ė��鐶�f�[�^�B
              byte0   : Button
                          bit0 : Button1
                          bit1 : Button2
                          bit2 : Button3
                          bit3-7 : Device-specific
              byte1   : X(signed char)
              byte2   : Y(signed char)
              byte3   : Wheel(Device-specific)
              byte4-n : Device-specific

<���l>
        ���ڂ���USB�}�E�X�̏��́A
	"http://www.usb.org/developers/devclass_docs.html"��
        �z�z����Ă���"Human Interface Devices"���Q�Ƃ��ĉ������B
