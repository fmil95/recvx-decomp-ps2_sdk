[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

VSyncStart���荞�݃n���h���̃T���v��

<�T���v���̉��>

���̃v���O�����́A�X���b�h��VSyncStart���荞�݃n���h���̓���𗝉�����
���߂̃T���v���ł��B


<�e�X���b�h����>

main �v���C�I���e�B0:
    ReadThread���쐬���ċN�����AVSyncStart���荞�݂ɓ�������dsedb 
    �R���\�[���ɕ�������o�͂��܂��B

ReadThread �v���C�I���e�B1:
    main��MainSema�Z�}�t�H�҂��̂����������t�@�C����ǂ݂܂��B
    �I�������main���I���ł���悤��ReadEnd�Z�}�t�H��������A���X���b�h��
    �폜���܂��B

<���荞��>

VSyncStart�̂��т�MainSema�Z�}�t�H��������܂��B

<�e�Z�}�t�H>

main       -- MainSema    ���荞�݂��Ƃ�main��RUN��Ԃɂ��邽�߂̃Z�}�t�H
ReadThread -- ReadEndSema �t�@�C���ǂݍ��݂̏I���̍��}�������Z�}�t�H

<�v���O�����̓���>

�v���O�����ɂ́Amain��ReadThread��2�̃X���b�h������AVSyncStart���荞��
�n���h���̂��т�main��������J�n����d�g�݂����܂��B���̂��߂ɃX���b�h��
�v���C�I���e�B�́Amain���ReadThread��Ⴍ���܂��B

Handler     V          V          V          V
main        |-------   |-------   |-------   |
ReadThread  |       ---|       ---|       -- |

ReadThread�̏��������S�ɏI���������ǂ����́Amain�̒���PollingSema�֐���
�m�F�����܂��B

�����ŁAmain��WAIT��Ԃɂ�����@�ł����A

    WaitSema
    SleepThread

�̓�ʂ�̕��@������܂��B�ǂ���ł��������Ƃ��\�ł����A�_�����
�Z�}�t�H��p��������D�܂����̂ŁA�T���v���ł̓Z�}�t�H��p���܂��B

VSyncStart���荞�݃n���h���̂Ȃ��ł́AiSignalSema���Ăяo�����ƂŁA
main��RUN��Ԃɂ��܂��B

VSyncStart���荞�݂�L���ɂ���܂��ɁAmain��WAIT��Ԃɂ��܂��BRUN��Ԃ�
�Ƃ��Ƀn���h���̒���iSignalSema���Ă΂��ƁAmain���{��WAIT��ԂɂȂ�
�ׂ��Ƃ���ŁAWAIT��ԂɂȂ炸�Ɉ�������RUN��ԂɂȂ�̂�h�����߂ł��B

�t�@�C���̓ǂݍ��݂��I������ƁAReadThread��main���I�������邽�߂�
ReadEnd�Z�}�t�H��������܂��B���̂Ƃ�ExitDeleteThread���Ăяo���O��
VSyncStart���荞�݂���������ƁAmain�ɐ��䂪�߂��āAmain���I�����܂��B
ReadThread���I�������邽�߂ɁAReadThead���main�̗D��x�������܂��B


<�t�@�C��>

	vsync.c

<�N�����@>

	% make		�F�R���p�C��
	% make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	% dsedb
	> run vsync.elf

<�R���g���[���̑�����@>

	�Ȃ�
