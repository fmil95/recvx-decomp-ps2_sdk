[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

EzMIDI - MIDIによる楽曲と効果音の演奏


<サンプルの解説>
	csl_hsyn、csl_midi、csl_msinを用いて、楽曲と効果音を演奏します。

	このサンプルは、IOP側のサンプルと対応しており、両方そろって機能しま
	す。EE側のプログラムが自動的にIOP側をロードしますので、前もってIOP
	側のモジュール（iop/sample/sound/ezmidi)をビルドしておいてください。

	効果音を演奏するには、libsdを使用する方法もありますが、このサンプル
	ではcsl_hsynを使っています。EE側でcsl_msinを用いてmidi-streamを生成
	し、それをIOP側に転送することで、csl_hsynが演奏を行ないます。

	この方法の利点は、JAMのファイルを活用できることと、SPU2のボイス管理
	をEE側で行なう必要が無いことです。BGM用と効果音用の２つのmidi-stream
	入力ポートに対して、最大音数と優先度を設定することで、
	”効果音用に最大ｎ音まで保証し、残りの（４８－ｎ）～４８音で楽曲を演
	奏する”
	といった制御が可能です。

	EE・IOP間の通信を最小限にするために、新たな効果音がリクエストされた
	時のみ、1フレームに1回だけ、IOPに効果音用のmidi-streamバッファを転送
	します。その結果についてはEE側は関知しません。ボイス管理はcsl_hsynに
	任せられますし、IOP側のループの方が数倍速く回っているため（240Hz）、
	バッファを取りこぼす心配も無いからです。

	EzBGMはBDの分割ロードに対応しています。IOPのメモリを圧迫することなく
	、SPU2のローカルメモリに大きなBDを転送することが可能です。

	BDのSPU2メモリへの転送には、DMAのチャンネル1を使っています。入出力ブ
	ロック転送（sceSdBlockTrans）等を同時に行なう場合には、使用チャンネ
	ルがぶつからないように注意してください。


<ファイル>
	main.c	
	midi_rpc.c
	ezmidi_i.h
	sakana.sq
	sakana.hd
	sakana.bd
	eff.hd
	eff.bd

<起動方法>
	% make		：IOP側のコンパイル（iop/sample/sound/ezmidi/)
	% cd  ???	：EE側のディレクトリに移動（ee/sample/sound/ezmidi/)
	% make		：EE側のコンパイル
	% make run	：実行

	曲が演奏され、方向キーを押せば効果音が発音されます。

<コントローラの操作方法>
        ×ボタン     :   楽曲の演奏再開
        □ボタン     :   楽曲の演奏停止
	↑ボタン     :   効果音（テレフォン）
	→ボタン     :   効果音（コメディ・右）
	←ボタン     :   効果音（コメディ・左）
	↓ボタン     :   効果音（グラス）
        STARTボタン  :   楽曲演奏の一時停止・再開
	SELECTボタン :   プログラム終了

<備考>
	MIDIメッセージは音楽用のフォーマットですので、効果音制御用として見た
	場合、以下の欠点があります。

	・発音ボイスはチャンネル番号とキー番号のみで管理されるので、同じキー
	　を連打して個別にキーオフするようなことは出来ない。
	・パンポット等のコントロールはチャンネル全体にかかってしまう。

	これらを解決するためにいくつか独自の拡張を行なった”拡張MIDIメッセー
	ジ”を定義しています。フォーマットについては CSL概要を、
        API（sceMSIn_PutHsMsg等）については CSL MIDI Stream 生成リファレンスを
        参照してください。

