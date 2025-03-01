[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
　　　　　　　　　Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

回転軸内挿によるキーフレームマトリクスアニメーションのランタイムサンプル

<サンプルの解説>
	intr_key 以下のプログラムは、回転軸内挿という方法で、関節のマ
	トリクスによるキーフレームアニメーションを行なうサンプルです。

	このディレクトリ runtime のプログラムは、回転軸内挿用のパラメータ
	シーケンスを使用して実際のキーフレームアニメーションを行ないます。

<ファイル>
	main.c          : メイン関数、一般表示関連関数
	matintr.c       : キーフレームアニメーション関連関数
	matintr.h       : ヘッダファイル
	axis.c          : 回転軸内挿関連関数
	axis.h          : ヘッダファイル
	aseq.c          : パラメータシーケンスデータ読み込み用
	aseq.h          : ヘッダファイル
	axisseq.dat     : パラメータシーケンスデータ実体 (mkdata で作成した			     もの)
	basic.vsm       : VU1 マイクロプログラム
	dma.dsm         : DMA 用データ一般
	torso.dsm       : 胴体部モデルデータ
	head.dsm        : 頭部モデルデータ
	arm.dsm         : 腕部モデルデータ
	leg.dsm         : 脚部モデルデータ

<起動方法>
	% make		：コンパイル
	% make run	：実行

	コンパイル後は、以下の方法でも実行可能
	% dsedb
	> run main.elf

<コントローラの操作方法>
	○,△,×,□,R1,R2ボタン	: カメラ位置変更
	STARTボタン		: リプレイ

<備考>
	回転内挿用のパラメータシーケンスは、mkdata ディレクトリのサンプル
	プログラムを使用して作成できます。

