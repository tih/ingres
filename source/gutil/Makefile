INGRES=	../..
CC=	cc
CFLAGS=	-O
AR=	ar
ARFLAGS=rvc
LIB=	$(INGRES)/lib

OBJS=	atof.o atoi.o atol.o bequal.o bitcnt.o bitpos.o bmove.o cat.o \
	concat.o dysize.o fgetline.o flush.o freebuf.o ftoa.o fullwait.o \
	getufield.o iocv.o itoa.o length.o locv.o longconst.o need.o oatoi.o \
	pad.o pmove.o prargs.o scompare.o sequal.o setflag.o smove.o syserr.o \
	tTf.o trace.o xputchar.o ztack.o

libgutil.a:	$(OBJS)
	-$(AR) $(ARFLAGS) libgutil.a $(OBJS)	
	ranlib libgutil.a

.s.o:
	as -o $@ $<

install: libgutil.a
	install -c -m 644 libgutil.a $(LIB)/libgutil.a
	ranlib $(LIB)/libgutil.a

clean:
	rm -f libgutil.a *.o *.core a.out
