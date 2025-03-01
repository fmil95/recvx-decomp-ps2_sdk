[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

スレッドレディキューの回転のサンプル

<サンプルの解説>

このプログラムは、スレッドレディキューの動作を理解するためのサンプルです。

<各スレッド動作>

main プライオリティ0:
    PrintThread1,PrintThread2,PrintThread3を作成して起動し、それぞれを
    READY状態にして、優先度を10に下げます。

PrintThread1 プライオリティ1:
    dsedb コンソールへスレッドIDを出力し、スレッドのレディキューを
    回転させることを繰り返します。

PrintThread2 プライオリティ1:
    dsedb コンソールへスレッドIDを出力し、スレッドのレディキューを
    回転させることを繰り返します。

PrintThread3 プライオリティ1:
    dsedb コンソールへスレッドIDを出力し、スレッドのレディキューを
    回転させることを繰り返します。

<プログラムの動作>

mainから、起動されたPrintThread1,2,3はREADY状態にあり、優先度が同じなので
起動した順にスレッドレディキューに登録されます。

mainの優先度を10に下げると、PrintThread1がRUN状態に移り、スレッドIDを出力
し、スレッドレディキューを回転させます。PrintThread1はスレッドレディキュー
の最後に移り、PrintThread2がRUN状態になります。これがPrintThread3まで実行
され4回繰り返したのち終了します。最後のPrintThread3が終了したら、mainに
制御が戻り、プログラムは正常に終了します。

<ファイル>

	rotate.c

<起動方法>

	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run rotate.elf

<コントローラの操作方法>

	なし
