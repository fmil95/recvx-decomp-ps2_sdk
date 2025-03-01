#
#  IOP module sample program Makefile
#
#  $Id: Makefile,v 1.4 2000/08/02 14:54:55 tei Exp $
#

#----------- customize section --------------
CFLAGS   =  -Wall
ASFLAGS  = 
LOADLIBES=
PROGRAMS = mylib.irx client.irx 

all: $(PROGRAMS) dumpcheck

clean:
	rm -f *.o *.irx *.ilb *.map

run: $(PROGRAMS) 
	dsreset 0 2; dsistart mylib.irx; dsistart client.irx

# 常駐ライブラリのコンパイル ===============================================
mylib.irx : mylib.o mylib.tbl
#	  ライブラリエントリテーブルの作成
	$(ILBGEN) mylib.tbl -e mylibent.s
	$(AS) -o mylibent.o mylibent.s
	rm -f mylibent.s
#	   常駐ライブラリの本体のコンパイルとエントリテーブルとのリンク
# 	   -e オプションで初期化エントリを指定してください
	$(LINK.o) -e MyLibInit mylib.o mylibent.o  \
		$(LOADLIBES) $(LDLIBS) -o $@

# ライブラリを利用するプログラムのリンク時に必要な mylib.ilb ファイルの作成
mylib.ilb: mylib.tbl
	$(ILBGEN) mylib.tbl -d mylib.ilb

# ライブラリを利用するプログラムのコンパイル ===============================
client.irx : client.o mylib.ilb
	$(LINK.o) -startfiles client.o $(LOADLIBES) $(LDLIBS) -o $@ \
		-ilb=mylib.ilb

# 参考 ===============================
#	  プログラムがどの常駐ライブラリを呼び出しているかは
#	  ioplibdumpユーティリティで確認できます。
#	  さらに、どの関数を呼び出しているかも ilb ファイルを
#	  指定することで確認できます
dumpcheck:
	$(ILBDUMP) client.irx 
	$(ILBDUMP) client.irx : mylib.ilb

#----------- rules --------------
%.irx: %.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

-include PathDefs
PathDefs:
	iop-path-setup > PathDefs || (rm -f PathDefs ; exit 1)

