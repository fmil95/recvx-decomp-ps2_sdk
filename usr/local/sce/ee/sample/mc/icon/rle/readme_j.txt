[SCEI CONFIDENTIAL DOCUMENT]

"PlayStation 2" Programmer Tool Runtime Library  Release 2.0

                  Copyright (C) 1999 by Sony Computer Entertainment Inc.

                                                     All Rights Reserved


[�������J�[�h�R�c�A�C�R���p�e�N�X�`�����k�v���O����]

�������J�[�h�ɃZ�[�u�����R�c�A�C�R���t�@�C���̃e�N�X�`���ɂ́A
���������O�X���������{�����Ƃ��ł��܂��B���̃��������O�X��������
�s���v���O�����ł��B

�g�p : rle [-x] <input> <output>

input�̃t�@�C�������k���Aoutput�ɏo�͂��܂��B
-x ������ƁAinput�Ŏw�肳�ꂽ�t�@�C�����𓀂�
output�ɏo�͂��܂��B

�A�C�R���Ɏg�p����e�N�X�`����PSMCT16�ł���A2byte�P�ʂł�
�A�����������ꍇ�������̂ŁA2byte��P�ʂƂ������������O�X���������s���܂��B
�摜�ɂ���Ă͈��k���ł����A�T�C�Y���傫���Ȃ��Ă��܂��ꍇ������܂��B
���̂Ƃ��ɂ́A���k�����Ƀe�N�X�`����p���Ă��������B

���Ƃ��āA�ȉ��̂悤�ȗ���Ńe�N�X�`���ƃI�u�W�F�N�g��g�ݍ��킹��
��̃A�C�R���t�@�C���ɂ��邱�Ƃ��ł��܂��B

1. ���f���Z�N�V�����A�A�j���[�V�����Z�N�V�����݂̂ŁA�e�N�X�`�����܂܂Ȃ��A
�A�C�R���t�@�C��aaa.ico���쐬����B���f���Z�N�V�����̃A�g���r���[�g�ɂ�����
�p�����[�^�̓Z�b�g���Ă����iTEX=1, RLE=1)�B
2. �e�N�X�`����PSMCT16��raw�f�[�^�Ƃ��ėp�ӂ���(tex.raw)�B
3. rle tex.raw tex.rle�@�ň��k����B
4. cat aaa.ico tex.rle > ICON.ico ���ň�̃t�@�C���ɂ���B


