[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

アンチエイリアシングライン上書きによる
アンチエイリアシングのサンプルプログラム（Ｚソート無し）

<サンプルの解説>
	このプログラムでは、AA1ライン上書き によるアンチエイリアシング
	効果とその使い方を説明します。

	ポリゴンエッジにAA1ラインを上書きすることによりアンチエリアシング
	効果を得ることができます。このサンプルプログラムでは２次元の
	メッシュデータを生成するとともにラインデータも生成し上書きします。

	これによりシルエット部分のエイリアシングの軽減、メッシュのLODの
	変化点でのT字すきまの軽減ができます。

<ファイル>
	
	Makefile	：make file
	clip.c		：クリップルーチン
	devinit.c	：デバイスの初期化ルーチン
	dma_util.c	：DMAルーチン
	gterrain.c	：地形生成ルーチン
	gterrain_packet.c　：地形パケット生成ルーチン
	main.c		：メインプログラム
	matrix_util.c	：マトリクス計算ルーチン
	misc_packet.c	：その他のパケット生成ルーチン
	random.c	：乱数ルーチン
	texload.c	：テクスチャロードルーチン
	view_util.c	：視点からマトリクス計算ルーチン
	terrain.h	：構造体
	constant.h	：定数
	scale.h		：スケールに依存する定数
	testconst.h	：VU1マイクロプログラム用定数
	*.raw		：テクスチャファイル
	work.dsm	：DMA packet
	fsub.vsm	：VU1マイクロプログラム

<起動方法>
	% make		：コンパイル

	コンパイル後、以下の方法で実行可能
	% dsedb
	> run main.elf

<コントローラの操作方法>
	↑（方向キー）	: 前進
	↓　　		: 後進
	→　　　	: 右進
	←　　　	: 左進
	○ボタン	: 右回転
	□ボタン	: 左回転
	△ボタン	: 下回転
	×ボタン	: 上回転
	R1ボタン	: 遠ざかる
	R2ボタン	: 接近
	L1ボタン	: 上がる
	L2ボタン	: 下がる

	SELECTボタン+
	R1ボタン	: ポリゴンOFF
	L1ボタン	: ポリゴンON
	L2ボタン	: AA1ラインOFF	
	R2ボタン	: AA1ラインON
	
<プログラムの解説> 
	プログラムのメモリーマップ等の解説につきましては mountain.ppt 
	ファイルを参照してください
