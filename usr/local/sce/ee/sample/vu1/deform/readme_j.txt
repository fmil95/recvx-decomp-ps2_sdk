[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

VU1を用いた２形状補間サンプル

<サンプルの解説>
        このプログラムは、VU1で透視変換と光源計算、２つの幾何形状の補間を
	行なうサンプルプログラムです。

<ファイル>
        deform.c	：メインプログラム
        metal.dsm	：テクスチャデータ
        sphere.dsm	：オブジェクトデータ(DMA packet)
        mime.vsm	：VU1マイクロプログラム

<起動方法>
	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run deform.elf

<コントローラの操作方法>
        ○ボタン        ：変形は続けたまま、回転の停止

<備考>
        処理の概略は、以下の通りです。

        1. テクスチャをロード
        2. コントローラオープン
        3. マトリクス、補間重み、光源マトリクスをVU1へ渡すパケット中に
	   セット
        4. VU1へのDMA kick
        5. 2に戻る

        VU1マイクロの中では以下のような処理を行っています。
        1. 回転 x 透視変換マトリクスを設定
        2. 各種パラメータ（マトリクス等）をVU1レジスタにロード
        3. 法線ベクトル、補間する２つの座標、ST値、色をロード
        4. 座標補間、透視変換、光源計算をして結果をVU1Memにストア
        5. 3. に戻り、頂点数回繰り返し
        6. ストアされたデータ(Gif Packet)をGSへXGKICK

        となります。VU1Mem上でのダブルバッファリングも行っています。

