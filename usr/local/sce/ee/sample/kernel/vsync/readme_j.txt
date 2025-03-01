[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

VSyncStart割り込みハンドラのサンプル

<サンプルの解説>

このプログラムは、スレッドとVSyncStart割り込みハンドラの動作を理解する
ためのサンプルです。


<各スレッド動作>

main プライオリティ0:
    ReadThreadを作成して起動し、VSyncStart割り込みに同期してdsedb 
    コンソールに文字列を出力します。

ReadThread プライオリティ1:
    mainがMainSemaセマフォ待ちのあいだだけファイルを読みます。
    終了するとmainが終了できるようにReadEndセマフォを解放し、自スレッドを
    削除します。

<割り込み>

VSyncStartのたびにMainSemaセマフォを解放します。

<各セマフォ>

main       -- MainSema    割り込みごとにmainをRUN状態にするためのセマフォ
ReadThread -- ReadEndSema ファイル読み込みの終了の合図を示すセマフォ

<プログラムの動作>

プログラムには、mainとReadThreadの2つのスレッドがあり、VSyncStart割り込み
ハンドラのたびにmainが動作を開始する仕組みを作ります。そのためにスレッドの
プライオリティは、mainよりReadThreadを低くします。

Handler     V          V          V          V
main        |-------   |-------   |-------   |
ReadThread  |       ---|       ---|       -- |

ReadThreadの処理が完全に終了したかどうかは、mainの中のPollingSema関数で
確認を取ります。

ここで、mainをWAIT状態にする方法ですが、

    WaitSema
    SleepThread

の二通りの方法があります。どちらでも同じことが可能ですが、柔軟性から
セマフォを用いる方が好ましいので、サンプルではセマフォを用います。

VSyncStart割り込みハンドラのなかでは、iSignalSemaを呼び出すことで、
mainをRUN状態にします。

VSyncStart割り込みを有効にするまえに、mainをWAIT状態にします。RUN状態の
ときにハンドラの中でiSignalSemaが呼ばれると、mainが本来WAIT状態になる
べきところで、WAIT状態にならずに引き続きRUN状態になるのを防ぐためです。

ファイルの読み込みが終了すると、ReadThreadはmainを終了させるために
ReadEndセマフォを解放します。このときExitDeleteThreadを呼び出す前に
VSyncStart割り込みが発生すると、mainに制御が戻って、mainが終了します。
ReadThreadを終了させるために、ReadTheadよりmainの優先度を下げます。


<ファイル>

	vsync.c

<起動方法>

	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run vsync.elf

<コントローラの操作方法>

	なし
