INGRES=	../..
CC=	cc
CFLAGS=	-O
LDFLAGS=-i
AR=	ar
ARFLAGS=rvc
BIN=	$(INGRES)/bin
LIB=	$(INGRES)/lib

LIBS=	-L$(LIB) -laccess -liutil -lgutil

OBJS=	ovqp.o startovqp.o getqry.o rdsym.o strategy.o findsimps.o \
	key.o scan.o interp.o interp1.o equelpr.o string.o typecheck.o \
	support.o prsym.o copyreturn.o

all: libovqp.a ovqp

ovqp: main.o $(OBJS)
	$(CC) $(LDFLAGS) -o ovqp main.o $(OBJS) $(LIBS)

libovqp.a: $(OBJS)
	-$(AR) $(ARFLAGS) libovqp.a $(OBJS)
	ranlib libovqp.a

install: libovqp.a ovqp
	install -c -m 644 libovqp.a $(LIB)
	ranlib $(LIB)/libovqp.a
	install -c -m 700 ovqp $(BIN)/ovqp

clean:
	rm -f libovqp.a ovqp *.o *.core a.out
