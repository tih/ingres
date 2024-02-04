CC = cc
CFLAGS = -O
AR = ar
ARFLAGS = ruv

LIB = ../../lib

OBJS =  parser.o yylex.o inout.o name.o number.o operator.o \
	string.o symtab.o comment.o cmap.o

scanner: $(OBJS)
	$(AR) $(ARFLAGS) scanner $(OBJS)

install: scanner
	cp scanner $(LIB)/scanner

clean:
	-rm -f *.o
	-rm -f scanner
