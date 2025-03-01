[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Performance Counter を用いて D Cache miss をカウントするサンプル

<サンプルの解説>
	このプログラムは、Performance Counter を用いて D Cache miss を
	カウントするサンプルプログラムです。4 つの例から成り立っています。

	最初の例では、カウンタ 0 に "Load Completed" イベントを、
	カウンタ 1 に "D Cache miss" イベントを設定し、キャッシュ領域に
	対する lq 命令を計測しています。最初のアクセスで D cache miss が
	発生しますが、キャッシュの 1ラインは 64 バイトなので、次の 3 回の
	アクセスに関しては、D cache miss が発生しません。

	その次の例は、上記内容の計測を非キャッシュ領域に対して行っています。

	最後の二つの例は、上記の二つの処理をカウンタ 0 のイベントを 
	"CPU cycle" として行っています。

<ファイル>
	main.c        : メインプログラム

<起動方法>
	$ make		：コンパイル
	$ make run	：実行

	コンパイル後、以下の方法でも実行可能
	$ dsedb
	> run main.elf

<備考>
	なし
