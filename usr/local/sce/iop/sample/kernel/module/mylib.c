/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *                      Emotion Engine Library
 *                          Version 0.1.0
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         mylib.c
 *                         develop library
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.1.0
 */

#include <stdio.h>
#include <kernel.h>

/* メモリに常駐するモジュールは, 以下のようにモジュール名と
 * モジュールバージョンを付けておくと便利です。
 */
ModuleInfo Module = {"My_Memory_Resident_Lib", 0x0101 };

/* ================================================================
 * 	常駐ライブラリとしての初期化エントリ
 * ================================================================ */

int MyLibInit()
{
    /* mylib.tblから, ユーティリティ loplibgen によってエントリテーブルが
     * 生成されます。エントリテーブルのラベル名には, 'ライブラリ名_entry' が
     *  つけられます。
     */
    extern libhead mylib_entry; /* ライブラリ名_entry を参照 */

    if( RegisterLibraryEntries(&mylib_entry) != 0 ) {
	/* 既に同名の常駐ライブラリがいるので登録に失敗 */
	return NO_RESIDENT_END; /* 終了してメモリから退去 */
    }
    return RESIDENT_END; /* 終了して常駐する */
}

/* ================================================================
 * 	常駐ライブラリの各エントリの定義
 * ================================================================ */

/*
   モジュールは、それぞれ独自の GPレジスタの値を持ちます。
   ところが、あるモジュールから他のモジュールの常駐ライブラリを
   呼び出すと、GPレジスタは呼び出し側モジュールの GP値を保持したまま
   呼び出されます。
   従って常駐ライブラリ内で自モジュール内のグローバルデータを
   アクセスするとき問題が生じます。
   この問題を避けるために、以下のどちらかの方法で常駐ライブラリを
   作成してください。
    1) コンパイラのオプション -G 0 で常駐ライブラリをコンパイルする。
       これによって常駐ライブラリは GP レジスタを使用しないコードになります。
       ただしこの方法は常駐ライブラリが若干大きくなり実行速度が遅くなります。
    2) 常駐ライブラリの各エントリ関数を、下記のように、GPレジスタを待避して
       自モジュールの GP レジスタを設定するようにコーディングする。
      (下記のコーディングは gcc系統のコンパイラを使用した場合の例です。)
 */

void libentry1(int i)
{
    unsigned long oldgp;
    asm volatile( "  move %0, $gp; la  $gp, _gp" : "=r" (oldgp));

    printf("mylib: libentry1(%d)\n", i);

    asm volatile( "  move $gp, %0"  : : "r" (oldgp));
}

void internal_libentry2(int i)
{
    unsigned long oldgp;
    asm volatile( "  move %0, $gp; la  $gp, _gp" : "=r" (oldgp));

    printf("mylib: libentry2(%d)\n", i);

    asm volatile( "  move $gp, %0"  : : "r" (oldgp));
}

