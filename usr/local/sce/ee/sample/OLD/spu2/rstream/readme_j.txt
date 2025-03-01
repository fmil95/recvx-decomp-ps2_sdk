[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

ボイスによるストリーム再生のサンプル


<サンプルの解説>
	暫定サウンドライブラリ librspu2 を使って、EE上からSPU2のボイスによ
	るストリーム再生を行ないます。

<ファイル>
	main.c	
	tr1l_pad.vb
	tr1r_pad.vb

<起動方法>
	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run main.elf

	音楽がステレオで再生されれば正常。

<コントローラの操作方法>
	なし

<備考>
	なし
