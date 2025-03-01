[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

ボイス発音のサンプル

<サンプルの解説>
	libsdのサンプルです。IOP上でSPU2のボイス発音を
	行ないます。オプションで、バッチ処理、DMAコールバック、およびIRQコ
	ールバックの実験ができます。

	VAGファイルのフォーマットは、現行PlayStationと全く同じなので、現行
	ツールを使用して作成してください。

	SPU2ではコアの選択に注意が必要です。コア0の出力はコア1のメインボリ
	ュームを通る（つまりコア1のメインボリュームがゼロの場合はコア0の音
	も発音されない）ことに注意してください。

	BATCH_MODEを1にすると、これまで独立のAPIで行なっていたことを、バッ
	チ処理で行ないます。EE側メモリに書きこむテストも行うので、dsedbを立
	ち上げて、メモリをダンプして確認してください。

	IRQ_CB_TEST  を1にセットすると、IRQコールバックが有効になり、VAGの
	先頭から0x1000番地目（IRQ_ADDR_OFST）がアクセスされたとき、
	"interrupt detected"のメッセージがコンソールに出力されます。

	DMA_CB_TEST  を1にセットすると、DMA転送終了コールバックが有効にな
	り、VAGファイルの転送が終了したときに"interrupt detected" のメッセ
	ージがコンソールに出力されます。

<ファイル>
	init_bat.h
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
