[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

"PlayStation" 用音声付き MPEG2 ストリーム(PSS)の再生
				CD/DVDドライブからのストリーミング仕様


<サンプルの解説>
	このサンプル は、"PlayStation" 用音声付き MPEG2 ストリーム(PSS)
	の再生サンプルプログラムです。mpgst_cd では、次の要求を満たす
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
	    チャンネル数              :2
	    (L/R)インターリーブサイズ :512

	映像と音声の多重化には、ストリームコンバータ ps2str を使用しま
	す。本プログラムの再生するストリームのストリーム番号は
	映像/音声共に 0 である必要があります。多重化のときには、スト
	リーム番号に 0 を指定してください。

	プログラムは、読み込みとデコードが別スレッドになっています。
	メインのスレッドが sceRead() で読み込みを行う裏で、もうひとつ
	のスレッドが MPEG2 のデコードを行います。

	音声は映像と共に EE に読み、非多重化した後に IOP 側に戻します。
	本サンプルプログラムでは、映像/音声のタイムスタンプをチェック
	していません。そのため、同期がずれた場合の復帰動作は組み込まれ
	ていません。本プログラムでは、映像/音声の先頭を合わせた後、再
	生を開始します。映像の再生速度は vblank が決めます。音声の再生
	速度は、SPU2 の再生速度が決めます。両者は僅かな誤差があること
	がありますが、通常の再生においては問題にならない程度です。

	read が間に合わない、または、デコードが間に合わない等の理由に
	より途中のバッファがアンダーフローした場合には、それまでにデ
	コードされたデータが繰り返し再生されます。すなわち、映像に
	関しては、最後にデコードされた絵がそのまま表示されます。音声
	に関してはバッファに残っているデータが繰り返し再生されます。
	このとき、音声と映像の同期がずれる可能性があります。また、バッ
	ファが一杯で在る限り、新たなデータの取り込みをしませんので、
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
	util.c
	vibuf.c
	vibuf.h
	videodec.c
	videodec.h
	vobuf.c
	vobuf.h

<起動方法>
	% make		：コンパイル

        ローカルのCD/DVDドライブにMPGファイルの存在するCD/DVD-ROMを挿入します。
	コンソールより以下の手順にて実行します。
        % dsreset 0 0
        $ dsedb
        dsedb S> run main.elf '\\MPGファイル名;1'

<コントローラの操作方法>

	○ボタン : 初めから再生
	×ボタン : 終了 

<備考>
	(a) ps2str ver1.00 では、定ビットレート(CBR)でエンコード
	    された映像のみの多重化に対応しています。可変ビット
	    レート(VBR)でエンコードされた映像は正しく多重化でき
	    ません。VBR には、以降のバージョンで対応します。

