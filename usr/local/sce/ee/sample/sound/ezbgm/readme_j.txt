[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

EzBGM - ディスクからのストリーミングによるBGM再生


<サンプルの解説>
	wavファイルをディスクからストリーミングして再生します。2つのwavファ
	イルを同時かつ非同期に再生できます。各ストリームの再生はコントローラ
	で操作できます。

	このサンプルではホストのハードディスクまたはローカルのCD/DVDディスク
	からストリーミングを行ないます。ネットワークに接続されたDTL-T10000を
	使用する場合、ネットワークの遅延によってスムーズに再生されないことが
	多いようです。

	使用できるwavファイルはWindows形式で、16bit, 48KHz, 無圧縮, ステレオ
	又はモノラル です。ステレオかモノラルかはヘッダ情報から判別して、再生
	モードを切り替えています。

	ステレオのwavファイルは、1サンプル毎にL/Rがインターリーブされています
	が、SPU2で使用するにはは512byteでL/Rインターリーブされている必要がある
	ため、そのままでは再生できません。このサンプルではIOPを使用して、wavの
	PCMをSPU2形式のPCMに並べ替えてます。IOPの負荷は1ファイルにつき2%ほど
	です。

	このサンプルは、IOP側のサンプルと対応しており、両方そろって機能します。
	EE側のプログラムが自動的にIOP側をロードしますので、前もってIOP側のモ
	ジュール（iop/sample/sound/ezbgm)をビルドしておいてください。

<ファイル>
	main.c	
	bgm_rpc.c
	bgm_i.h
	m_stereo.wav
	ps_mono.wav

<起動方法>
	% make		：IOP側のコンパイル（iop/sample/sound/ezbgm/)
	% cd  ???	：EE側のディレクトリに移動（ee/sample/sound/ezbgm/)
	% make		：EE側のコンパイル
	% make run	：実行

	以上で、ホストのHDからWAVファイルを読んで演奏します。

	ローカルのCD/DVDドライブにWAVファイルがある場合は、以下のよう実行します。
	% dsreset 0 0
	$ dsedb
	dsedb S> run main.elf 'cdrom0:\\M_STEREO.WAV;1' 'cdrom0:\\PS_MONO.WAV;1'

	dsedbのコマンドラインでは、以下のように実行します。（ホストHDの場合）
	
	dsedb S> run main.elf host0:m_stereo.wav host0:ps_mono.wav

	１つまたは２つのwavファイルを引数に指定します。


<コントローラの操作方法>
        ○ボタン     :   1番目のwavファイルの演奏開始
        △ボタン     :   1番目のwavファイルの演奏停止
        ×ボタン     :   2番目のwavファイルの演奏開始
        □ボタン　　 :   2番目のwavファイルの演奏停止
        STARTボタン  :   演奏中のファイルの一時停止・再開
	SELECTボタン :   プログラム終了

<備考>
	なし

