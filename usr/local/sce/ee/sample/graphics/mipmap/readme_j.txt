[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

ミップマップサンプル

<サンプルの解説>
       ミップマップを用いた描画を行うサンプル

<ファイル>
        256.dsm
        128.dsm
        64.dsm
        32.dsm
        16.dsm
        8.dsm
        4.dsm           ：各ミップレベル用のテクスチャ
        main.c          ：メイン関数
        mathfunc.c      ：各種数値演算関数
        rect.dsm        ：板を描画するパケット
        basic.vsm       ：描画用マイクロコード
        mathfunc.h      ：各種パラメータを定義

<起動方法>

        % make          ：ミップマップサンプルのコンパイル
        % make run      ：ミップマップサンプルの実行

        コンパイル後、以下の方法でも実行可能

        % dsedb
        > run main.elf

<コントローラの操作方法>

        ○ボタン        ：板の前後運動の停止

<備考>
        処理の概略は、以下の通りです。

        [main.c]

        1. ダブルバッファの設定、各ミップレベルのテクスチャのロード
        2. テクスチャのレジスタを設定(TEX0_1, TEX2_1, MIPTBP1_1, MIPTBP2_1等）
        3. パッドからの入力値を取得し、その値によってパラメータをセット
        4. MIPMAP on 設定(TEX1_1)
        5. マトリクスを設定
        6. ミップマップされた板（右）を描画
        7. MIPMAP off 設定(TEX1_1)
        8. マトリクスを設定
        9. ミップマップされていない板（左）を描画
        10. 3に戻る

        VU1マイクロの中では以下のような処理を行っています。

        [basic.vsm]

        1. 回転 x 透視変換マトリクスを設定
        2. 各種パラメータ（マトリクス等）をVU1レジスタにロード
        3. 法線ベクトル、頂点座標、ST値、色をロード
        4. 透視変換、光源計算、Q値などを計算して結果をVU1Memにストア
        5. 3. に戻り、頂点数回繰り返し
        6. ストアされたデータ(Gif Packet)をGSへXGKICK


