[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

USBマウスのサンプルドライバ

<サンプルの解説>
        このプログラムは、USBマウスのサンプルドライバで、
        USBドライバ(USBD)の使い方のサンプルになります。

        このドライバは、以下のディレクトリにあるEE側のサンプルで
        使用されます。
        sce/ee/sample/usb/usbmouse

        また、このIOP側のドライバのみ実行する場合は、dsidbを使用
        することで表示を確認できます。
        プログラム起動の際には、どちらかのUSBポートにUSBマウス
        を接続してください。

<サンプルの仕様>
        1. 同時に８つまでのUSBマウスが接続できます。
           この値は"usbmouse.h"の MAX_MOUSE_NUM マクロで変更できます。
        2. 各マウス毎にリングバッファが用意されているため、EE側の
           データの取りこぼしを防ぐことができます。リングバッファの
           サイズは"usbmouse.h"の RINGBUF_SIZE マクロで定義されています。

<ファイル>

        usbmouse.c  メインプログラム
        usbmouse.h  ヘッダファイル

<起動方法>

        % make                                          ：コンパイル
        % dsreset 0 0                                   ：リセット
        % dsistart /usr/local/sce/iop/modules/usbd.irx
        % dsistart usbmouse.irx                         ：実行

        表示を確認するには、以下の方法で実行します。
        % dsidb
        > reset 0 0
        > mstart /usr/local/sce/iop/modules/usbd.irx
        > mstart usbmouse.irx

<表示について>
        
	[接続直後の表示]
          usbmouse0: attached (port=1)
          |_______|            |____|
              A                   B

          A : マウス番号。動的に割り当てられる。
          B : 接続されているポート番号。

        [接続中の表示]
	  usbmouse0: count=1 data=( 00 0f 0f )
          |_______|  |_____|        |______|
              C         D               E

          C : 接続時に動的に割り当てられたマウス番号。
          D : マウスからデータを受信した回数。
          E : マウスから送られて来る生データ。
              byte0   : Button
                          bit0 : Button1
                          bit1 : Button2
                          bit2 : Button3
                          bit3-7 : Device-specific
              byte1   : X(signed char)
              byte2   : Y(signed char)
              byte3   : Wheel(Device-specific)
              byte4-n : Device-specific

<備考>
        より詳しいUSBマウスの情報は、
	"http://www.usb.org/developers/devclass_docs.html"で
        配布されている"Human Interface Devices"を参照して下さい。
