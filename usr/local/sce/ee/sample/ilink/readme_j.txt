[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

i.LINK(IEEE1394)で通信を行なうサンプル

<サンプルの解説>

	このプログラムは、SCE独自のプロトコルであるi.LINKソケットを使い
	自動生成されたデータを２台の"PlayStation 2"が相互に送受信し、受信
	結果の正否をモニタに表示します。

	このサンプルはIOP側のサンプルと対応しており、両方そろって機能します。
        EE側のプログラムが自動的にIOP側のモジュールをロードしますので、
        前もってIOP側のサンプル（iop/sample/ilink)をビルドしてください。

	・接続までの手順
	最初は相手となる"PlayStation 2"のGUID(1394 Global Unique ID)が不明
	なのでブロードキャストで送信します。
	同時に受信スレッドはソケットを監視し、受信されたパケットの送信元の
	GUIDを調べ、それが自分以外のノードからのものであれば、そのGUIDを
	通信相手として登録し、connectします。
	これ以降はconnectしたノードに向かってのみ、パケットを送信します。
	尚、このサンプルでは通信中にi.LINKが切断されることは想定していません。

	・表示
	受信データに付加したチェックサムの正否を表示します。
	受信は非同期なので、受信すべきデータがないときは空行が表示され、
	余った場合はFIFOに入れられ、古いものから順に取り出されます。

	・サンプルの補足説明
	eemain.cの先頭の「#define SEND_NO_WAIT」を有効にすると非同期送信、
	無効にすると同期送信となります。
	同期・非同期の違いはsocketee.cでRPCを行うときにSIF_RPCM_NOWAITと
	するか、しないかの違いだけです。
	受信は常に非同期受信です。

	・i.LINKソケットドライバについて
	i.LINKソケットを使ったブロードキャストはIOPへの負荷が大きいうえに
	1394バスに存在するノード数に比例して増加するので、どうしても必要な
	場合にだけご利用ください。

	i.LINKソケットを使ったとき、１パケットで送受信できる最大ペイロード
	サイズ（ユーザーが自由に使えるサイズ）は492バイトです。
	因みにサンプルでは１Ｖに７パケットを送受信しているので、データ転送
	レートは492×7×60=206640バイト／秒となります。

	sceILsockRecvなどを使ってi.LINKソケットドライバから受信パケットを
	取り出す速さよりも、パケットが到着する方が速いとドライバ内部にある
	FIFOが溢れます。このFIFOの大きさはデフォルトで７パケットです。
	これを超えたパケットはFIFOに格納されず、廃棄されます。

<ファイル>
	eemain.c        メインプログラム
	socketee.c      RPC client

<起動方法>
	２台の"PlayStation 2"をS400対応のi.LINKケーブルで接続します。
	（ソニー　ＶＭＣ－ＩＬ４４１５、４４３５など）

	% make		：IOP 側のコンパイル（iop/sample/ilink)

	EE側のサンプルディレクトリに移動（ee/sample/ilink)
	% make		：EE 側のコンパイル

	コンパイル後、各々の"PlayStation 2" で dsedb を起動します。

	% dsedb
	> run eemain.elf

	を実行します。

<コントローラの操作方法>
	なし

＜画面表示の説明＞
	NodeID	：自機のノードID
	Src	：自機のGUID (64bit)
	Dest	：敵機のGUID (64bit)
	Status	：自機のリンクの状態
			Single:  未接続
			Connect: 接続
	Elapsed	：実行開始後の経過時間（時：分：秒：フレーム）
	nRemHDPkt：half done packet(*)をアプリのFIFOから削除した回数
		（*：１Ｖに複数のパケットを受信しているとき、受信パケット
		　の連続性に異常を検出したら、それらのパケットをFIFOから
		　削除しています）
	nRetryCnt：同期送信時に何らかのエラーのために再送信した回数
		　非同期送信の場合には表示されません。
	TxFrm	：送信フレーム番号（秒：1/60秒）
	nRx	：受信FIFOに残っているパケット数
	RxFrm	：受信フレーム番号（秒：1/60秒）
	SumN...	：第Ｎパケットのチェックサムの結果

<備考>
	なし
