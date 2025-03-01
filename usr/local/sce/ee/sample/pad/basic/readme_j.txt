[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
　　　　　　　　　Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

コントローラ情報を表示するサンプル

<サンプルの解説>

	このプログラムは、コントローラライブラリを用いてボタン情報の取得
	やアクチュエータの設定など一連のコントローラ制御を行うものです。
	ただし、アクチュエータ制御はDUALSHOCKのみに対応しています。

<ファイル>
        main.c

<起動方法>

	% make		：コンパイル

	コンパイル後、dsedb を起動します。
	% dsedb
	> run main.elf
	を実行します

<コントローラの操作方法>

	(DUALSHOCK接続時)
	↑	:	小モータ回転
	↓	:	小モータ停止
	←	:	大モータ回転
	→	:	大モータ停止

<備考>
	コントローラは差込口１(ポート０）に差してください。
	マルチタップ(SCPH-1070)には対応していません。
