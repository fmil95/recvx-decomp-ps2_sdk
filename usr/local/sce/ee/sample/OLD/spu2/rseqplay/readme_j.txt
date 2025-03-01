[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

MIDIシーケンス再生のサンプル

<サンプルの解説>
	暫定サウンドライブラリ librspu2 を使って、EE上からMIDIシーケンスの
	再生を行ないます。

	main.c のdefine行の
		SEQ_FILENAME にSEQファイルを、
		VH_FILENAME  にvhファイルを、
		VB_FILENAME  にvbファイルを指定してください。

	これらのファイルフォーマットは、現行PlayStationと全く同じなので、
	現行ツールを使用して生成してください。

<ファイル>
	main.c	
	fuga.seq
	simple.vh
	simple.vb

<起動方法>
	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run main.elf

	fugaがピアノの音で演奏されれば正常。

<コントローラの操作方法>
	なし

<備考>
	なし
