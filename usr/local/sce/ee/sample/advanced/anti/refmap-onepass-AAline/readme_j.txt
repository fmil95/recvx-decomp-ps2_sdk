[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

リフレクションマッピング・スペキュラーサンプル
+ LINEによるアンチエイリアシング（１度書き版）


<サンプルの解説>
        スペキュラーのある照度計算、リフレクションマッピングを
        行うサンプルに、AA1 LINE によるアンチエイリアシング機能を
	付加した。

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
        reftex.dsm      ：リフレクションマッピングのためのテクスチャ

<起動方法>

        % make          ：サンプルのコンパイル
        % make run      ：サンプルの実行

        コンパイル後、以下の方法でも実行可能
        % dsedb
        > run main.elf

<コントローラの操作方法>
	↑,↓,←,→ (方向キー）	:カメラの上下左右回転
	△,×,□,○ ボタン	:ライトの上下左右回転
	L1ボタン		:アンチエイリアシング ON-OFF
	L2ボタン		:ＴＦＸトグル Modulate -> Decal -> 
 				 Highlight -> Highlight2
				（Highlight と Highlight2 はこの場合同じ）
	R1ボタン		:カメラ前進
	R2ボタン		:カメラ後退
	STARTボタン		:Texture Mapping ON<->OFF
	SELECTボタン		:AA1 LINE のみ描画 <-> Shading も描画

<備考>
	Triangle, Triangle Strip を AA1 off で描いた後、外形線や
	外形候補線を AA1 LINE で上書きすることにより、破綻の少ない
	アンチエイリアシングが出来る。

	refmap-AAline では、リフレクションマッピングを上書き描画して
	いるが、この sample では、テクスチャファンクションの Highlight
	を用いることで、１度書きで同様のリフレクションを作っている。

	このため、より完全なアンチエイリアシングが実現されている。


