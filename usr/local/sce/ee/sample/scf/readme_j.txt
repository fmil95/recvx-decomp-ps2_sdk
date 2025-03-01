[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved


"PlayStation 2"上のシステム設定に設定されている情報を取得するサンプル

<サンプルの解説>

"PlayStation 2"上のシステム設定に設定されている情報を取得して、
コンソールに出力します。日付、時刻の形式はシステム設定から取得した
ものを使っています。

DTL-T10000 上には、システム設定がありません。そのため DTL-T10000 上の
システム設定は設定ファイル（T10000.scf）を用意して sceScfT10kSetConfig
関数で DTL-T10000 上のみ有効なシステム設定を設定しています。
設定ファイル T10000.scf のフォーマットは、以下のようになっています。

#DTL-T10000 System Configuration File T10000.scf
#
[SPDIF]         ON              # ON OFF
[ASPECT]        FULL            # 4:3 FULL 16:9
[LANGUAGE]      English         # Japanese English French
				  Spanish German Italian
 				  Dutch Portuguese
[TIMEZONE]      0               # from -720 to 780
[SUMMERTIME]    OFF             # ON OFF
[TIMENOTATION]  24HOUR          # 24HOUR 12HOUR
[DATENOTATION]  YYYY/MM/DD      # YYYY/MM/DD MM/DD/YYYY DD/MM/YYYY

#以降はコメントになっています。
[]で括られたものが設定項目で、スペースをあけて設定内容を書き込みます。
各行のコメントに設定できる内容が書いてあります。

このファイルが開けない時は、システムコンフィグレーションライブラリ概要
（libscf.doc）に書かれている DTL-T10000 デフォルト設定を取得します。


<定数の定義>
テレビ画面をPALで出力する時は、PALを定義して下さい。

<ファイル>
     T10000.scf
     scf.c

<起動方法>

     % make         ：コンパイル
     % make run     ：実行

     コンパイル後、以下の方法でも実行可能
     % dsedb
     > run scf.elf

<コントローラの操作方法>

     なし


