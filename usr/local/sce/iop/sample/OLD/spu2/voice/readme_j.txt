[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

ボイス発音のサンプル

<サンプルの解説>
	暫定サウンドライブラリ libspu2 を使って、IOP上でSPU2のボイス発音を
	行ないます。オプションで、DMAコールバック、およびIRQコールバックの
	実験ができます。

	VAGファイルのフォーマットは、現行PlayStationと全く同じなので、現行
	ツールを使用して作成してください。

	APIの使い方は、ほぼ現行PlayStationと同様ですが、SPU２ではコアの選
	択が必要です。コア0の出力はコア1のメインボリュームを通る（つまりコ
	ア1のメインボリュームがゼロの場合はコア0の音も発音されない）ことに
	注意してください。

	IRQ_CB_TEST  を1にセットすると、IRQコールバックが有効になり、VAGの
	先頭から0x1000番地目（IRQ_ADDR_OFST）がアクセスされたとき、
	"IRQ interrupt detected"のメッセージがコンソールに出力されます。

	DMA_CB_TEST  を1にセットすると、DMA転送終了コールバックが有効にな
	り、VAGファイルの転送が終了したときに"DMA interrupt detected" のメ
	ッセージがコンソールに出力されます。

<ファイル>
	main.c	
	piano.vag

<起動方法>
	% make		：コンパイル
	% make run	：実行

	Piano音で1オクターブ8音の音階が発音されれば正常。

<コントローラの操作方法>
	なし

<備考>
	なし
