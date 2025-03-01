[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

4bit テクスチャclutの一時バッファへのロードを csa/cldフィールドによって
制御するサンプル

<サンプルの解説>
        このプログラムは、4bit テクスチャclutの一時バッファへのロードを
        csa/cldフィールドによって制御するサンプルです。
        ４ビットCLUTは１６個毎に一時バッファにロードされ、これが３組
        あらかじめ用意されています。

<ファイル>
        clut_csa.c      ：メインプログラム
        tex4.dsm        ：4 bit テクスチャデータ、CLUT (48個）データ

<起動方法>
        % make          ：コンパイル
        % make run      ：実行

        コンパイル後、以下の方法でも実行可能
        % dsedb
        > run clut_csa.elf

<コントローラの操作方法>
	なし

<処理概略>
        処理の概略は、以下の通りです。

        1. 描画環境、DMAの初期化
        2. 4インデックスイメージのGSフレームバッファへのロード(My_indeximg4	      )
        3. clutのフレームバッファへのロード(My_clut4_1, 4_2, 4_3)
        4. 描画パケットの初期化
        5. ダブルバッファのスワップ
        6. フレーム毎のパケットの設定
        6-1. １６個の４ビットCLUTを１個の８ビットCLUTとして一度に一時バッフ	        ァにロードするための設定。前のフレームと同じ４ビットCLUTの		     組であれば（同じCBPであれば）ロードされない。
        6-2. CLUTバッファからCLUTをロードすることなく、csaフィールドの示す              オフセットの値によってカレントフレームの描画に用いる4bitCLUTを             指定する(0-15)。
        7. スプライトを描画する
        8. 5.にもどる

