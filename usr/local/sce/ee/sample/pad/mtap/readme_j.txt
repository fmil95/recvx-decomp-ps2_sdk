[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
　　　　　　　　　Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

マルチタップ経由でコントローラを制御するサンプル

<サンプルの解説>

        このプログラムは、コントローラライブラリとマルチタップライブラリ
        を用いてコントローラの制御や情報取得を行うものです。
        アクチュエータ制御はDUALSHOCKのみに対応しています。

<ファイル>
        main.c

<起動方法>

        % make          ：コンパイル

        コンパイル後、dsedb を起動します。
        % dsedb
        > run main.elf
        を実行します

<コントローラの操作方法>

        (DUALSHOCK接続時)
        ↑      :       小モータ回転
        ↓      :       小モータ停止
        ←      :       大モータ回転
        →      :       大モータ停止

<備考>
        マルチタップをコントローラ端子１(ポート０）に差し、コントローラを
	コントローラ端子*-AーDに差してください。
