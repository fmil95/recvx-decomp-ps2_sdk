[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

スレッド優先度のサンプル

<サンプルの解説>

このプログラムは、スレッドの優先度を理解するためのサンプルです。

<各スレッド動作>

main プライオリティ0:
    ReadThread、PrintThreadを作成して起動し、それぞれをREADY状態にして、
    優先度を10に下げてREADY状態へ移行します。

ReadThread プライオリティ1:
    dsedb コンソールへスレッドIDを出力し、ファイルを読みつづけます。

PrintThread プライオリティ2:
    dsedb コンソールへスレッドIDを出力しつづけます。

<セマフォ>

ReadEndSema -- ReadThread ファイルの読み込みの終了を示すセマフォ

<プログラムの動作>

mainから、起動されたReadThread、PrintThreadはREADY状態にあります。

mainの優先度をReadThreadとPrintThreadより低くすると、優先度が高いReadThread
がRUN状態になります。ReadThreadは、ファイルのオープンまたは読み込みを行なう
と、一時的にWAIT状態になります。ReadThreadが、WAIT状態に遷移すると、優先度
の低いPrintThreadはRUN状態に移行します。

ReadThreadはファイルの読み込みを終了すると、ReadEndSemaを解放し、
PrintThreadを終了させます。ReadThreadが終了した後、mainが終了します。


<ファイル>

	priority.c

<起動方法>

	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run priority.elf

<コントローラの操作方法>

	なし
