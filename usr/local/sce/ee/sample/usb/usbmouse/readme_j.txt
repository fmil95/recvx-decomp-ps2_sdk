[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

EE������IOP����USB�}�E�X�h���C�o�̏����擾����T���v��


<�T���v���̉��>
        ���̃v���O�����́AUSB�}�E�X�̃f�[�^��EE������擾���܂��B

        ���s�ɂ�����A�ȉ��̃f�B���N�g���ɂ���IOP����USB�}�E�X��
        �T���v���h���C�o�����C�N���Ă����K�v������܂��B
        sce/iop/sample/usb/usbmouse
        �܂��A�v���O�����N���̍ۂɁA�ǂ��炩��USB�|�[�g��USB�}�E�X
        ��ڑ����Ă��������B

<�t�@�C��>
        main.c        : USB�}�E�X�h���C�o�̎g�p��T���v��
        usbmouse.c    : ���C�u��������USB�}�E�X�擾���[�`��(�T���v���ł�)
        usbmouse.h�@�@: ���w�b�_�t�@�C��


<�N�����@>
        $ make          �F�R���p�C��
        $ make run      �F���s

        �R���p�C����A�ȉ��̕��@�ł����s�\
        $ dsedb
        > run main.elf

<�\���ɂ���>

	���L�̗�Ɋ�Â��Đ������܂��B

        usbmouse3 : port=2,4 Button:00 X:  9 Y: -2 W: 12 [00 00 00]
        |_______|   |______| |_______| |_________| |___| |________|
            A           B        C          D        E       F

        A : IOP�����蓖�Ă�}�E�X�ԍ�(0-7)�B
            �f�o�C�XID�ł͂Ȃ��B

        B : USB�}�E�X���ڑ�����Ă���|�[�g�B
            ��̏ꍇ�A
            �{�̂̃|�[�g�Q-->��i��HUB�̃|�[�g�S��
            �}�E�X���ڑ�����Ă��邱�Ƃ��Ӗ����Ă���B

        C : �}�E�X�{�^�����
            bit0:Button1(0:OFF,1:ON)
            bit1:Button2(0:OFF,1:ON)
            bit2:Button3(0:OFF,1:ON)
            bit3-7:Device-specific

        D : USB�}�E�X�̈ړ���
        
        E : �z�C�[���̈ړ���(�z�C�[�����Ȃ��Ƃ��͕\������܂���)

        F : �f�o�C�X�ˑ��f�[�^(�f�o�C�X�ˑ��f�[�^�̂Ȃ��Ƃ��͕\������܂���)

<�R���g���[���̑�����@>
	�Ȃ�

<���l>
        �Ȃ�
