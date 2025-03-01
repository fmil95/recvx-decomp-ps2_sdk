[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

マルチスレッドサンプル

<サンプルの解説>

スレッドの生成と起動、スレッドのプライオリティの操作、スレッド間の同期等の
基本的なサンプルです。

<ファイル>

	createth.c		スレッドの生成と起動方法の基本的なサンプル
	sleepth.c		SleepThread()/WakeupThread() のサンプル
	eventth.c		SetEventFlag()/WaitEventFlag() のサンプル
	startthargs.c		スレッドの起動サンプル

<起動方法>

	% make					：コンパイル

    createth の実行

	まず、別のウインドウで
	% dsicons 
	を実行しておきます。

	% dsreset 0 2
	% dsistart createth.irx

	dsicons を実行したウインドウに 6 つのスレッドの起動の様子が
	表示されます。

	以下の方法でも実行可能
	% dsidb
	> reset 0 2 ; mstart createth.irx


    sleepth の実行

	まず、二つウインドウを用意しそれぞれで
	% dsicons 0
	と
	% dsicons 1-9
	を実行しておきます。

	そして、さらに別の３つめのウインドウで以下を実行します
	% dsreset 0 2
	% dsistart sleepth.irx

	dsicons 0 を実行したウインドウに '0..5 or a..f or A..F > ' という
	プロンプトが表示されます。
	この時、プロンプトを表示して入力待ちになっているコマンド入力
	スレッドと、その他の 6 つのスレッドが起動しています。
	プロンプトを表示して入力待ちになっているスレッドは他の 6 つの
	スレッドよりもプライオリティが高くなっていますが、キー入力待ちで
	WAIT状態に入っているので、他の ６つのスレッドはそれぞれ実行権を
	獲得しスタートメッセージを表示します。
	

	0 から 5 の数字を入力して、RETURN キーを入力すると対応する
	スレッドを起床(WakeupThread)させます。

	a から f の文字を入力して、RETURN キーを入力すると対応する
	0 から 5 番のスレッドを 2 回起床(WakeupThread)させます。

	A から F の文字を入力して、RETURN キーを入力すると対応する
	0 から 5 番のスレッドを 4 回起床(WakeupThread)させます。

	以下の方法でも実行可能
	% dsidb
	> reset 0 2 ; mstart sleepth.irx


	ウインドウをさらにたくさん用意し、それぞれで
	% dsicons 1
	% dsicons 2
	% dsicons 3
	% dsicons 4
	% dsicons 5
	を起動しておくと、各スレッドが open した TTY 毎に別のウインドウに
	表示することもできます。

    eventth の実行

	まず、二つウインドウを用意しそれぞれで
	% dsicons 0
	と
	% dsicons 1-9
	を実行しておきます。

	そして、さらに別の３つめのウインドウで以下を実行します
	% dsreset 0 2
	% dsistart eventth.irx

	dsicons 0 を実行したウインドウに '0..5,a > ' という
	プロンプトが表示されます。
	この時、プロンプトを表示して入力待ちになっているコマンド入力
	スレッドと、その他の 6 つのスレッドが起動しています。
	プロンプトを表示して入力待ちになっているスレッドは他の 6 つの
	スレッドよりもプライオリティが高くなっていますが、キー入力待ちで
	WAIT状態に入っているので、他の ６つのスレッドはそれぞれ実行権を
	獲得しスタートメッセージを表示します。

	0 から 5 の数字を入力して、RETURN キーを入力するとイベントフラグの
	対応するビットを立てます。

	a を入力して、RETURN キーを入力するとイベントフラグの全部のビット
	を立てます。

	以下の方法でも実行可能
	% dsidb
	> reset 0 2 ; mstart eventth.irx

	ウインドウをさらにたくさん用意し、それぞれで
	% dsicons 1
	% dsicons 2
	% dsicons 3
	% dsicons 4
	% dsicons 5
	を起動しておくと、各スレッドが open した TTY 毎に別のウインドウに
	表示することもできます。

    startthargs の実行

	まず、別のウインドウで
	% dsicons 
	を実行しておきます。

	% dsreset 0 2
	% dsistart startthargs.irx

	dsicons を実行したウインドウに スレッドの起動引数の様子が
	表示されます。

	以下の方法でも実行可能
	% dsidb
	> reset 0 2 ; mstart startthargs.irx
