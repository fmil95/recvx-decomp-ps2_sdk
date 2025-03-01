[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

EzADPCM - ディスクからの ADPCM データストリーミングによる BGM 再生


<サンプルの解説>

	.VB (ADPCM) ファイルをディスクからストリーミングして再生します。
	2 つの .VB ファイルを同時に再生し、ステレオの BGM を構成します。

	このサンプルではホストのハードディスクまたはローカルの CD/DVD 
	ディスクからストリーミングを行います。ネットワークに接続された
	DTL-T10000 を使用する場合、ネットワークの遅延によってはスムー
	ズに再生されないことがあるようです。

	使用できる .VB (ADPCM) ファイルは 1 つの波形データのみ含まれま
	す。その先頭と末尾でループしている必要があります。ファイルサイ
	ズは SPU2 ローカルメモリ内に設定するバッファサイズの整数倍になっ
	ていなければいけません。

	このサンプルは、IOP 側のサンプルと対応しており、両方そろって機
	能します。EE 側のプログラムが自動的に IOP 側をロードしますので、
	前もって IOP 側のモジュール (iop/sample/sound/ezadpcm) を make 
	しておいてください。

<ファイル>
	main.c	
	rpc.c
	ezadpcm.h

<起動方法>
	% make		: IOP 側のコンパイル (iop/sample/sound/ezadpcm/)
	% cd ???	: EE 側のディレクトリに移動 
			  (ee/sample/sound/ezadpcm/)
	% make		: EE 側のコンパイル
	% make run	: 実行

	make run 後、START ボタンを押すことにより、ホストのハードディ
	スクから .VB ファイル (サンプルの 
	ee/sample/spu2/rstream/tr1l(r)_pad.vb) を読んで ADPCM データを再
	生します。

	ローカルの CD/DVD ドライブに .VB ファイルがある場合は、例えば
	以下のよう実行します。

	% dsreset 0 0
	% dsedb
	dsedb S> run main.elf 'cdrom0:\\TR1L_PAD.VB;1' 'cdrom0:\\TR1R_PAD.VB;1'

	dsedbのコマンドラインでは、以下のように実行します (ホスト側ハー
	ドディスクの場合)。
	
	dsedb S> run main.elf host0:tr1l_pad.vb host0:tr1r_pad.vb

	必ず 2 つの .VB (ADPCM) ファイルを引数に指定します。

<コントローラの操作方法>
        ↑（方向キー）:   ボリュームを上げる
        ↓　　　　　  :   ボリュームを下げる
        STARTボタン   :   演奏中のファイルの開始（再開）・停止
	SELECTボタン  :   プログラム終了

<備考>
	なし

