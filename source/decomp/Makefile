CC = cc
CFLAGS = -O
LDFLAGS =
AR = ar
ARFLAGS = ruv

LIB = ../../lib
BIN = ../../bin

LIB40=  ../../lib/dbulib ../../lib/access ../../lib/iutil ../../lib/gutil

LIB70=  ../../lib/ovqplib ../../lib/dbulib ../../lib/access \
	../../lib/iutil ../../lib/gutil

OBJ40=  call_dbu.o call_ovqp.o

OBJ70=  call_dbu70.o call_ovqp70.o

OBJS=   ageval.o aggregate.o byeval.o decision.o decomp.o error.o exec_sq.o \
	lockit.o main.o makenode.o mapvar.o mklist.o openrs.o printree.o \
	prlinks.o pull_const.o pull_sq.o qryproc.o rangetable.o readq.o \
	reduction.o reformat.o reinit.o ret_unique.o rnum.o selectv.o \
	setvar.o tempvar.o usubr.o writeback.o writenod.o

all: decomp decomp70

decomp: $(OBJ40) $(OBJS)
	-rm -f $(OBJ70)
	$(CC) $(LDFLAGS) -o decomp *.o $(LIB40) -lf

decomp70: $(OBJ70) $(OBJS)
	-rm -f $(OBJ40)
	$(CC) $(LDFLAGS) -i -o decomp70 *.o $(LIB70) -lf

install: all
	cp decomp $(BIN)/decomp
	chmod 4700 $(BIN)/decomp
	cp decomp70 $(BIN)/decomp70
	chmod 4700 $(BIN)/decomp70

clean:
	-rm -f *.o
	-rm -f decomp decomp70
