[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved


"PlayStation 2"��̃V�X�e���ݒ�ɐݒ肳��Ă�������擾����T���v��

<�T���v���̉��>

"PlayStation 2"��̃V�X�e���ݒ�ɐݒ肳��Ă�������擾���āA
�R���\�[���ɏo�͂��܂��B���t�A�����̌`���̓V�X�e���ݒ肩��擾����
���̂��g���Ă��܂��B

DTL-T10000 ��ɂ́A�V�X�e���ݒ肪����܂���B���̂��� DTL-T10000 ���
�V�X�e���ݒ�͐ݒ�t�@�C���iT10000.scf�j��p�ӂ��� sceScfT10kSetConfig
�֐��� DTL-T10000 ��̂ݗL���ȃV�X�e���ݒ��ݒ肵�Ă��܂��B
�ݒ�t�@�C�� T10000.scf �̃t�H�[�}�b�g�́A�ȉ��̂悤�ɂȂ��Ă��܂��B

#DTL-T10000 System Configuration File T10000.scf
#
[SPDIF]         ON              # ON OFF
[ASPECT]        FULL            # 4:3 FULL 16:9
[LANGUAGE]      English         # Japanese English French
				  Spanish German Italian
 				  Dutch Portuguese
[TIMEZONE]      0               # from -720 to 780
[SUMMERTIME]    OFF             # ON OFF
[TIMENOTATION]  24HOUR          # 24HOUR 12HOUR
[DATENOTATION]  YYYY/MM/DD      # YYYY/MM/DD MM/DD/YYYY DD/MM/YYYY

#�ȍ~�̓R�����g�ɂȂ��Ă��܂��B
[]�Ŋ���ꂽ���̂��ݒ荀�ڂŁA�X�y�[�X�������Đݒ���e���������݂܂��B
�e�s�̃R�����g�ɐݒ�ł�����e�������Ă���܂��B

���̃t�@�C�����J���Ȃ����́A�V�X�e���R���t�B�O���[�V�������C�u�����T�v
�ilibscf.doc�j�ɏ�����Ă��� DTL-T10000 �f�t�H���g�ݒ���擾���܂��B


<�萔�̒�`>
�e���r��ʂ�PAL�ŏo�͂��鎞�́APAL���`���ĉ������B

<�t�@�C��>
     T10000.scf
     scf.c

<�N�����@>

     % make         �F�R���p�C��
     % make run     �F���s

     �R���p�C����A�ȉ��̕��@�ł����s�\
     % dsedb
     > run scf.elf

<�R���g���[���̑�����@>

     �Ȃ�


