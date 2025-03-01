[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

EE側からIOP側のUSBキーボードドライバの情報を取得するサンプル


<サンプルの解説>
        このプログラムは、USBキーボードのデータをEE側から取得します。

        実行にあたり、以下のディレクトリにあるIOP側のUSBキーボードの
        サンプルドライバをメイクしておく必要があります。
        sce/iop/sample/usb/usbkeybd
        また、プログラム起動の際に、どちらかのUSBポートにUSBキーボード
        を接続してください。

<ファイル>
        main.c        : USBキーボードドライバの使用例サンプル
        usbkeybd.c    : ライブラリ風のUSBキーボード取得ルーチン(サンプルです)
        usbkeybd.h　　: 同ヘッダファイル


<起動方法>
        $ make          ：コンパイル
        $ make run      ：実行

        コンパイル後、以下の方法でも実行可能
        $ dsedb
        > run main.elf

<表示について>

	下記の例に基づいて説明します。

        usbkeybd1 : port=2,1,1 : LED[00] KEY[00 00 00 00 00 00 00 00 ]
        |_______|   |________|   |_____|     |______________________|
            A            B          C                    D

        A : IOPが割り当てるキーボード番号(0-7)。
            デバイスIDではない。

        B : USBキーボードが接続されているポート。
            例の場合、
            本体のポート２-->一段目HUBのポート１-->二段目HUBのポート１に
            キーボードが接続されていることを意味している。

        C : LED状態。
              bit0  :NUM LOCK
              bit1  :CAPS LOCK
              bit2  :SCROLL LOCK
              bit3  :COMPOSE
              bit4  :KANA
              bit5-7:not used

        D : USBキーボードがインタラプト転送で送って来る生データ。
            キーボードによっては、さらに長いデータが送られて来る可能性が
            あります。
              byte0 :Modifier keys
                       bit0:Left-Ctrl
                       bit1:Left-Shift
                       bit2:Left-Alt(Win),Left-option(Mac)
                       bit3:Left-Win(Win),Left-Apple(Mac)
                       bit4:Right-Ctrl
                       bit5:Right-Shift
                       bit6:Right-Alt(Win),Right-option(Mac)
                       bit7:Right-Win(Win),Right-Apple(Mac)
              byte1 :予約
              byte2 :キーコード
                :
              byte7 :キーコード

	キーコードについては、
        "http://www.usb.org/developers/devclass_docs.html"で
        配布されている"HID Usage Tables Document"を参照して下さい。


<コントローラの操作方法>
	なし

<備考>
        なし
