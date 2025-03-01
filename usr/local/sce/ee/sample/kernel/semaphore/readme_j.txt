[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

セマフォのサンプル

<サンプルの解説>

このプログラムは、VSync割り込みとセマフォを使ってスレッドの動作を制御する
サンプルです。

<各スレッド動作>

main プライオリティ0:
    semamain,sema1,sema2,sema3を作成する。
    PrintThread1,PrintThread2,PrintThread3を作成して起動し、READY状態にす
    る。次にVSyncStart割り込みを有効にして、WaitSemaでWATI状態へ移行します。

PrintThread1 プライオリティ1:
    セマフォsema1を獲得したあと、dsedb コンソールへスレッドIDを出力する
    動作を繰り返します。

PrintThread2 プライオリティ1:
    セマフォsema2を獲得したあと、dsedb コンソールへスレッドIDを出力する
    動作を繰り返します。

PrintThread3 プライオリティ1:
    セマフォsema3を獲得したあと、dsedb コンソールへスレッドIDを出力する
    動作を繰り返します。

<割り込み>

この割り込みのたびにセマフォsema1,sema2,sema3を返却します。

<各セマフォ>

対応関係
main         -- semamain    mainを終了させるためのセマフォ
PrintThread1 -- sema1       PrintThread1を動作させるセマフォ
PrintThread2 -- sema2       PrintThread2を動作させるセマフォ
PrintThread3 -- sema3       PrintThread3を動作させるセマフォ

<プログラムの動作>

PrintThread1,2,3を正常に終了させた後、mainを正常に終了させるために
mainスレッドのプライオリティをPrintThread1,2,3より下げる。
mainスレッドはVSyncStartの割り込みを有効にした後にWAIT状態になります。

mainスレッドがWAIT状態になった後に、VSyncStartの割り込みが発生します。
この割り込みのたびにセマフォsema1,sema2,sema3を返却します。

mainスレッドから、起動されたPrintThread1,2,3はRUN状態になり、WaitSema
関数でWAIT状態になります。VSyncStart割り込みのたびにセマフォ
sema1,sema2,sema3が返却されます。割り込みハンドラから抜けた時、
PrintThread1,2,3は、優先度が同じなのでREADY状態になった順に
スレッドレディキューに登録され、順番に実行されます。

4回繰り返した後、最後にRUN状態になるPrintThread3がExitDeleteThread関数を
呼び出す前に、mainスレッドのsemamainセマフォを解放します。こうすることで、
PrintThread3が終了した後に、優先度が低いmainスレッドへ戻りプログラムが
正常に終了します。

<ファイル>

	semaphore.c

<起動方法>

	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run semaphore.elf

<コントローラの操作方法>

	なし
