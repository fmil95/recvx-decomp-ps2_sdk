[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

マルチスレッドサンプル

<サンプルの解説>

	このプログラムはマルチスレッドを用いてオブジェクトの描画を行う
	サンプルです。スレッドの切替えにはセマフォを使用し、２つの制御
	用スレッドと、１つのDMA キックスレッド、および複数のオブジェクト
	スレッドから構成されます。１つのオブジェクトスレッドが、１つの
	オブジェクトを表示します。

<ファイル>

	thread.c

<起動方法>

	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run thread.elf

<コントローラの操作方法>

	○,×ボタン		：スレッドの生成と削除
	△ボタン		：回転角を乱数により再設定
	□ボタン		：回転角の初期化
	R1,R2ボタン		：オブジェクト間隔変更
	←,→ボタン		：オブジェクト位置Ｘ軸移動
	↑,↓ボタン		：オブジェクト位置Ｙ軸移動
	L1,L2ボタン		：オブジェクト位置Ｚ軸移動
	startボタン		：ポーズ
	select+△ボタン		：初期化(乱数)
	select+□ボタン		：初期化(整列)
	select+R1ボタン		：オブジェクト間隔を0に設定
	select+R2ボタン		：オブジェクト間隔初期化
	select+←,→ボタン	：オブジェクト位置Ｘ軸初期化
	select+↑,↓ボタン	：オブジェクト位置Ｙ軸初期化
	select+L1,L2ボタン	：オブジェクト位置Ｚ軸初期化

[セマフォ]

    packet_sema：
	パケットへデータを追加可能かどうかを示しています。
	（※実際にはなくても動作します）
    send_signal：
	パケットへのデータ追加が終わりDMA送信可能である事を示します。
    rotate_signal：
	処理が終わり、別オブジェクトへ制御を移して良い事を示します。

[各スレッド動作]

オブジェクトスレッド：
packet_semaを取得する事が出来たらパケットへデータを追加し、packet_semaを
返却したのちrotate_signalを送ります。

コントロールスレッド：
制御が回ってきたらkick_signalを送信したのちにrotate_signalを送ります。
制御が回ってきた時は、一通りのオブジェクトの処理が終わった事を表します。

DMA キックスレッド:
パケットの準備をしてpacket_semaを登録してからkick_signalを待ちます。
kick_signalが来たらパケットの後処理をして、DMA送信を行います。

オブジェクトローテートスレッド:
rotate_signalを待ち、シグナルが来たらpriority2のスレッドの制御対象を回転
させ、別のオブジェクトへ制御を移します。

[スレッド構成図]	

     priority
	0	オブジェクトローテートスレッド
	1	DMA キックスレッド
	2	オブジェクトスレッド
		オブジェクトスレッド
		：
		コントロールスレッド

