[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

hello print サンプル

<サンプルの解説>

"hello !\n" を表示したあと、プログラムに渡された引数を一つずつ表示します。

<ファイル>

	thread.c

<起動方法>

	% make						：コンパイル
	% dsreset 0 2 ; dsistart hello.irx 任意の引数	：実行

	以下の方法でも実行可能
	% dsidb
	> reset 0 2 ; mstart hello 任意の引数
