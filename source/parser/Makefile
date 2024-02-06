CC=cc
LIB=../../lib
BIN=../../bin
LIBS=-L$(LIB) -lscanner -laccess -liutil -lgutil
CFLAGS=-O
LDFLAGS=-i
YACC=$(BIN)/yyyacc
YFLAGS=-s
AR= ar

OBJS=	attfcn.o xdot.o main.o rngfcn.o tree.o norml.o call_p.o call_tree.o \
	control.o format.o p_util.o getscr.o

parser: y.tab.o libparser.a
	$(CC) $(LDFLAGS) -o parser y.tab.o -L. -lparser $(LIBS)

libparser.a: $(OBJS)
	-$(AR) rvc libparser.a $(OBJS)
	ranlib libparser.a

y.tab.c: grammar.y yyerror.y tables.y
	$(YACC) $(YFLAGS) grammar.y

grammar.y: master.grammar
	grep -v "DDD" master.grammar > grammar.y 

install: libparser.a parser
	install -c -m 644 libparser.a $(LIB)
	ranlib $(LIB)/libparser.a
	install -c -m 700 parser $(BIN)/parser

clean:
	rm -f parser libparser.a grammar.y y.tab.c *.o *.core a.out
