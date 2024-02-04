CC = cc
CFLAGS = -O
LDFLAGS =
AR = ar
ARFLAGS = ruv
YACC = ../../bin/yyyacc
YFLAGS =

BIN = ../../bin
LIB = ../../lib

LIBS = ../../lib/iutil ../../lib/gutil

OBJS =  main.o parser.o yylex.o cvar.o include.o name.o getkey.o \
	number.o operator.o prtout.o string.o comment.o retrieve.o \
	display.o getch.o cmap.o yyerror.o nalloc.o

equel: equelib y.tab.o
	$(CC) $(LDFLAGS) -o equel y.tab.o equelib $(LIBS) -lf

y.tab.c: grammar.y
	$(YACC) $(YFLAGS) grammar.y

equelib: $(OBJS)
	$(AR) $(ARFLAGS) equelib $(OBJS)

install: equelib equel
	cp equelib $(LIB)/equelib
	cp equel $(BIN)/equel
	chmod 0755 $(BIN)/equel

clean:
	-rm -f *.o
	-rm -f equelib equel
