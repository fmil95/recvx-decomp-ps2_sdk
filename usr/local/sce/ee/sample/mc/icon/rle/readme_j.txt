[SCEI CONFIDENTIAL DOCUMENT]

"PlayStation 2" Programmer Tool Runtime Library  Release 2.0

                  Copyright (C) 1999 by Sony Computer Entertainment Inc.

                                                     All Rights Reserved


[メモリカード３Ｄアイコン用テクスチャ圧縮プログラム]

メモリカードにセーブされる３Ｄアイコンファイルのテクスチャには、
ランレングス符号化を施すことができます。このランレングス符号化を
行うプログラムです。

使用 : rle [-x] <input> <output>

inputのファイルを圧縮し、outputに出力します。
-x をつけると、inputで指定されたファイルを解凍し
outputに出力します。

アイコンに使用するテクスチャはPSMCT16であり、2byte単位での
連続性が高い場合が多いので、2byteを単位としたランレングス符号化を行います。
画像によっては圧縮ができず、サイズが大きくなってしまう場合があります。
そのときには、圧縮せずにテクスチャを用いてください。

一例として、以下のような流れでテクスチャとオブジェクトを組み合わせて
一つのアイコンファイルにすることができます。

1. モデルセクション、アニメーションセクションのみで、テクスチャを含まない、
アイコンファイルaaa.icoを作成する。モデルセクションのアトリビュートにおける
パラメータはセットしておく（TEX=1, RLE=1)。
2. テクスチャをPSMCT16のrawデータとして用意する(tex.raw)。
3. rle tex.raw tex.rle　で圧縮する。
4. cat aaa.ico tex.rle > ICON.ico 等で一つのファイルにする。


