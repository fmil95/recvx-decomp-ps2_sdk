[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

"PlayStation" 用音声付き MPEG2 ストリーム(PSS)の再生


<サンプルの解説>
	mpegstr は、"PlayStation" 用音声付き MPEG2 ストリーム(PSS)
	の再生サンプルプログラムです。mpegstr では、次の要求を満たす
	映像及び音声を再生します。

	- 映像 -

	    Main Profile at Main Level(MP@ML)
	    Simple Profile at Main Level(SP@ML)
	    最大サイズ               :720x480
	    ピクチャ構造             :フレーム構造(プログレッシブ
	                                           フレームが望ましい)
	    
	- 音声 -
	    
	    形式                      :ストレートPCM
	    サンプル長                :16bit
	    エンディアン              :リトルエンディアン
	    サンプリングレート        :48.0KHz
	    チャネル数                :2
	    (L/R)インターリーブサイズ :512

	mpegstr はホストのハードディクドライブまたは、DTL-T10000
	の CD/DVD ドライブに置かれた PSS ファイルを再生します。再生
	もとのデバイスはファイル名の先頭のデバイス名(host0:/cdrom0:)
	で指定します。

	映像と音声の多重化には、ストリームコンバータ ps2str を使用しま
	す。本プログラムの再生するストリームのストリーム番号は
	映像/音声共に 0 である必要があります。多重化のときには、スト
	リーム番号に 0 を指定します。

	プログラムは、読み込みとデコードが別スレッドになっています。
	メインスレッドが sceRead()/sceCdStRead() で読み込みを行う裏
	で、もうひとつのデコードスレッドが MPEG2 のデコードを行います。

	音声は映像と共に EE に読み込み、非多重化した後に IOP 側に戻し
	ます。本サンプルプログラムでは、映像/音声のタイムスタンプを
	チェックしていません。そのため、同期がずれた場合の復帰動作は
	組み込まれていません。本プログラムでは、映像/音声の先頭を合わ
	せた後、再生を開始します。映像の再生速度は vblank が決めます。
	音声の再生速度は、SPU2 の再生速度が決めます。両者には僅かな誤
	差があることがありますが、通常の再生においては問題にならない程
	度です。

	pause/resume を行った場合、音声に関しては最大で 512 サンプル
	(48KHz で 0.0107 sec) 欠落する可能性があります。

	read が間に合わない、または、デコードが間に合わない等の理由に
	より途中のバッファがアンダーフローした場合には、それまでにデ
	コードされたデータが繰り返し再生されます。すなわち、映像に
	関しては、最後にデコードされた絵がそのまま表示されます。音声
	に関してはバッファに残っているデータが繰り返し再生されます。
	このとき、音声と映像の同期がずれる可能性があります。また、バッ
	ファが一杯である限り、新たなデータの取り込みをしませんので、
	バッファがオーバーフローすることはありません。

	関数 sceMpegGetPicture() は、内部で SPR(Scratch Pad RAM) を
	使用します。このため、他で SPR を使う場合には
	sceMpegGetPicture() の間だけは SPR を開放する必要があります。

<ファイル>
	audiodec.c
	audiodec.h
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

<起動方法>
	% make		: コンパイル
	% make run	: 実行(音声付き)
	% make noaudio  : 実行(音声なし)
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
	○ボタン　: 初めから再生
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


