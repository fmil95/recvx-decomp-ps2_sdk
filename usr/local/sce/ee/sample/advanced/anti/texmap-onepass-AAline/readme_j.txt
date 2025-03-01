[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

テクスチャマッピングサンプル
+ LINEによるアンチエイリアシング（１度書き版）


<サンプルの解説>
        スペキュラーのある照度計算、テクスチャマッピングを
        行うサンプルに、AA1 LINE によるアンチエイリアシング機能を
	付加した。
	滑らかで無い CUBE 等でもきれいにスムージングがかかるように
        AA1 LINE 表示に Backface Clip 処理を加えた。

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
	add_object.c	：曲面データ生成プログラム
        packet.dsm      ：path1/path2に流すパケットデータ
        matrix.c        ：各種マトリクスをセットする関数
        reftex.dsm      ：テクスチャマッピングのためのテクスチャ

<起動方法>

        % make          ：サンプルのコンパイル
        % make run      ：サンプルの実行

        コンパイル後、以下の方法でも実行可能
        % dsedb
        > run main.elf

<コントローラの操作方法>
	↑,↓,←,→（方向キー）	:カメラの上下左右回転
	△,×,□,○ ボタン	:ライトの上下左右回転
	L1ボタン		:アンチエイリアシング ON-OFF
	L2ボタン		:ＴＦＸトグル Modulate -> Decal 
				  -> Highlight -> Highlight2
				（Highlight と Highlight2 はこの場合同じ）
	R1ボタン		:カメラ前進
	R2ボタン		:カメラ後退
	STARTボタン		:Texture Mapping ON<->OFF
	SELECTボタン		:AA1 LINE のみ描画 <-> Shading も描画

<備考>
	Triangle, Triangle Strip を AA1 off で描いた後、外形線や
	外形候補線を AA1 LINE で上書きすることにより、破綻の少ない
	アンチエイリアシングが出来る。

	Backface Clip 処理は、面により法線が異なる多面体形状や、非常に細か
        い曲面ポリゴンには有効であるが、ポリゴン数の少なく、法線が各頂点で
	共有されている場合は、逆に多面体形状を目立たせてしまうこともある。

	対象に応じて、Backface Clip を用いるかどうか決める必要がある。

