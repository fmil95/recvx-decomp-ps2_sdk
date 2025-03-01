[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

IPU を使用した MPEG2 ストリーム再生サンプル


<サンプルの解説>
	IPU を使用した MPEG2(MPEG1を含む)ストリームの再生サンプル
	プログラムです。再生できる MPEG2 ストリームは

	    Main Profile at Main Level(MP@ML)
	    Simple Profile at Main Level(SP@ML)

	であり、且つフレーム構造のピクチャを持つストリームのみです。
	フィールド構造のピクチャを持つ動画の再生には現在対応しており
	ません。また、MPEG2 のシステム情報の付いたストリームにも対応
	しておりません。

	本サンプルプログラムでは、MPEG2 データを一旦すべてメモリ上に
	読み込みます。読み込んだデータを関数 sceMpegAddBs() を用いて
	IPU へ送り、関数 sceMpegGetPicture() を用いて 1 ピクチャづつ
	デコードします。デコード結果は、メインメモリ上に展開されます
	ので、これを GS に DMA 転送して画面に表示します。

	フレーム構造をとるピクチャは、一つのフレームの中に櫛歯状に
	偶数および奇数フィールドを持ちます。各フィールドが切り替わ
	るのが 1/60 sec ですので、ピクチャ自体は 1/30 sec で切り替わる
	必要があります。そこで、プログラム中では 2 vblank で一回
	ピクチャが切り替わるように指定しています。

	関数 sceMpegGetPicture() は、内部で SPR(Scratch Pad RAM) を
	使用します。このため、他で SPR を使う場合には
	sceMpegGetPicture() の間だけは SPR を開放する必要があります。

<ファイル>
        ezmeg.h
        display.c
        ldimage.c
        main.c
	ez.m2v

<起動方法>
	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run ezmpeg.elf ez.m2v

<コントローラの操作方法>
	なし

<備考>
	なし
