[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
　　　　　　　　　Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

回転軸内挿によるキーフレームマトリクスアニメーションのデータ作成サンプル

<サンプルの解説>
	intr_key 以下のプログラムは、回転軸内挿という方法で、関節のマ
	トリクスによるキーフレームアニメーションを行なうサンプルです。

	このディレクトリ mkdata のプログラムは、もとのキーフレームマト
	リクスシーケンスから、回転軸内挿用のパラメータシーケンスを作成
	しファイルに出力します。

<ファイル>
	main.c          : メイン関数、入出力関数
	axis.c          : 回転軸内挿関連関数
	matrix.c        : 行列演算関数
	matseq.c        : 入力データ (キーフレームマトリクスシーケンス)
	axis.h		: ヘッダファイル
	matrix.h	: ヘッダファイル
	axisseq.ref	: パラメータシーケンスデータ

<起動方法>
	% make		：コンパイル
	% make run	：実行
	もしくは、以下の方法でも実行できます
	% dsedb
	> run *.elf
	
	出力された axisseq.dat ファイルの内容が	axisseq.ref と同一であれば
	正常終了です。

<コントローラの操作方法>
	なし

<備考>
	出力はテキスト形式で axisseq.dat ファイルに書き出します。
	このデータは runtime のサンプルプログラムで利用するものと同じ形式
	です。
	バイナリ形式での出力をするには、writeMatrix(), writeAxis() などを
	適宜変更してください。

