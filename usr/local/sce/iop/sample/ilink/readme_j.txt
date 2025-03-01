[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

i.LINK(IEEE1394)で通信を行なうサンプル

<サンプルの解説>

	このプログラムは、SCE独自のプロトコルであるi.LINKソケット
	を使い、自動生成したデータを２台の"PlayStation 2"で送受信し、その
	結果の正否をモニタに表示します。

	このサンプルは、EE 側のサンプルと対応しており、両方そろって機
        能します。詳しくは EE 側サンプルの readme 
        (ee/sample/ilink/readme_j.txt) を参照して下さい。
	
<ファイル>
	ilsample.c	RPC server

        自動的にロードする IOP モジュール
	ilink.irx (/usr/local/sce/iop/modules)
		 	i.LINK基本ドライバ
	ilsock.irx (/usr/local/sce/iop/modules)
			i.LINKソケットドライバ

<起動方法>
	EE 側サンプルの readme を参照して下さい

<コントローラの操作方法>
	なし

<備考>
	なし
