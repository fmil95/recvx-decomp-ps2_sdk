[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

CD/DVD-ROM Drive コマンド関数のIOP側呼び出しサンプル

<サンプルの解説>
	CD/DVD-ROMのコマンド関数のテストプログラムです。

	１・CD/DVD-ROM関係のイニシャライズ
	２・コマンド関数の呼び出し
	３・コマンド関数の結果表示
        ４・メディア交換処理

        を順に繰り返し行います。

<ファイル>
	main.c

<起動方法>
	CD/DVD-ROM DriveにPlayStationのCDをセットします。
	ウインドウを二つオープンします。
	  一つのウインドウでdsidbを立ち上げもう一つのウインドウにて
        下記の要領にてプログラムを実行します。
	% dsreset 0 0
	% make		：コンパイル
	% make run	：プログラムを実行します

	  メディア交換のメッセージを表示が出ましたらメディアの交換を
        行ってください。

<コントローラの操作方法>
	なし

<備考>

