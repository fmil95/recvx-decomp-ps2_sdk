[SCEI CONFIDENTIAL DOCUMENT]

"PlayStation 2" Programmer Tool Runtime Library  Release 2.0

                  Copyright (C) 1999 by Sony Computer Entertainment Inc.

                                                     All Rights Reserved


[３Ｄアイコンファイルのアスキー・バイナリ変換プログラム]

バイナリ形式である３Ｄアイコンファイルを、アスキー形式に
変換するツールです。アスキーになったファイルを再びバイナリ化
することもできるので、３Ｄオーサリングツール等からアスキー形式
で出力した後、それをバイナリ化して用いることも可能です。

iconconv [-b] <input> <output>

inputで指定されたバイナリアイコンファイルを、outputに出力します。
-bをつけると、inputで指定されたアスキーファイルを、
バイナリ化してoutputに出力します。

なおテクスチャのアスキー化、バイナリ化には現状対応していませんので、
アスキー化したものを編集、バイナリに変換した後、
iconconv -b ascii.ico binary.ico
cat binary.ico tex.raw > icon.ico

のようにして、テクスチャをファイルに付加してください。


