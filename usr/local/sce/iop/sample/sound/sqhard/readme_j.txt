[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
            Copyright (C) 1999, 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

ハードウェア・シンセサイザによるMIDI再生

<サンプルの解説>
	MIDIシーケンサ（modmidi）とハードウェア・シンセサイザ（modhsyn）
	のサンプルです。
	cslモジュールの使い方の最も簡単な例にもなっています。

	二つのsqファイル（MIDIシーケンス）とバンクバイナリファイル
	（音色）を読み、演奏します。SCE提供のツールである SMF2SQ およ
	び JAM を用いて、これらのファイルを作成することができます。

	IOP側で、libsd.irx が動作している必要があります。
        ロード状況はdsiconsに表示されるので、うまく動作しない時には
        これらがロードされているかどうか確認してください。

	
<ファイル>
	main.c	
	sakana.sq
	sakana.hd
	sakana.bd
	overload.sq
	overload.hd
	overload.bd

<起動方法>
	% make		：コンパイル
	% make run	：実行

	二つの曲が同時に演奏されれば正常。

<コントローラの操作方法>
	なし

<備考>
	なし
