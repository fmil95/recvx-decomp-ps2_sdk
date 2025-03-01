[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

                                                          サンプルの構成
========================================================================

サンプルディレクトリ構成
------------------------------------------------------------------------
(*)印のサンプルは今回追加されたディレクトリです

sce/iop/sample/
├─cdvd
│  ├─smp_iop
│  ├─stmadpcm
│  ├─stmread
│  └─stmspcm
├─hello
├─ilink
├─kernel
｜  ├─module
│  └─thread
├─sif
｜  ├─sifcmd
│  └─sifrpc
├─sound
｜  ├─ezadpcm
｜  ├─ezbgm
｜  ├─ezmidi
｜  ├─sqhard
｜  ├─sqsoft
│  └─voice
├─OLD
│  └─spu2
│  	 ├─autodma
│	 ├─seqplay
│	 ├─stream
│	 └─voice
└─usb
    ├─usbdesc				(*)
    ├─usbkeybd
    └─usbmouse


サンプル一覧
------------------------------------------------------------------------
サンプルコードには以下のものがあります。

cdvd:
	cdvd/smp_iop	IOP側で CD/DVD-ROM Drive コマンド関数を
			呼び出すサンプル
        cdvd/stmadpcm	ADPCMのストリーミング再生サンプル
        cdvd/stmread	CD/DVDドライブからストリーム関数を利用して
			ファイルを読み込むサンプル
        cdvd/stmspcm	ストレートPCM のストリーミング再生サンプル

hello:
	hello		"hello !" を表示するサンプル

ilink:
	ilink		i.LINK(IEEE1394)で通信を行なうサンプル

kernel:
	kernel/module	常駐ライブラリモジュールの作成方法サンプル
	kernel/thread	スレッドの生成と起動、スレッドのプライオリティの
			操作、スレッド間の同期を行うサンプル

sif:
	sif/sifcmd	SIF CMD プロトコルのサンプル
	sif/sifrpc	SIF RPC プロトコルのサンプル

sound:
	sound/ezadpcm	ディスクからの ADPCM データストリーミングにより 
			BGM 再生するサンプル
	sound/ezbgm	ディスクから音声ファイル（WAV）をストリーミング
			再生するサンプル
	sound/ezmidi	MIDI による楽曲と効果音の演奏サンプル
	sound/sqhard	ハードウェア・シンセサイザによるMIDI再生サンプル
	sound/sqsoft	ソフトウェア・シンセサイザによるMIDI再生サンプル
	sound/voice	ボイス発音サンプル

OLD/spu2:
	（libspu2を使用したサンプル）
	spu2/autodma	暫定サウンドライブラリ（libspu2） のAutoDMA転送
			機能を使って、ストレートPCM入力による発音を行う
			サンプル
	spu2/seqplay	暫定サウンドライブラリ（libspu2, libsnd2）を
			使って、IOP上でMIDIシーケンスの再生を行うサンプル
	spu2/stream	暫定サウンドライブラリ（libspu2）を使って、IOP上で
			SPU2のボイスによるストリーム再生を行うサンプル
	spu2/voice	暫定サウンドライブラリ（libspu2）を使って、IOP上で
			SPU2のボイス発音を行うサンプル

usb:
	usb/usbdesc     USBデバイスの静的ディスクリプタをダンプするサンプル
	usb/usbkeybd	USBキーボードのサンプルドライバ
	usb/usbmouse	USBドライバ(USBD)の使い方のサンプル


サンプルコンパイルの前に行っておくこと
------------------------------------------------------------------------
特にありません。
