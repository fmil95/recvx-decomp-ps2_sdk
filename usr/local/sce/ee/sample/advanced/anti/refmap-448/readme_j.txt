[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

リフレクションマッピング・スペキュラーサンプル
Ｖ方向倍解像度


<サンプルの解説>
        スペキュラーのある照度計算、リフレクションマッピングを
        行うサンプル。

	アンチエイリアシングの為に、640x448 のバッファに描画し
	これをバイリニアで 640x224 の表示バッファに縮小描画している。

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
	↑,↓,←,→ （方向キー）	:カメラの上下左右回転
	△,×,□,○ ボタン		:ライトの上下左右回転
	L1 ボタン			:シェーディング表示
	L2 ボタン			:線画表示
	R1 ボタン			:カメラ前進
	R2 ボタン			:カメラ後退
	SELECTボタン			:バイリニアフィルター
					 （50%,50% blend）
	STARTボタン			:25%,50%,25% blend

<備考>
	なし

<ＧＳバッファ>
	0-69	表示バッファ	640x224 PSMCT32
	70-139	描画バッファ	640x448 PSMCT16 Dither ON
	140-279	Ｚバッファ	640x448	PSMZ24
	300-	反射マップ用テクスチャ


