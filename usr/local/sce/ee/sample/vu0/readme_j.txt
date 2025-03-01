[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

VU0サンプルプログラム

<サンプルの解説>
VU0サンプルプログラムには以下のものがあります。

vu0/tballs ：	３次元の球体が動作するサンプル
vu0/tfog   ：	方向なし点光源とオブジェクトクリップを行なうサンプル
vu0/tmip   ：	MiMeの線形内挿とMIPMODELを行なうサンプル
vu0/tskin  ：	１つのモデルをマトリクスを使用して複数表示するサンプル
vu0/tsquare：	インラインアセンブラを使用したVU0マクロのサンプル
vu0/tdiff  ：	ポリゴンのクリップを行なうサンプル

<ファイル>
vu0/tballs：	object.c
		object.h
		objfast.c
		objfastA.c
		objnorm.c
		objsimpl.c
		sce_pp3.c
		tballs.c
		util.c
vu0/tdiff：
		object.c
		object.h
		objfast.c
		objfastA.c
		objnorm.c
		objpart.c
		objsimpl.c
		sce_pp3.c
		tdiff.c
		util.c

vu0/tfog：
		object.c
		object.h
		objfast.c
		objfastA.c
		objnorm.c
		objpart.c
		objsimpl.c
		sce_pp3.c
		tfog.c
		util.c

vu0/tmip：
		object.c
		object.h
		objfast.c
		objfastA.c
		objnorm.c
		objpart.c
		objsimpl.c
		sce_pp3.c
		tmip.c
		util.c

vu0/tskin：
		object.c
		object.h
		objfast.c
		objfastA.c
		objnorm.c
		objpart.c
		objsimpl.c
		sce_pp3.c
		tskin.c
		util.c

vu0/tsquare：
		object.c
		object.h
		objfast.c
		objfastA.c
		objnorm.c
		objpart.c
		objsimpl.c
		sce_pp3.c
		tsquare.c
		util.c

<起動方法>
	% make		：コンパイル
	% make run	：実行

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run *.elf

<コントローラの操作方法>
	なし

<備考>
	なし

