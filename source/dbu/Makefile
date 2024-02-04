CC = cc
CFLAGS = -O
LDFLAGS =
AR = ar
ARFLAGS = ruv

LIB = ../../lib
BIN = ../../bin
LIBS = ../../lib/access ../../lib/iutil ../../lib/gutil

OBJS =  index.o create.o modify.o modupdate.o destroy.o userdestroy.o \
	purgetup.o print.o help.o copy.o save.o rupdate.o update.o \
	convert.o secupdate.o rdwrbatch.o resetrel.o get_seq_atts.o \
	dbuproctab.o display.o pr_tree.o readtree.o pr_prot.o rmqm.o \
	get_p_tid.o

all: dbulib overlaya overlayc overlayi overlaym overlayn overlayg ksort

dbulib: $(OBJS)
	$(AR) $(ARFLAGS) dbulib $(OBJS)
	cp dbulib $(LIB)/dbulib

overlaya: main.o dbulib overlaya.o
	$(CC) $(LDFLAGS) -o overlaya main.o overlaya.o dbulib $(LIBS) -lf

overlayc: main.o dbulib overlayc.o
	$(CC) $(LDFLAGS) -o overlayc main.o overlayc.o dbulib $(LIBS) -lf

overlayi: main.o dbulib overlayi.o
	$(CC) $(LDFLAGS) -o overlayi main.o overlayi.o dbulib $(LIBS) -lf

overlaym: main.o dbulib overlaym.o
	$(CC) $(LDFLAGS) -o overlaym main.o overlaym.o dbulib $(LIBS) -lf

overlayn: main.o dbulib overlayn.o
	$(CC) $(LDFLAGS) -o overlayn main.o overlayn.o dbulib $(LIBS) -lf

overlayg: main.o dbulib overlayg.o
	$(CC) $(LDFLAGS) -i -o overlayg main.o overlayg.o dbulib $(LIBS) -lf

ksort: ksort.o
	$(CC) $(LDFLAGS) -n -o ksort ksort.o $(LIBS) -lf

install: all
	cp overlaya $(BIN)/overlaya
	cp overlayc $(BIN)/overlayc
	cp overlayi $(BIN)/overlayi
	cp overlaym $(BIN)/overlaym
	cp overlayn $(BIN)/overlayn
	cp overlayg $(BIN)/overlayg
	cp ksort $(BIN)/ksort
	chmod 4711 $(BIN)/ksort
	chmod 4700 $(BIN)/overlay?

clean:
	-rm -f *.o
	-rm -f dbulib ksort overlay?
