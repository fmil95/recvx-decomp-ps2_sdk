[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

EE������IOP����USB�L�[�{�[�h�h���C�o�̏����擾����T���v��


<�T���v���̉��>
        ���̃v���O�����́AUSB�L�[�{�[�h�̃f�[�^��EE������擾���܂��B

        ���s�ɂ�����A�ȉ��̃f�B���N�g���ɂ���IOP����USB�L�[�{�[�h��
        �T���v���h���C�o�����C�N���Ă����K�v������܂��B
        sce/iop/sample/usb/usbkeybd
        �܂��A�v���O�����N���̍ۂɁA�ǂ��炩��USB�|�[�g��USB�L�[�{�[�h
        ��ڑ����Ă��������B

<�t�@�C��>
        main.c        : USB�L�[�{�[�h�h���C�o�̎g�p��T���v��
        usbkeybd.c    : ���C�u��������USB�L�[�{�[�h�擾���[�`��(�T���v���ł�)
        usbkeybd.h�@�@: ���w�b�_�t�@�C��


<�N�����@>
        $ make          �F�R���p�C��
        $ make run      �F���s

        �R���p�C����A�ȉ��̕��@�ł����s�\
        $ dsedb
        > run main.elf

<�\���ɂ���>

	���L�̗�Ɋ�Â��Đ������܂��B

        usbkeybd1 : port=2,1,1 : LED[00] KEY[00 00 00 00 00 00 00 00 ]
        |_______|   |________|   |_____|     |______________________|
            A            B          C                    D

        A : IOP�����蓖�Ă�L�[�{�[�h�ԍ�(0-7)�B
            �f�o�C�XID�ł͂Ȃ��B

        B : USB�L�[�{�[�h���ڑ�����Ă���|�[�g�B
            ��̏ꍇ�A
            �{�̂̃|�[�g�Q-->��i��HUB�̃|�[�g�P-->��i��HUB�̃|�[�g�P��
            �L�[�{�[�h���ڑ�����Ă��邱�Ƃ��Ӗ����Ă���B

        C : LED��ԁB
              bit0  :NUM LOCK
              bit1  :CAPS LOCK
              bit2  :SCROLL LOCK
              bit3  :COMPOSE
              bit4  :KANA
              bit5-7:not used

        D : USB�L�[�{�[�h���C���^���v�g�]���ő����ė��鐶�f�[�^�B
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
        �Ȃ�
