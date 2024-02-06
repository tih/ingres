INGRES=	../..
CC=	cc
CFLAGS=	-O
LDFLAGS= -i

LIB=	$(INGRES)/lib
BIN =	$(INGRES)/bin
ACCESS=	-laccess
GUTIL=	-lgutil
IUTIL=	-liutil
OVQP=	-lovqp
DBU=	-ldbu

LIB40=	-L$(LIB) $(DBU) $(ACCESS) $(IUTIL) $(GUTIL)

LIB70=	-L$(LIB) $(OVQP) $(DBU) $(ACCESS) $(IUTIL) $(GUTIL)

OBJS=	main.o ageval.o aggregate.o byeval.o \
	decision.o decomp.o error.o exec_sq.o lockit.o \
	makenode.o mapvar.o mklist.o openrs.o printree.o prlinks.o \
	pull_const.o pull_sq.o qryproc.o rangetable.o readq.o reduction.o \
	reformat.o reinit.o ret_unique.o rnum.o selectv.o setvar.o tempvar.o \
	usubr.o writeback.o writenod.o

OBJ40=	call_dbu.o call_ovqp.o $(OBJS)

OBJ70=	call_dbu70.o call_ovqp70.o $(OBJS)

all: decomp decomp70

decomp: $(OBJ40)
	$(CC) $(LDFLAGS) -o decomp $(OBJ40) $(LIB40)

decomp70: $(OBJ70)
	$(CC) $(LDFLAGS) -o decomp70 $(OBJ70) $(LIB70)

install: decomp decomp70
	install -c -m 700 decomp $(BIN)/decomp
	install -c -m 700 decomp70 $(BIN)/decomp70

clean:
	rm -f decomp decomp70 *.o *.core a.out
