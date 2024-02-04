CC = cc
CFLAGS = -O
LDFLAGS =
AR = ar
ARFLAGS = ruv

LIB = ../../lib
BIN = ../../bin

LIBS =  ../../lib/parlib ../../lib/access ../../lib/iutil ../../lib/gutil

OBJS =  readtree.o qrymod.o issue.o define.o d_view.o d_integ.o d_prot.o \
	fillprotup.o view.o integrity.o protect.o writetree.o range.o \
	tree.o trscan.o treepr.o util.o catalog_desc.o

qrymod: main.o qmlib
	$(CC) $(LDFLAGS) -o qrymod main.o qmlib $(LIBS)

qmlib: $(OBJS)
	$(AR) $(ARFLAGS) qmlib $(OBJS)

install: qmlib qrymod
	cp qmlib $(LIB)/qmlib
	cp qrymod $(BIN)/qrymod
	chmod 4700 $(BIN)/qrymod

clean:
	-rm -f *.o
	-rm -f qmlib qrymod
