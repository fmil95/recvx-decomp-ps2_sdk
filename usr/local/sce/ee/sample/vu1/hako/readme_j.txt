[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

VU1を用いた透視変換のサンプル

<サンプルの解説>
	このプログラムは、VU1で透視変換だけを行うサンプルプログラムです。

<ファイル>
	sample.c	メインプログラム
	work.dsm	オブジェクトデータ(DMA packet)
	work.vsm	VU1マイクロプログラム

<起動方法>
	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run sample.elf

<コントローラの操作方法>
	なし

<備考>
	処理の概略は、以下の様になっています。

	1. 表示したいオブジェクトの回転マトリクスを用意する(RotMatirx())
	2. DMAパケット(work.dsm)の回転マトリクスを書き換える(SetMatrix())
	3. 準備ができたDMAパケットをpath1にDMA転送する。
	4. 前フレームの描画及びVSyncを待つ(sceGsSyncPath(), sceGsSyncV())
	5. 2に戻る

	注意
	work.vsmは可読性の為、最適化されておりません。
