[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

USBデバイスの静的ディスクリプタをダンプするサンプルプログラム

<サンプルの解説>
        このプログラムは、USBデバイスの静的ディスクリプタを
        ダンプするサンプルプログラムです。
        USBドライバ(USBD)の使い方のサンプルになります。

<ファイル>

        usbdesc.c  メインプログラム

<使用方法>

1. "PlayStation 2"のUSBポートに何も繋がないようにして下さい。
2. $ make run
3. ディスクリプタをダンプしたいUSBデバイスを"PlayStation 2"に挿入して
   ください。
   コンソールには、下記のように表示されると思います。

　　New device plug-in!!!
　　DEVICE-ID:2
　　Dump end

4. カレントディレクトリのファイルを見てください。
   "dev_id2.dsc"がデバイスディスクリプタのファイルです。

　　dsidb R> quit
　　$ ls
　　Makefile      dev_id2.dsc   readme_j.txt  usbdesc.irx
　　PathDefs      readme_e.txt  usbdesc.c     usbdesc.o
　　$ 

