CC = cc
CFLAGS = -O
AR = ar
ARFLAGS = ruv

BIN = ../../bin
LIB = ../../lib

LIBS = ../../lib/access ../../lib/iutil ../../lib/gutil

OBJS =  ovqp.o startovqp.o getqry.o rdsym.o strategy.o findsimps.o \
	key.o scan.o interp.o interp1.o equelpr.o string.o typecheck.o \
	support.o prsym.o copyreturn.o

all: ovqplib ovqp

ovqplib: $(OBJS)
	$(AR) $(ARFLAGS) ovqplib $(OBJS)

ovqp:   main.o $(OBJS)
	$(CC) $(LDFLAGS) -o ovqp main.o $(OBJS) $(LIBS) -lf

install: all
	cp ovqp $(BIN)/ovqp
	cp ovqplib $(LIB)/ovqplib

clean:
	-rm -f *.o
	-rm -f ovqplib ovqp
