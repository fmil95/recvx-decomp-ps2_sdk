[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
　　　　　　　　　Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

DUALSHOCK2の感圧情報を表示するサンプル

<サンプルの解説>

        このプログラムは、コントローラライブラリを用いてDUALSHOCK2の制御
        と感圧情報の取得を行うものです。

<ファイル>
        main.c
        sprt.c
        sprt.h
        pstypes.h

<起動方法>

        % make          ：コンパイル

        コンパイル後、dsedb を起動します。
        % dsedb
        > run main.elf
        を実行します

<コントローラの操作方法>

        SELECTボタン、STARTボタンを除く任意のボタンを押すと
        押し具合によってゲージが増減します。

<備考>
        コントローラは差込口１(ポート０）に差してください。
        マルチタップ(SCPH-1070)経由での接続には対応していません。
