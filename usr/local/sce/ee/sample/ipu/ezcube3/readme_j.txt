[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

複数の圧縮データをデコードするサンプル
<サンプルの解説>
	本サンプルは sample ディレクトリの中の ipu/ezcube のサンプルを
	拡張したものです。
	複数の圧縮されたデータを IPUで1つ1つデコードし、テクスチャバッ
	ファにDMA転送し、テクスチャとして表示しています。
	またIPUからのDMA終了割り込みを用いて、1つのデータのデコードが
	終了した直後に次のデコードを行っており、効率良くIPUを利用して
	います。
	
<ファイル>
	main.c
	ldimage.c
	ezcube.c
	ezcube.h
	ezmovie.h
	ez.ipu
	rugby.ipu
	456.ipu
	1_6.ipu
	wada.ipu
	yama.ipu

<起動方法>
	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run main.elf

<コントローラの操作方法>
	↑,↓ボタン	：x軸回転
	←,→ボタン	：y軸回転
	L1,L2ボタン	：z軸回転
	R1,R2ボタン	：立方体のz軸方向の移動
	△,×ボタン	：カメラのx軸回転
	○,□ボタン	：カメラのy軸回転
	STARTボタン	：リセット

