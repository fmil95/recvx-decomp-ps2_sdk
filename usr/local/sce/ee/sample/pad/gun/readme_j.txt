[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
　　　　　　　　　Copyright (C) 2000 by Sony Computer Entertainment Inc.                                                 					   All Rights Reserved

ガンコンサンプル

<サンプルの解説>

	このプログラムは、ガンコントローラ(SCPH-00034)の座標上を取得し
        画面上に表示するもです。

<ファイル>
        main.c
        sprt.c
        sprt.h
        pstypes.h

<起動方法>

	% make		：コンパイル

	コンパイル後、dsedb を起動します。
	% dsedb
	> run main.elf
	を実行します

<備考>
        "PlayStation 2"専用マルチタップ経由で使う場合は、スロットAに差
        してください。
