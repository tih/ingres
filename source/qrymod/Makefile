LIB=../../lib
BIN=../../bin
CC=cc
CFLAGS=-O
LDFLAGS= -i
AR=ar
LIBS=-L. -lqm -L$(LIB) -lparser -laccess -liutil -lgutil

OBJS=	readtree.o qrymod.o issue.o define.o d_view.o d_integ.o \
	d_prot.o fillprotup.o view.o integrity.o protect.o writetree.o \
	range.o tree.o trscan.o treepr.o util.o catalog_desc.o

qrymod: main.o libqm.a
	$(CC) $(LDFLAGS) -o qrymod main.o -L. -lqm ../parser/norml.o -L$(LIB) -laccess -liutil -lgutil
#	$(CC) $(LDFLAGS) -o qrymod main.o $(LIBS)

libqm.a: $(OBJS)
	-$(AR) crv libqm.a $(OBJS)
	ranlib libqm.a

install: libqm.a qrymod
	install -c -m 644 libqm.a $(LIB)
	ranlib $(LIB)/libqm.a
	install -c -m 700 qrymod $(BIN)/qrymod

clean: 
	rm -f qrymod libqm.a *.o *.core a.out
