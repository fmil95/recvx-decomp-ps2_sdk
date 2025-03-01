[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved


アンチエイリアシング効果のサンプルプログラム
（Ｚソート無し）

<サンプルの解説>
	このプログラムでは、AA1 によるアンチエイリアシング効果とその
	使い方を説明します。

	AA1 の効果を最大限に発揮するには、描画するポリゴンを画面奥から
	近くの方向へソートする必要があります。しかしながらソーティング
	は時間がかかり、場合によって現実的な解決ではありません。

	このサンプルは、ソーティングが出来ない場合に、特に破綻が目立つ
	“つぶれた”ポリゴンに関して AA1 を OFF することで実用上十分な
	効果が得られることを示します。

	また、AA1 LINE が Z Buffer に値を書き込まない問題を解決する為
	に １度目に AA1 を off して Z Buffer に描画し、２度目に Z Test
	を行いながら AA1 LINE を描画することで Z Buffer を利用可能です。

	また、もう一つの AA の方法として、1 field 前の frame buffer の
	内容を bilinear filter をかけながら現在の frame buffer に α 
	Blend 描画する機能も実装されています。

<ファイル>
	aa1.c		：メインプログラム
	devinit.c	：デバイスの初期化ルーチン
	make_packet.c	：パケット生成ルーチン
	*.h		：テクスチャデータ等
	work.dsm	：オブジェクトデータ(DMA packet)
	work.vsm	：VU1マイクロプログラム

<起動方法>
	% make		：コンパイル

	コンパイル後、以下の方法で実行可能
	% dsedb
	> run aa1.elf

<コントローラの操作方法>
	↑↓(方向キー）	：視点切り替え
	○ボタン	：オブジェクト数増加
	×ボタン	：オブジェクト数減少
	△ボタン	：ライト減少（暗くなる）
	□ボタン	：ライト増加（明るくなる）

	R1ボタン	：AA1 ON <-> OFF
	R2ボタン	：オブジェクト切り替え triangle <-> line

	L1ボタン	：1 field 前の画像とのブレンドによる AA
	L2ボタン	：1 field 前の画像とのブレンドによる AA（少し明るい）

<備考>
	L1ボタン、L2ボタン の効果は AA1 の機能とは独立です。

	また、AA1 の副作用を取り除くために、視点から見てつぶれた
	ポリゴンおよび裏向きのポリゴンでは AA1 を OFF しています。
	そのためのコードが work.vsm に含まれています。

	計算の概要は以下の通りです。

	1. VF05 に view 座標系での頂点座標を計算する
	2. VF06 に view 座標系でのポリゴンの法線を計算する
	3. 内積 VF05*VF06 を計算し VF29x に保存
	4. 視点から頂点までの距離を VF05z で代用し VF08x に保存
	5. VF29x*8.0f - VF08x を計算
	6. 符号により、PRMODE の AA1 を ON-OFF したコマンドを VF07 に準備
	7. 1頂点を描画する度に PRMODE で AA1 を ON-OFF

	視線ベクトルと面の法線との cos が 1/8 以下の時 AA1 を OFF します。


