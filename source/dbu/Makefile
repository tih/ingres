LIB=../../lib
BIN=../../bin
LIBS=-L$(LIB) -laccess -liutil -lgutil
CC=cc
CFLAGS=-O
LDFLAGS=-i
AR=ar
ARFLAGS=rvc

OBJS=	index.o create.o modify.o modupdate.o destroy.o userdestroy.o \
	purgetup.o print.o help.o copy.o save.o rupdate.o update.o convert.o \
	secupdate.o rdwrbatch.o resetrel.o get_seq_atts.o dbuproctab.o \
	display.o pr_tree.o readtree.o pr_prot.o rmqm.o get_p_tid.o

all: libdbu.a overlaya overlayc overlayi overlaym overlayn overlayg ksort

overlaya: libdbu.a main.o overlaya.o
	$(CC) $(LDFLAGS) -o overlaya main.o overlaya.o -L. -ldbu $(LIBS)

overlayc: libdbu.a main.o overlayc.o
	$(CC) $(LDFLAGS) -o overlayc main.o overlayc.o -L. -ldbu $(LIBS)

overlayi: libdbu.a main.o overlayi.o
	$(CC) $(LDFLAGS) -o overlayi main.o overlayi.o -L. -ldbu $(LIBS)

overlaym: libdbu.a main.o overlaym.o
	$(CC) $(LDFLAGS) -o overlaym main.o overlaym.o -L. -ldbu $(LIBS)

overlayn: libdbu.a main.o overlayn.o
	$(CC) $(LDFLAGS) -o overlayn main.o overlayn.o -L. -ldbu $(LIBS)

overlayg: libdbu.a main.o overlayg.o
	$(CC) $(LDFLAGS) -o overlayg main.o overlayg.o -L. -ldbu $(LIBS)

libdbu.a: $(OBJS)
	-$(AR) rvc libdbu.a $(OBJS)
	ranlib libdbu.a

ksort: ksort.o
	$(CC) $(LDFLAGS) -o ksort ksort.o $(LIBS)

install: all
	install -c -m 644 libdbu.a $(LIB)
	ranlib $(LIB)/libdbu.a
	install -c -m 700 overlaya $(BIN)/overlaya
	install -c -m 700 overlayc $(BIN)/overlayc
	install -c -m 700 overlayi $(BIN)/overlayi
	install -c -m 700 overlaym $(BIN)/overlaym
	install -c -m 700 overlayn $(BIN)/overlayn
	install -c -m 700 overlayg $(BIN)/overlayg
	install -c -m 700 ksort $(BIN)/ksort

clean:
	rm -f libdbu.a overlay[acimng] ksort *.o *.core a.out
