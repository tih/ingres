INGRES=	../..
LIB=	$(INGRES)/lib
CC=	cc
CFLAGS=	-O
AR=	ar
ARFLAGS=rvc


OBJS=	parser.o yylex.o inout.o name.o number.o operator.o string.o symtab.o \
	comment.o cmap.o

libscanner.a: $(OBJS)
	-$(AR) $(ARFLAGS) libscanner.a $(OBJS)
	ranlib libscanner.a

install: libscanner.a
	install -c -m 644 libscanner.a $(LIB)
	ranlib $(LIB)/libscanner.a

clean:
	rm -f libscanner.a *.o *.core a.out
