[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

SIF CMD プロトコルサンプル

<サンプルの解説>
	SIF CMD プロトコルのサンプルプログラムです。

	１・SIF CMD のイニシャライズ
	２・バッファとコマンド関数の登録
	３・相手側のコマンド関数の呼び出し
	４・コマンド関数の結果表示

	を順に行います。

<ファイル>
	iopmain.c

<起動方法>
	% dsreset 0 0
	% make		：コンパイル

	コンパイル後、dsidb を起動します。
	% dsidb

	次に、別のウィンドウを開き、EE 側サンプル
        (ee/sample/sif/sifcmd) より dsedb を起動します

	IOPサンプル側ウィンドウにおいて
	> mstart iopmain.irx
	を実行します

	EEサンプル側ウィンドウにおいて
	> run main.elf
	を実行します

	test0 = 10 test1 = 20

	と表示されれば正常終了です


<コントローラの操作方法>
	なし

<備考>
	今回のプログラムは、EE 側と同一にしてあります。
