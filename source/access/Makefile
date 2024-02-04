CC = cc
CFLAGS = -O
AR = ar
ARFLAGS = ruv

LIB = ../../lib

OBJ1 =  opencatalog.o closecatalog.o catalog_desc.o inpcloser.o \
	closer.o noclose.o openr.o accbuf.o acc_addbuf.o batch.o \
	batchxtra.o writebatch.o getequal.o get.o insert.o \
	replace.o delete.o setkey.o

OBJ2 =  find.o rhash.o ndxsearch.o findbest.o scan_dups.o get_tuple.o \
	put_tuple.o del_tuple.o tup_len.o newlino.o add_ovflo.o \
	formatpg.o last_page.o page.o compare.o add_prim.o utility.o \
	cleanrel.o readadmin.o

access: $(OBJ1)
	$(AR) vru access $(OBJ1)
	$(AR) vru access $(OBJ2)

access: $(OBJ2)
	$(AR) vru access $(OBJ1)
	$(AR) vru access $(OBJ2)

install: access
	cp access $(LIB)/access

clean:
	-rm -f access *.o
