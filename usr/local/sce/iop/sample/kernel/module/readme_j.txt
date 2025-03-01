[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

常駐モジュールサンプル

<サンプルの解説>

常駐ライブラリモジュールの作成方法のサンプルです。

<ファイル>

	mylib.c			常駐ライブラリのサンプルソース
	mylib.tbl		常駐ライブラリのエントリテーブルのサンプル
	client.c		常駐ライブラリを利用するプログラムのサンプル

<起動方法>

	% make						：コンパイル
	% dsreset 0 2
	% dsistart mylib.irx; dsistart client.irx	：実行

	以下の方法でも実行可能
	% dsidb
	> reset 0 2 ; mstart mylib.irx; mstart client.irx
