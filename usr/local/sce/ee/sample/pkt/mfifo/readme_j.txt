[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

MFIFO サンプル

<サンプルの解説>

	このプログラムはMFIFOを用いてオブジェクトの描画を行うサンプル
	です。SPR→Memory 及び Memory→GIF という DMA 転送を行い、SPR
	は DoubleBuffer を、Memory は RingBuffer を行なっています。
	また、MFIFO領域は.cmdファイルを編集してMFIFO領域用のsectionを
	確保しています。

<ファイル>
	mfifo.cmd

<起動方法>

	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run mfifo.elf

<コントローラの操作方法>

	○,×ボタン		：オブジェクトの増減
	△,□ボタン		：オブジェクトサイズの増減
	R1,R2ボタン		：オブジェクト間隔変更
	←,→ボタン		：オブジェクト位置Ｘ軸移動
	↑,↓ボタン		：オブジェクト位置Ｙ軸移動
	L1,L2ボタン		：オブジェクト位置Ｚ軸移動
	startボタン		：ポーズ
	select+○,×ボタン	：オブジェクト数の初期化
	select+R1ボタン		：オブジェクト間隔を０に設定
	select+R2ボタン		：オブジェクト間隔初期化
	select+←,→ボタン	：オブジェクト位置Ｘ軸初期化
	select+↑,↓ボタン	：オブジェクト位置Ｙ軸初期化
	select+L1,L2ボタン	：オブジェクト位置Ｚ軸初期化
	select+startボタン	：オブジェクト位置初期化
				　（Ｘ、Ｙ、Ｚ軸）

<備考>
	※MFIFOを使用する上での注意事項

 -	リングバッファのサイズ(MFIFO_SIZE)は 2のn乗 qword でなけれ
	ばならず、D_RBSRにはMFIFO_SIZE-0x10を指定します。実際には、
	MFIFO_SIZEは0x00200000や0x00008000のようなサイズで、D_RBSR
	には0x001FFFF0や0x00007FF0を設定します。

 -	リングバッファ領域はMFIFO_SIZEアライメントに揃えます。これは、
	(Ａ)global変数として領域を確保する際にアライメントを指定する
	(Ｂ)kernelでユーザーに利用が許可されている領域を使用する
	(Ｃ).cmdファイルを編集してバッファ領域用にsectionを確保する
	などの方法があります。

	(Ａ)	char base[SIZE] __attribute__ ((aligned(SIZE)));
	上記のようにalignedで指定しますが、alignedの制限としてSIZEは
	32KB(0x8000)が最大値となります。従って32KB以上のリングバッファ
	を使用する場合は以下の方法を用います。

	(Ｂ)	char* base = (char*)0x00100000;
	上記のように、ユーザー利用可能領域(0x00100000～0x00200000)
	をポインタ指定して領域を使用します。ユーザーはユーザー利用
	可能領域を管理する必要があります。

	(Ｃ)	.mfifo		ALIGN(0x40000): { *(.mfifo) }
	上記のように、.cmdファイルにセクションを追加し、
		char base[0x40000] __attribute__ ((section(".mfifo")));
	のようにセクションを指定して領域を確保します。

 -	D_RBORとD_RBSRの&を取った値は必ず0にならなければなりません。
	これは上記の条件を満たせば必然的に満たされます。実際には、
	MFIFO_SIZEが0x00040000のとき、D_RBORが0x00240000、D_RBSRが
	0x0003FFF0のようになります。

 -	drainチャネルのSTR bitは、D_RBOR、D_RBSR、及びD_CTRL.MFDを
	設定後、sourceチャネル(DMA8)送信の前に立てておきます。

 -	drainチャネルのSTR bitは、ENDtagが送られたら落ちます。

 -	MFIFO関連のレジスタはQWC以外はbyte単位ですが、QWCはqword単位
	なので注意が必要です。

 -	sourceチャネルがMFIFOに転送可能かの判定は、
	remain = (tadr==madr) ? MFIFO_SIZE
		: (tadr+MFIFO_SIZE-madr)&(MFIFO_SIZE-0x10);
	によって書込み可能領域(remain)の容量を求め、remainが転送しよ
	うとしているsourceチャネルのデータ量よりも大きくなった時です。

	※drainチャネルをGIF経由(path3)とする場合の注意事項

 -	sceGsSwapDBuffはGIF経由でDoubleBufferの切り替えを行なうので
	D_CTRL.MFDを0に戻す(MFIFOを終了する)必要があります。それに
	伴って再度MFIFOを使用する前にD8_MADR、D2_TADR、D2_CHCRなど
	の内容も再設定する必要があります。VIF1経由(path2)の場合は、
	MFIFOの状態にしたままに出来ます。

	※その他の注意事項

 -	sceGsSwapDBuffはGIF経由でDoubleBufferの切り替えを行なうので、
	sceGsDBuff変数がCacheに乗る場合は値を変更したあと、SyncDCache
	などでCacheをWriteBackする必要があります。

