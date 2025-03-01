[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

フレームバッファの内容をセーブするサンプル

<サンプルの解説>
        このプログラムは、描画されたフレームバッファをホストマシンの
        フレームバッファにセーブするものです。
        描画自体は sample/vu1/deform のサンプルを使用しています。

<ファイル>
        capture.c       ：メインプログラム
        metal.dsm       ：テクスチャデータ
        sphere.dsm      ：オブジェクトデータ(DMA packet)
        mime.vsm        ：VU1マイクロプログラム

<起動方法>
        % make          ：コンパイル
        % make run      ：実行

        コンパイル後、以下の方法でも実行可能
        % dsedb
        > run capture.elf

<コントローラの操作方法>
        ○ボタン        ：変形は続けたまま、回転の停止
        ×ボタン        ：キャプチャしてプログラムを終了

<処理概略>
        キャプチャ処理の概略は、以下の通りです。
        （描画処理自体はdeformのサンプルを参照）

        1. ×ボタンを押す。
        2. アニメーションのためのマトリクス更新を行うことなく
           次のフレームをもう一方のフレームバッファに描画
        3. ローカル関数 StoreImage()を呼び出し
        4. ホストファイルのオープン
        5. odd/evenのフレームバッファの内容を貯えるメモリを確保
        6. evenフィールドの内容をローカル-->ホスト転送する
           (sceGsExecStoreImage())
        7. oddフィールドの内容をローカル-->ホスト転送する
        8. 転送されたイメージをodd/even交互にホスト上のファイルにかき出す
           (sceWrite())
        9. 確保したメモリを開放
        10. ファイル記述子を開放
        11. プログラムを終了する

<備考>
        出力されるファイルは32bit-rgbaのraw dataです。
        
