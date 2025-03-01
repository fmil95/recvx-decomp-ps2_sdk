[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

3次元ポリゴンを表示させるサンプル

<サンプルの解説>
	このプログラムは立方体を透視変換を用いて表示し、コントローラに
	より x軸 y軸 z軸それぞれ独立に回転させるサンプルプログラムです。
	またカメラをx軸 y軸中心に動かすこともできます。

<ファイル>
	main.c

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

