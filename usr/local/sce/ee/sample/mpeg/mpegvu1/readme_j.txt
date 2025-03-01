[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

"PlayStation" 用 MPEG2 ストリーム(PSS)のデコードと VU1 を使った色変換


<サンプルの解説>
	mpegvu1 は、"PlayStation" 用 MPEG2 ストリーム(PSS) の再生サン
	プルプログラムです。mpegvu1 は、IPU で色変換 (YCbCr->RGB)
	を行わずに VU1 を用いて色変換を行います。

	IPU の色変換機能は色差(CbCr)成分に関しては、隣り合う 4 点を
	同じ値として扱います。progressive frame の場合、縦方向、
	横方向ともに同じ時間にサンプリングされた値を持つため、この
	やり方でもほとんど問題はありません。

	ところが、interlace frame および field 構造のピクチャの場合は
	表示された時、時間の異なるフィールドが縦方向に交互に配置されま
	す。このときに同様の色差変換をすると絵によっては細かい色ずれ/
	縞模様といった現象を引き起こします。

	mpegvu1 は、色変換に IPU を用いず、VU1 で高精度の色変換
	を実現したサンプルプログラムです。MPEG2 のデコードの際に、
	VU1 が空いている場合は、この方法によりより美しい MEPG2 デコード
	画像を得ることが出来ます。

	mpegvu1 では、次の要求を満たす映像を再生します。

	- 映像 -

	    Main Profile at Main Level(MP@ML)
	    Simple Profile at Main Level(SP@ML)
	    最大サイズ               :720x480

	mpegvu1 ではピクチャ構造を次のように分類し、それぞれ別の
	VU1 マイクロコードで色変換を実行します。

	    (1) progressive frame structure
	    (2) interlace frame structure
	    (3) field structure

	ビットストリームが上の３つのうちどれに該当するかは、
	デコード後 sceMpeg 構造体のメンバを参照することにより
	知ることが出来ます(videodec.c において csct を算出する
	部分を参照)。
	
	mpegvu1 はホストのハードディクドライブまたは、DTL-T10000
	の CD/DVD ドライブに置かれた PSS ファイルを再生します。再生
	もとのデバイスはファイル名の先頭のデバイス名(host0:/cdrom0:)
	で指定します。

	pause/resume を行った場合、音声に関しては最大で 512 サンプル
	(48KHz で 0.0107 sec) 欠落する可能性があります。

	プログラムの構造はサンプルプログラム mpegstr と同様です。
	詳しくは mpegstr を参照してください。
	    
<ファイル>
	audiodec.c
	audiodec.h
	cscvu1.c
	cscvu1.h
	defs.h
	disp.c
	disp.h
	main.c
	read.c
	readbuf.c
	readbuf.h
	strfile.c
	strfile.h
	util.c
	vibuf.c
	vibuf.h
	videodec.c
	videodec.h
	vobuf.c
	vobuf.h
	vu1.dsm
	yuvfl.dsm
	yuvfl.vsm
	yuvfrfl.dsm
	yuvfrfl0.vsm
	yuvfrfl1.vsm
	yuvprg.dsm
	yuvprg0.vsm
	yuvprg1.vsm

<起動方法>
	% make		：コンパイル
	% make run	：実行(音声付き)
	% make noaudio	：実行(音声なし)
	% make help     : help 情報表示(ファイル名の指定方法を確認できます)

	ホストのハードディスクドライブに PSS ファイルがある場合は、例えば
	以下のように実行します。

	% dsreset 0 0
	% dsedb
	dsedb S> run main.elf 'host0:XXX.pss’
'
	また、DTL-T10000 の CD/DVD ドライブに PSS ファイルがある場合は、例えば
	以下のように実行します。

	% dsreset 0 0
	% dsedb
	dsedb S> run main.elf 'cdrom0:\\XXX.PSS;1’

<コントローラの操作方法>

	○ボタン  : 初めから再生
	×ボタン  : 終了 
	口ボタン  : pause/resume

<備考>
	(a) DTL-T10000 を用いてホスト側からストリーミングする際に
	    は、ネットワークの負荷により再生が乱れることがあります。
	    ネットワークを介してストリーミングする際には、
	    次の点にご注意ください。

		100Base-T 接続にする
		ハブ等を介さない

	(b) /usr/local/sce/data/movie/sample.pss のサンプルデータは、
	    画像の一部に乱れがあります。


