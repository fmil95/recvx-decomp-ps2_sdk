[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

リフレクションマッピング・スペキュラーサンプル


<サンプルの解説>
        スペキュラーのある照度計算、リフレクションマッピングを
        行うサンプル。

<ファイル>

        buff0.dsm       ：テクスチャ転送の設定を行うパケット
        buff1.dsm       ：TEX0_1レジスタの設定を行うパケット
        r.vsm           ：スペキュラー・リフレクションマッピング用
                          マイクロコード（最適化なし）
        r_opt.vsm       ：スペキュラー・リフレクションマッピング用
                          マイクロコード（最適化）
        main.c          ：メイン関数
        mathfunc.c      ：各種数値演算関数
        mathfunc.h      ：各種パラメータを定義
        mug.dsm         ：マグカップモデルデータ（テクスチャオフ）
        mug_tex.dsm     ：マグカップモデルデータ（テクスチャオン）
        packet.dsm      ：path1/path2に流すパケットデータ
        matrix.c        ：各種マトリクスをセットする関数
        reftex.dsm      ：リフレクションマッピングのためのテクスチャ

<起動方法>

        % make          ：サンプルのコンパイル
        % make run      ：サンプルの実行

        コンパイル後、以下の方法でも実行可能
        % dsedb
        > run main.elf

<コントローラの操作方法>
	↑,↓,←,→ ボタン		:カメラの上下左右回転
	△,×,□,○ ボタン		:ライトの上下左右回転

<備考>
	なし

