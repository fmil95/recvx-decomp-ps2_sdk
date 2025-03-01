[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

MIDIシーケンス再生のサンプル

<サンプルの解説>
	暫定サウンドライブラリ libspu2, libsnd2 を使って、IOP上でMIDIシ
	ーケンスの再生を行ないます。

	データのファイルフォーマットは、現行PlayStationと全く同じなので、
	現行ツールを使用して生成してください。

<ファイル>
	main.c	
	fuga.seq
	simple.vh
	simple.vb

<起動方法>
	% make		：コンパイル
	% make run	：実行

	fugaがピアノの音で演奏されれば正常。

<コントローラの操作方法>
	なし

<備考>
	なし
