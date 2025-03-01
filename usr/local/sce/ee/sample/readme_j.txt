[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

                                                          サンプルの構成
========================================================================

サンプルディレクトリ構成
------------------------------------------------------------------------
(*)印のサンプルは今回追加されたディレクトリです

sce/ee/sample/
├─advanced
│  └─anti
｜       ├─aa1-feather			(*)
｜       ├─mountain				(*)
｜       ├─pcrtc-blend			(*)
｜       ├─refmap-448				(*)
｜       ├─refmap-4times			(*)
｜       ├─refmap-noAA			(*)
｜       ├─refmap-onepass-AAline		(*)
｜       └─texmap-onepass-AAline		(*)
├─basic3d
│  ├─core
│  ├─vu0
│  └─vu1
├─cdvd
│  ├─smp_ee
│  └─OLD
｜       └─mpgst_cd
├─deci2
│  ├─mouse
│  └─system
├─device
│  └─file
├─graphics
│  ├─anti
│  ├─bumpmap
│  ├─capture
│  ├─clip_vu0
│  ├─clip_vu1
│  ├─clut
│  ├─clut_csa
│  ├─framework
｜  ｜  └─general
│  ├─jointm
｜  ｜  └─intr_key
｜  ｜      ├─mkdata
｜  ｜      └─runtime
│  ├─mipmap
│  ├─point_l	
│  ├─refmap
│  ├─scis_vu0
│  ├─scis_vu1
│  ├─spot_l	
│  ├─tex_swap
│  └─textrans
｜      ├─4BIT
｜      ├─8BIT
｜      └─bitconv
├─ilink
├─ipu
│  ├─ezcube
│  ├─ezcube3	
│  └─ezmovie
├─kernel
│  ├─priority				 (*)
│  ├─rotate					 (*)
│  ├─semaphore				 (*)
│  ├─vsync					 (*)
│  └─thread
├─mc
│  ├─basic
│  └─icon
├─mcx
│  └─basic					 (*)
├─mpeg
│  ├─ezmpeg
｜  ├─mpegstr
｜  ｜   └─OLD
｜  └─mpegvu1
｜       └─OLD
├─pad
│  ├─basic  
│  ├─dual2
│  ├─gun
│  └─mtap
├─pc
│  └─dcache
├─pkt
│  ├─balls
│  ├─cube
│  └─mfifo
├─scf						 (*)
├─sif
│  ├─sifcmd
│  └─sifrpc
├─sound
│  ├─ezadpcm
│  ├─ezbgm
│  ├─ezmidi
│  ├─rblock
│  └─sqsoft
├─OLD
│  └─spu2
│  	 ├─rautodma
│	 ├─rseqplay
│	 ├─rstream
│	 └─rvoice
├─usb
│  ├─usbkeybd
│  └─usbmouse
├─vu0
│  ├─tballs
│  ├─tdiff
│  ├─tfog
│  ├─tmip
│  ├─tskin
│  └─tsquare
└─vu1
　  ├─blow
　  ├─bspline
　  ├─deform
　  ├─hako
　  └─iga

サンプル一覧
------------------------------------------------------------------------
サンプルコードには以下のものがあります。

advanced:
	advanced/anti/aa1-feather	アンチエイリアシング効果のサンプル
	advanced/anti/mountain		アンチエイリアシングライン上書きに
					よるアンチエイリアシングのサンプル
	advanced/anti/pcrtc-blend	PCRTCを使ったアンチエイリアシング
					効果のサンプル
	advanced/anti/refmap-448	リフレクションマッピング・スペキュ
					ラーサンプル（Ｖ方向倍解像度）
	advanced/anti/refmap-4times	リフレクションマッピング・スペキュ
					ラーサンプル（サブピクセルずらしに
					よるアンチエイリアシング）
	advanced/anti/refmap-noAA	リフレクションマッピング・スペキュ
					ラーサンプル
	advanced/anti/refmap-onepass-AAline
					リフレクションマッピング・スペキュ
					ラーサンプル + LINEによるアンチエ
					イリアシング（１度書き版）
	advanced/anti/texmap-onepass-AAline
					テクスチャマッピングサンプル+ LINE
					によるアンチエイリアシング
					（１度書き版）

basic3d:
        3Dのオブジェクトを表示するまでの、基本的な手続きを理解するための
        サンプル
        basic3d/core    cpu のコアの命令のみで記述されたアルゴリズム
                        理解用サンプル
        basic3d/vu0     core のプログラムをもとにメインの処理を VU0 
                        マクロ命令に置き換えたサンプル
        basic3d/vu1     core、vu0 で行われている処理を、さらに VU1 
                        マイクロ命令で実現したサンプル

cdvd:
	cdvd/smp_ee		CD/DVD-ROM Drive コマンド関数を呼び出す
				サンプル
        cdvd/OLD/mpgst_cd   	CD/DVDドライブからのMPEG2ストリームを読み
                        	出し、再生するサンプル 
				注意：このサンプルの機能はmpeg/mpegstr
				サンプルに組み込まれた為、今後更新され
				ません。

deci2:
	deci2/mouse	DECI2 プロトコルを用いたプログラムのダウンロード、
                        及びホスト上のマウスの動きをシミュレートする
			サンプル
	deci2/system    DECI2 プロトコルを用いたホスト上の system() を
			実行するサンプル

device:
        device/file     ホストマシンのファイル操作サンプル

graphics:
	graphics/anti		アンチエイリアスサンプル(ソート&
				ポリゴンクリップ)
	graphics/bumpmap	バンプマッピングサンプル
        graphics/capture        フレームバッファ内容をホストPCの
                                ファイルにセーブするサンプル
	graphics/clip_vu0	Clipping サンプル(VU0) 
	graphics/clip_vu1	Clipping サンプル(VU1)
        graphics/clut           4bit/8bitカラーのテクスチャを表示する
				サンプル
	graphics/clut_csa	4bit テクスチャCLUTの一時バッファへの
				ロードをcsa/cldフィールドによって制御する
				サンプル
	graphics/framework/general	
				統合グラフィックス環境の、基本コンセプトと
				各種機能を盛り込んだ、統合環境の構築を目指
				すサンプル
	graphics/jointm/intr_key/mkdata
				回転軸内挿によるキーフレームマトリクス
				アニメーションのデータ作成サンプル
	graphics/jointm/intr_key/runtime
				回転軸内挿によるキーフレームマトリクス
				アニメーションのランタイムサンプル
        graphics/mipmap         mipmapを行うサンプル
	graphics/point_l	点光源処理をおこなうサンプル
	graphics/refmap		スペキュラーのある照度計算、リフレク
				ションマッピングを行うサンプル
	graphics/scis_vu0	シザリングサンプル(CORE&VU0版)
	graphics/scis_vu1	シザリングサンプル(VU1版)
	graphics/spot_l		スポットライト光を表現するサンプル
	graphics/tex_swap	テクスチャを入れ替えながら描画を行う
				サンプル
	graphics/textrans/4BIT
				4bit-256x256 のテクスチャを 32bit-128x64 
				のテクスチャとして転送するサンプル
	graphics/textrans/8BIT
				8bit-256x256 のテクスチャを 32bit-128x128
				のテクスチャとして転送するサンプル
	graphics/textrans/bitconv
				PSMT4/8 の raw-image を PSMCT32 で転送可能
				なビット配置に変換するオフラインサンプル

ilink:
	ilink		i.LINK(IEEE1394)で通信を行なうサンプル

ipu:
	ipu/ezcube	3次元ポリゴンに IPU を用いてデコードした
			テクスチャを張りつけたサンプル
	ipu/ezcube3	複数の圧縮データをデコードするサンプル
        ipu/ezmovie     IPU を使用した動画の簡易再生サンプル

kernel:
	kernel/priority		スレッド優先度を理解するためのサンプル
	kernel/rotate		スレッドレディキューの回転のサンプル
	kernel/semaphore	VSync割り込みとセマフォを使ってスレッドの
				動作を制御するサンプル
	kernel/thread		マルチスレッドを用いてオブジェクトの描画を
				行なうサンプル
	kernel/vsync		VSyncStart割り込みハンドラのサンプル

mc:
	mc/basic	メモリーカードライブラリの基本機能確認サンプル
	mc/icon		3D アイコン生成のための Linux 上で動くツール

mcx:
	mcx/basic	PDAライブラリの基本機能確認サンプル

mpeg:
	mpeg/ezmpeg	IPU を使用した MPEG2(MPEG1を含む)ストリーム
			再生サンプル
	mpeg/mpegstr	ホスト側ハードディスク及びCD/DVDから"PlayStation"
			用音声付きMPEG-2 ファイル(PSS ファイル)を再生する
			サンプル（libsdrを使用したサンプル）
	mpeg/mpegstr/OLD
			ホスト側ハードディスク及びCD/DVDから"PlayStation"
			用音声付きMPEG-2 ファイル(PSS ファイル)を再生する
			サンプル（librspu2を使用したサンプル）
        mpeg/mpegvu1	IPU ではなく VU1 を使用して、色変換を実行する
                        サンプル（libsdrを使用したサンプル）
        mpeg/mpegvu1/OLD
			IPU ではなく VU1 を使用して、色変換を実行する
                        サンプル（librspu2を使用したサンプル）

pad:
	pad/basic 	コントローラ情報を表示するサンプル
        pad/dual2       アナログコントローラ（DUALSHOCK 2）の感圧情報を
			表示するサンプル
        pad/gun		ガンコントローラの座標情報を取得し画面上に表示する
			サンプル
        pad/mtap	マルチタップ経由でコントローラを制御するサンプル

pc:
	pc/dcache	Performance Counter を用いて D Cache miss を
			カウントするサンプル

pkt:
        pkt/balls       スプライト描画サンプル
        pkt/cube        ポリゴンモデル描画サンプル
	pkt/mfifo	MFIFOを用いてオブジェクトの描画を行うサンプル

scf:
	scf		"PlayStation 2" 上のシステム設定に設定されている
			情報を取得するサンプル

sif:
	sif/sifcmd	SIF CMD プロトコルのサンプル
	sif/sifrpc	SIF RPC プロトコルのサンプル

sound:
	sound/ezadpcm	ディスクからの ADPCM データストリーミングにより 
			BGM 再生するサンプル
	sound/ezbgm	ディスクから音声ファイル（WAV）をストリーミング
			再生するサンプル
	sound/ezmidi	MIDI による楽曲と効果音の演奏サンプル
	sound/rblock	入出力ブロック転送（旧称AutoDMA転送）サンプル
	sound/sqsoft	ソフトシンセサイザによるMIDI再生サンプル

OLD/spu2:
	（librspu2を使用したサンプル）
	spu2/rautodma	AutoDMA 転送機能を使って、ストレートPCM 入力に
			よる発音を行なうサンプル
	spu2/rseqplay	MIDIシーケンス再生を行なうサンプル
	spu2/rstream	ボイスによるストリーム再生を行なうサンプル
	spu2/rvoice	ボイスの発音を行なうサンプル

usb:
	usb/usbkeybd	EE側からIOP側のUSBキーボードドライバの情報を取得
			するサンプル
	usb/usbmouse	EE側からIOP側のUSBマウスドライバの情報を取得する
			サンプル

vu0:
        VU0を使用してジオメトリ処理を行なうサンプル
        vu0/tballs      ３次元の球体が動作するサンプル
        vu0/tdiff       ポリゴンのクリップを行なうサンプル
        vu0/tfog        方向なし点光源とオブジェクトクリップを行なう
                        サンプル
        vu0/tmip        MiMeの線形内挿とMIPMODELを行なうサンプル
        vu0/tskin       １つのモデルをマトリクスを使用して複数表示する
                        サンプル
        vu0/tsquare     インラインアセンブラを使用したVU0マクロのサンプル

vu1:
        VU1を使用してジオメトリ処理を行なうサンプル
        マイクロコードの記述にはdvpasm形式を使用
        vu1/blow        パーティクル描画サンプル
        vu1/bspline     VU1 B-Spline曲面描画サンプル
        vu1/deform      ２形状補間サンプル
        vu1/hako        VU1透視変換サンプル
        vu1/iga         VU1透視変換、光源計算サンプル

