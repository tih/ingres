INGRES=../..
BIN=$(INGRES)/bin
LIB=$(INGRES)/lib
LIBS=-liutil -lgutil
YACC=$(BIN)/yyyacc
YFLAGS=
CC=cc
CFLAGS=-O
LDFLAGS=-i
AR=ar

OBJS=	main.o parser.o yylex.o cvar.o include.o name.o getkey.o number.o \
	operator.o prtout.o string.o comment.o retrieve.o display.o getch.o \
	cmap.o yyerror.o nalloc.o

equel: y.tab.o libequel.a
	$(CC) $(LDFLAGS) -o equel y.tab.o -L. -lequel -L$(LIB) $(LIBS)

y.tab.c: grammar.y tokens.y
	$(YACC) $(YFLAGS) grammar.y

libequel.a: $(OBJS)
	-$(AR) crv libequel.a $(OBJS)
	ranlib libequel.a

install: libequel.a equel
	install -c -m 644 libequel.a $(LIB)
	ranlib $(LIB)/libequel.a
	install -c -m 751 equel $(BIN)/equel

clean:
	rm -f equel libequel.a y.tab.c *.o *.core a.out
