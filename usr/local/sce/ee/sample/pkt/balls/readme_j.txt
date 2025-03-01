[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

scratch ballsサンプル

<サンプルの解説>
	高速化のために、スクラッチラム上にあらかじめパケットを作成して
	おき、x,yのフィールドのみを書き換えて、スクラッチから直接 VIF1へ 
	PATH２経由でポリゴンを描画するサンプルプログラムです。

	ボールのデータベースの読み込みにもスクラッチへの DMA 転送を
	用いています。

<ファイル>
	balltex.s
	main.c

<起動方法>
	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run main.elf

<コントローラの操作方法>
	↑ボタン	：ボールを増加
	↓ボタン	：ボールを減少

<備考>
	なし
