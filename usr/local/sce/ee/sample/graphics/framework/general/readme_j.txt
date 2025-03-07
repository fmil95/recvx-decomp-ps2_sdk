[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved
                                                               Aug. 2000

graphics framework general sample

<サンプルの解説>

	本サンプルは sample/graphics/framework/ カテゴリにおける統合
	グラフィックス環境の、基本コンセプトと各種機能を盛り込んだ、統合環境の
	構築を目指します。
	今後 sample/graphics/framwork/ 内の構成の充実に伴い、順次バージョン
	アップしていく予定です。

	graphics framework の概要については、別ファイルのドキュメント
	over_j.txt をご覧ください。

<ファイル>
	*.h             : プロトタイプ宣言へッダー

	anim.c		: アニメーション用関数群
	camera.c	: カメラパラメータを設定する関数群
	eval.c		: 補間計算の関数群
	framebuf.c	: テクスチャ用フレームバッファ領域管理の関数群
	gmain.c		: データの初期化・読み込み・更新を行う関数群
	hier.c		: モデルの階層構造および共有ポリゴン処理の関数群
	main.c		: メインプログラム
	packbuf.c	: パケッテイングのバッファ領域を確保するための関数群
	vu1frame.c	: モデルデータ群をデータベース管理するための関数群

	micro.dsm	: マイクロコードのラベル名を管理するファイル
	vu1base.vsm	: 平行光源*3 のVU1マイクロコード
	vu1baseo.vsm	: 平行光源*3 のVU1マイクロコード(最適化版)
	vu1cull.vsm	: 平行光源*3 と Culling を行なうVU1マイクロコード
	vu1cullo.vsm	: 平行光源*3 と Culling を行なうVU1マイクロコード
			  (最適化版)

	dino.s		: rawフォーマットデータ
	logo.s		: rawフォーマットデータ
	APEWALK39.s	: サンプルデータ（階層・アニメーション）

	dino.hmd	: HMDデータ
	logo.hmd	: HMDデータ
	APEWALK39.hmd	: LightWave 3D チュートリアルデータのHMDデータ

	本ディレクトリに含まれる、モデルデータおよびシーンデータは、米国 NewTek
	社のLightWave 3D 5.6 により配布されているデータを株式会社ディ・ストーム
	社開発のAnimation Saver v1.8 でエクスポートしました。

<起動方法>

	% make		  ：コンパイル
	% make run	  ：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run main.elf


<コントローラの操作方法>

	↑↓ (方向キー ）	:	カメラx軸回転
	←→ (方向キー ）	:	カメラy軸回転
	L1L2 ボタン   		:	カメラz軸回転
	△× ボタン   		:	カメラ上下移動(y方向)
	□○ ボタン   		:	カメラ左右移動(x方向)
	R1R2 ボタン   		:	カメラ前後移動(z方向)
	STARTボタン   		:	VU1マイクロコード切り替え
	SELECTボタン		:	カメラリセット

<処理概略>
	処理の概略は、以下の通りです。

	g_init();	初期化

			1. GSダブルバッファ初期化
			2. DMAパケットバッファ初期化
			3. カメラパラメータ初期化
			4. VU1マイクロコードマネージャ初期化

	g_read_model();	データ読み込み

			1. データの初期化
			2. データの読み込み
			3. データバッファ生成
			4. DMAパケット生成
			5. バッファオーバフローチェック

	g_main();	メインループ

			1. PAD情報の読み込み
			2. 前処理計算(animation,hierarchy,share)
			3. DMAデータフローリスト作成
			4. DMA転送
			5. ステータス表示
			6. 1 に戻る

	g_finish();	終了処理

			1. DMAパケットバッファ解放
			2. データバッファ解放
			3. 変数の初期化
			4. PADの初期化

<備考>
	データフォーマットの詳細については
	eS パッケージ内の doc/es2raw_j.txtを参照してください。

<制限事項・注意事項>

	常駐・非常駐テクスチャの切り替えはユーザー自身がフラグを設定する
	必要があります。
	デフォルトは非常駐テクスチャ、フラグ0になっています。
	常駐テクスチャフラグを設定する場合はes2raw_j.txtを参照してフラグを1
	に設定してデータに反映させてください。
	また、それぞれのベースポインタは#defineで固定しています。
	ベースポインタを変更する場合はframebuf.hを参照してそれぞれのベース
	ポインタを設定してください。

	階層構造のマトリクススタックサイズは30までとなっています。
	スタックサイズを変更する場合はhier.cを参照してスタックサイズを設定して
	ください。

	階層構造およびアニメーションの回転順序は LightWave3Dに合せてYXZの順に
	なっています。
	変更する場合はhier.cを参照して回転順序を設定してください。
	今後は回転順序はデータに反映する予定です。

	タイリングテクスチャには対応しておりません。

-----------------------------------------------------------------------
商標に関する注意書き
-----------------------------------------------------------------------
LightWave 3D(TM)は、米国NewTek社の商標です。

