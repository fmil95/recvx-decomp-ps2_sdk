[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

USB�L�[�{�[�h�̃T���v���h���C�o

<�T���v���̉��>
        ���̃v���O�����́AUSB�L�[�{�[�h�̃T���v���h���C�o�ŁA
        USB�h���C�o(USBD)�̎g�����̃T���v���ɂȂ�܂��B
	
        ���̃h���C�o�́A�ȉ��̃f�B���N�g���ɂ���EE���̃T���v����
        �g�p����܂��B
        sce/ee/sample/usb/usbkeybd

        �܂��A����IOP���̃h���C�o�̂ݎ��s����ꍇ�́Adsidb���g�p
        ���邱�Ƃŕ\�����m�F�ł��܂��B

<�T���v���̎d�l>
        1. �����ɂW�܂ł�USB�L�[�{�[�h���ڑ��ł��܂��B
           ���̒l��"usbkeybd.h"�� MAX_KEYBD_NUM �}�N���ŕύX�ł��܂��B
        2. �e�L�[�{�[�h���Ƀ����O�o�b�t�@���p�ӂ���Ă��邽�߁AEE����
           �f�[�^�̎�肱�ڂ���h�����Ƃ��ł��܂��B�����O�o�b�t�@��
           �T�C�Y��"usbkeybd.h"�� RINGBUF_SIZE �}�N���Œ�`����Ă��܂��B
        3. LED�̐���́A���̃T���v���h���C�o�ōs���Ă��܂��B
           CAPS LED �̓_��������d�l�͂Q�ʂ�p�ӂ���Ă��܂��B
           1. Macintosh �̂悤�� CAPS �L�[������ CAPS-LED �_��
           2. Windows �̂悤�� CAPS+Shift �L�[������ CAPS-LED �_��
           ����́A"usbkeybd.h"�� CAPS_LED_TYPE �}�N���Œ�`����Ă��܂��B

<�t�@�C��>
        usbkeybd.c  ���C���v���O����
        usbkeybd.h  ���w�b�_�t�@�C��

<�N�����@>

        % make                                          �F�R���p�C��
        % dsreset 0 0                                   �F���Z�b�g
        % dsistart /usr/local/sce/iop/modules/usbd.irx
        % dsistart usbkeybd.irx                         �F���s

        �\�����m�F����ɂ́A�ȉ��̕��@�Ŏ��s���܂��B
        % dsidb
        > reset 0 0
        > mstart /usr/local/sce/iop/modules/usbd.irx
        > mstart usbkeybd.irx

<�\���ɂ���>

        �e�\����Ɋ�Â��Đ������܂��B

        [�L�[�{�[�h���ڑ����ꂽ�Ƃ�]

          dev_id:7  <--- A 
          usbkeybd1: attached (port=2,1,1)
          |_______|            |________|
              B                     C

	  A : �f�o�C�XID(1-127)
          B : IOP�����蓖�Ă�L�[�{�[�h�ԍ�(0-7)�B
          C : USB�L�[�{�[�h���ڑ�����Ă���|�[�g�B
              ��̏ꍇ�A
              �{�̂̃|�[�g�Q-->��i��HUB�̃|�[�g�P-->��i��HUB�̃|�[�g�P��
              �L�[�{�[�h���ڑ�����Ă��邱�Ƃ��Ӗ����Ă���B
        
        [�L�[�{�[�h�̃f�[�^����M���Ă���Ƃ�]

          usbkeybd1: count=120 led=00 data=( 00 00 00 00 00 00 00 00 )
          |_______|  |_______| |____|        |_____________________|
              D          E        F                     G

          D : ���̃T���v���h���C�o�����蓖�Ă�L�[�{�[�h�ԍ��B
              �f�o�C�XID�ł͂Ȃ��B
          E : ���̃L�[�{�[�h����f�[�^���󂯎�����񐔁B
          F : LED�̏�ԁB
                bit0  :NUM LOCK
                bit1  :CAPS LOCK
                bit2  :SCROLL LOCK
                bit3  :COMPOSE
                bit4  :KANA
                bit5-7:not used
          G : USB�L�[�{�[�h���C���^���v�g�]���ő����ė��鐶�f�[�^�B
              �L�[�{�[�h�ɂ���ẮA����ɒ����f�[�^�������ė���\����
              ����܂��B
                byte0 :Modifier keys
                         bit0:Left-Ctrl
                         bit1:Left-Shift
                         bit2:Left-Alt(Win),Left-option(Mac)
                         bit3:Left-Win(Win),Left-Apple(Mac)
                         bit4:Right-Ctrl
                         bit5:Right-Shift
                         bit6:Right-Alt(Win),Right-option(Mac)
                         bit7:Right-Win(Win),Right-Apple(Mac)
                byte1 :�\��
                byte2 :�L�[�R�[�h
                  :
                byte7 :�L�[�R�[�h

	�L�[�R�[�h�ɂ��ẮA
        "http://www.usb.org/developers/devclass_docs.html"��
        �z�z����Ă���"HID Usage Tables Document"���Q�Ƃ��ĉ������B


<�R���g���[���̑�����@>
	�Ȃ�

<���l>
        ���ڂ���USB�L�[�{�[�h�̏��́A
	"http://www.usb.org/developers/devclass_docs.html"��
        �z�z����Ă���"Human Interface Devices"���Q�Ƃ��ĉ������B
