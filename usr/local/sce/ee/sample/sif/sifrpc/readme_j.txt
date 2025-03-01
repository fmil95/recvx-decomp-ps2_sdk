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
	main.c

<起動方法>
	% dsreset 0 0
	% make		：コンパイル

	コンパイル後、dsedb を起動します。
	% dsedb

	次に、別のウィンドウを開き、IOP 側サンプル
        (iop/sample/sif/sifrpc) より dsidb を起動します

	EEサンプル側ウィンドウにおいて
	> run main.elf
	を実行します

	IOPサンプル側ウィンドウにおいて
	> mstart iopmain.irx
	を実行します

	サーバが、送ったデータに 0xff000000 をマスクして
	クライアントへ送り返していれば正常終了です。

<コントローラの操作方法>
	なし

<備考>


