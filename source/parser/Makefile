CC = cc
CFLAGS = -O
LDFLAGS =
AR= ar
ARFLAGS = ruv
YACC = ../../bin/yyyacc
YFLAGS = -s

LIB = ../../lib
BIN = ../../bin
LIBS = ../../lib/scanner ../../lib/access ../../lib/gutil ../../lib/iutil

OBJS =  attfcn.o xdot.o main.o rngfcn.o tree.o norml.o call_p.o \
	call_tree.o control.o format.o p_util.o getscr.o

parser: y.tab.o parlib
	$(CC) $(LDFLAGS) -o parser y.tab.o parlib $(LIBS) -lf -lS

parlib: $(OBJS)
	$(AR) $(ARFLAGS) parlib $(OBJS)

y.tab.c: grammar.y yyerror.y tables.y
	$(YACC) $(YFLAGS) grammar.y

grammar.y: master.grammar
	grep -v "DDD" master.grammar > grammar.y

install: parlib parser
	cp parlib $(LIB)/parlib
	cp parser $(BIN)/parser
	chmod 4700 $(BIN)/parser

clean:
	-rm -f *.o y.tab.c
	-rm -f parlib parser
