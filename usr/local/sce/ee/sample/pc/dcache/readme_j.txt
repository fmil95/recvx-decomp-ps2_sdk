[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Performance Counter ��p���� D Cache miss ���J�E���g����T���v��

<�T���v���̉��>
	���̃v���O�����́APerformance Counter ��p���� D Cache miss ��
	�J�E���g����T���v���v���O�����ł��B4 �̗Ⴉ�琬�藧���Ă��܂��B

	�ŏ��̗�ł́A�J�E���^ 0 �� "Load Completed" �C�x���g���A
	�J�E���^ 1 �� "D Cache miss" �C�x���g��ݒ肵�A�L���b�V���̈��
	�΂��� lq ���߂��v�����Ă��܂��B�ŏ��̃A�N�Z�X�� D cache miss ��
	�������܂����A�L���b�V���� 1���C���� 64 �o�C�g�Ȃ̂ŁA���� 3 ���
	�A�N�Z�X�Ɋւ��ẮAD cache miss ���������܂���B

	���̎��̗�́A��L���e�̌v�����L���b�V���̈�ɑ΂��čs���Ă��܂��B

	�Ō�̓�̗�́A��L�̓�̏������J�E���^ 0 �̃C�x���g�� 
	"CPU cycle" �Ƃ��čs���Ă��܂��B

<�t�@�C��>
	main.c        : ���C���v���O����

<�N�����@>
	$ make		�F�R���p�C��
	$ make run	�F���s

	�R���p�C����A�ȉ��̕��@�ł����s�\
	$ dsedb
	> run main.elf

<���l>
	�Ȃ�
