[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

IPU を使用した動画の簡易再生サンプル

<サンプルの解説>
	IPU の IDEC コマンドのみを使用した簡易動画再生サンプルプログラム
	です。

	圧縮したデータ全体を IPU に DMA 転送します。各フレームごとに
	1 つのIDEC コマンドを発行し、そのフレームのデコードを実行します。
	デコードしたデータは PATH3 を通して GS に DMA 転送します。

	本サンプルで使用するデータは、IDEC が解釈するデータを必要なマクロ
	ブロック数分だけ並べたものです。
	フレーム間の相関は取られていません。

	EE の core はほとんど使用しておらず、また、VU は全く使用しません
	ので、ポリゴン処理など他の処理と多重化するのも容易です。
	本サンプルでは、フレームバッファに転送した画像をそのまま表示して
	いますが、プログラムを変更して動画をテクスチャとして貼ることも
	可能です。

<ファイル>
        ezmovie.h
        ldimage.c
        main.c
	ez.ipu

<起動方法>
	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run ezmovie ez.ipu

<コントローラの操作方法>
	なし

<備考>
	現在、本サンプルで使用している動画データを作成するツール等
	はリリースされていません。
	
