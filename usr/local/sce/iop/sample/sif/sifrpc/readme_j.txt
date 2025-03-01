[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

SIF RPC プロトコルサンプル

<サンプルの解説>
	SIF RPC プロトコルのサンプルプログラムです。

	EE/IOP でサーバ・クライアントの動作をします。片方の
	#define SERVER を生かしてください。そちらがサーバになります。

	クライアントで、データを作成しサーバに送ります。
	サーバは、受け取ったデータを加工してクライアントに送り返します。

	いくつかのモードで、サーバをコールします。

<ファイル>
	iopmain.c

<起動方法>
	% dsreset 0 0
	% make		：コンパイル

	コンパイル後、dsidb を起動します。
	% dsidb

	次に、別のウィンドウを開き、EE 側サンプル
        (ee/sample/sif/sifrpc) より dsedb を起動します

	IOPサンプル側ウィンドウにおいて
	> mstart iopmain.irx
	を実行します

	EEサンプル側ウィンドウにおいて
	> run main.elf
	を実行します

	サーバが、送ったデータに 0xff000000 をマスクして
	クライアントへ送り返していれば正常終了です。

<コントローラの操作方法>
	なし

<備考>
	今回のプログラムは、EE 側とほぼ同一にしてあります。
