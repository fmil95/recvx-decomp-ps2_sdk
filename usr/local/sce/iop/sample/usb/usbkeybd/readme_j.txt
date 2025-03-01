[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

USBキーボードのサンプルドライバ

<サンプルの解説>
        このプログラムは、USBキーボードのサンプルドライバで、
        USBドライバ(USBD)の使い方のサンプルになります。
	
        このドライバは、以下のディレクトリにあるEE側のサンプルで
        使用されます。
        sce/ee/sample/usb/usbkeybd

        また、このIOP側のドライバのみ実行する場合は、dsidbを使用
        することで表示を確認できます。

<サンプルの仕様>
        1. 同時に８つまでのUSBキーボードが接続できます。
           この値は"usbkeybd.h"の MAX_KEYBD_NUM マクロで変更できます。
        2. 各キーボード毎にリングバッファが用意されているため、EE側の
           データの取りこぼしを防ぐことができます。リングバッファの
           サイズは"usbkeybd.h"の RINGBUF_SIZE マクロで定義されています。
        3. LEDの制御は、このサンプルドライバで行っています。
           CAPS LED の点灯させる仕様は２通り用意されています。
           1. Macintosh のように CAPS キー押下で CAPS-LED 点灯
           2. Windows のように CAPS+Shift キー押下で CAPS-LED 点灯
           これは、"usbkeybd.h"の CAPS_LED_TYPE マクロで定義されています。

<ファイル>
        usbkeybd.c  メインプログラム
        usbkeybd.h  同ヘッダファイル

<起動方法>

        % make                                          ：コンパイル
        % dsreset 0 0                                   ：リセット
        % dsistart /usr/local/sce/iop/modules/usbd.irx
        % dsistart usbkeybd.irx                         ：実行

        表示を確認するには、以下の方法で実行します。
        % dsidb
        > reset 0 0
        > mstart /usr/local/sce/iop/modules/usbd.irx
        > mstart usbkeybd.irx

<表示について>

        各表示例に基づいて説明します。

        [キーボードが接続されたとき]

          dev_id:7  <--- A 
          usbkeybd1: attached (port=2,1,1)
          |_______|            |________|
              B                     C

	  A : デバイスID(1-127)
          B : IOPが割り当てるキーボード番号(0-7)。
          C : USBキーボードが接続されているポート。
              例の場合、
              本体のポート２-->一段目HUBのポート１-->二段目HUBのポート１に
              キーボードが接続されていることを意味している。
        
        [キーボードのデータを受信しているとき]

          usbkeybd1: count=120 led=00 data=( 00 00 00 00 00 00 00 00 )
          |_______|  |_______| |____|        |_____________________|
              D          E        F                     G

          D : このサンプルドライバが割り当てるキーボード番号。
              デバイスIDではない。
          E : このキーボードからデータを受け取った回数。
          F : LEDの状態。
                bit0  :NUM LOCK
                bit1  :CAPS LOCK
                bit2  :SCROLL LOCK
                bit3  :COMPOSE
                bit4  :KANA
                bit5-7:not used
          G : USBキーボードがインタラプト転送で送って来る生データ。
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
        より詳しいUSBキーボードの情報は、
	"http://www.usb.org/developers/devclass_docs.html"で
        配布されている"Human Interface Devices"を参照して下さい。
