[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

ソフトウェア・シンセサイザによるMIDI再生

<サンプルの解説>
	MIDIシーケンサ（modmidi）とソフトウェア・シンセサイザ（modssyn）とEEラインアウト
	(liblout)のサンプルです。

	このサンプルは、IOP側のサンプルと対応しており、両方そろって機能
        します。EE側のプログラムが自動的にIOP側をロードしますので、前も
	ってIOP側のモジュール（iop/sample/sound/sqsoft)をビルドしてお
	いてください。

	ソフトウェア・シンセサイザの音素データは、data/sound/sce.ssb を使用して
        います。これはまだ評価版で、さらにリファインする予定です。

	GMまたはGS準拠の曲データであれば、ほとんどそのまま演奏することが
        できますが、”ドラムパート”の概念が無いために、ドラムパートの
        チャンネルにBankChange(MSB) = 120 を設定しておくことが必要です。
        詳しくは音素データのreadmeをご覧下さい。


	プログラムの動作は以下のようになっています。

       ・EE側プログラムが起動し、IOP側に必要なモジュールをロードする。

       ・EE側の初期化を行なう。

       ・EE側からIOP側のユーザープログラムをロードして起動する。以後はEE側
	 はIOPからのデータ待ち状態になる。

       ・IOP側ではsqをロードし、シーケンスをスタートさせる。modmidiの出力
	 のStream MIDI がmodssynに入力されると、タイムコードを付加してEE側
	 に転送される。（データは独自形式）

       ・MIDIデータが送られてくると、EE側はそれをもとに演算し、Stream-PCM
	 データとして出力する。

       ・そのPCM Streamデータはlibloutの入力となり、SPU2の入出力ブロックに
	 転送される。内部でlibsdrの機能を使用している。


<ファイル>
	main.c	

<起動方法>

	% make		：IOP側のコンパイル（iop/sample/sound/sqsoft/)
	% cd  ???	：EE側のディレクトリに移動（ee/sample/sound/sqsoft/)
	% make		：EE側のコンパイル
	% make run	：実行

	曲が演奏されれば正常。

<コントローラの操作方法>
	なし


