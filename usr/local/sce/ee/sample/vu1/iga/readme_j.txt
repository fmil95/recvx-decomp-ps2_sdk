[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

VU1を用いた透視変換、光源計算のサンプル

<サンプルの解説>
	このプログラムは、VU1で透視変換と光源計算を高速に行うためのサ
	ンプルプログラムです。

<ファイル>
	sample.c	：メインプログラム
	devinit.c	：デバイスの初期化ルーチン
	tex.h		：テクスチャデータ
	work.dsm	：オブジェクトデータ(DMA packet)
	work.vsm	：VU1マイクロプログラム

<起動方法>
	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run sample.elf

<コントローラの操作方法>
	↑ボタン	：前方加速
	↓ボタン	：後方加速
	selectボタン    ：停止（押している間） 

<備考>
	処理の概略は、以下の通りです。

	1. path1 DMAパケット領域２つ用意(dpmem[2])
	2. オブジェクト描画に必要なマトリックスを、SPRAM上に用意
           (SetMatrix())。
	3. DMAパケット領域に2のマトリックスを転送(SetMatrix())。
	4. 前フレームの描画及びVSyncを待つ(sceGsSyncPath(), sceGsSyncV())
	5. 準備ができたDMAパケットをpath1にDMA転送
	6. 2に戻る

