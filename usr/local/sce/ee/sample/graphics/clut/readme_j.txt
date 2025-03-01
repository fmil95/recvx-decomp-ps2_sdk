[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

4bit/8bit clut テクスチャの表示サンプル

<サンプルの解説>
        このプログラムは、4bit/8bitCLUTのテクスチャを表示するものです。

<ファイル>
        clut.c          ：メインプログラム
        tex8.dsm        ：8 bit テクスチャデータ
        tex4.dsm        ：4 bit テクスチャデータ

<起動方法>
        % make          ：コンパイル
        % make run      ：実行

        コンパイル後、以下の方法でも実行可能
        % dsedb
        > run clut.elf

<コントロールの操作方法>
	なし

<処理概略>
        処理の概略は、以下の通りです。

        1. 描画設定の初期化
        2. 4, 8bitのインデックスイメージのGSフレームバッファへのロード
        3. パケットの作成
        4. 8bit用clut設定用のインデックス配列の設定
        5. clut animationのためフレーム番号をもとに4,8bitのclutを更新し、
           GSに転送する。
        6. ダブルバッファのスワップ
        7. 左側スプライト（8bit texture)の描画
        8. 右側スプライト（4bit texture)の描画
        9. 5.に戻る

