[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

EE側からIOP側のUSBマウスドライバの情報を取得するサンプル


<サンプルの解説>
        このプログラムは、USBマウスのデータをEE側から取得します。

        実行にあたり、以下のディレクトリにあるIOP側のUSBマウスの
        サンプルドライバをメイクしておく必要があります。
        sce/iop/sample/usb/usbmouse
        また、プログラム起動の際に、どちらかのUSBポートにUSBマウス
        を接続してください。

<ファイル>
        main.c        : USBマウスドライバの使用例サンプル
        usbmouse.c    : ライブラリ風のUSBマウス取得ルーチン(サンプルです)
        usbmouse.h　　: 同ヘッダファイル


<起動方法>
        $ make          ：コンパイル
        $ make run      ：実行

        コンパイル後、以下の方法でも実行可能
        $ dsedb
        > run main.elf

<表示について>

	下記の例に基づいて説明します。

        usbmouse3 : port=2,4 Button:00 X:  9 Y: -2 W: 12 [00 00 00]
        |_______|   |______| |_______| |_________| |___| |________|
            A           B        C          D        E       F

        A : IOPが割り当てるマウス番号(0-7)。
            デバイスIDではない。

        B : USBマウスが接続されているポート。
            例の場合、
            本体のポート２-->一段目HUBのポート４に
            マウスが接続されていることを意味している。

        C : マウスボタン状態
            bit0:Button1(0:OFF,1:ON)
            bit1:Button2(0:OFF,1:ON)
            bit2:Button3(0:OFF,1:ON)
            bit3-7:Device-specific

        D : USBマウスの移動量
        
        E : ホイールの移動量(ホイールがないときは表示されません)

        F : デバイス依存データ(デバイス依存データのないときは表示されません)

<コントローラの操作方法>
	なし

<備考>
        なし
