[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

IOP側ストリーム関数の呼び出しサンプル

<サンプルの解説>
	libcdvdのストリーム関数テストプログラムです。

	１・CD/DVD-ROM関係のイニシャライズ
	２・ストリーム関数の呼び出し

	を順に行います。

<ファイル>
	main.c

<起動方法>
	% dsreset 0 0
	% make		：コンパイル

	CD/DVD-ROM Driveに"PlayStation"のCDをセットします。
	ウインドウを二つオープンします。
	  一つのウインドウでdsidbを立ち上げもう一つのウインドウにて
        下記の要領にてプログラムを実行します。
	% make run プログラムを実行します。

	dsidbのウインドウに

	sample end.

	と表示されカレントのディレクトリに"PlayStasion"CDのプログラム
	ファイルがコピーされれば正常終了です。


<コントローラの操作方法>
	なし

<備考>	なし

