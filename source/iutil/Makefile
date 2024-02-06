INGRES=	../..
CC=	cc
CFLAGS=	-O
AR=	ar
ARFLAGS=rvc
LIB=	$(INGRES)/lib

OBJS=	printup.o resyncpipes.o rdpipe.o proc_error.o copypipes.o \
	wrpipe.o prpipe.o getuser.o getnuser.o initucode.o \
	initproc.o out_arg.o dblocks.o errfilen.o error.o \
	end_job.o ingresname.o printdesc.o paramd.o typeconv.o \
	typeunconv.o rub.o cslocks.o rllocks.o pglocks.o \
	clr_tuple.o trim_relname.o trbuild.o globals.o \
	version.o reset.o gamma.o

libiutil.a:	$(OBJS)
	-$(AR) $(ARFLAGS) libiutil.a $(OBJS)
	ranlib libiutil.a

install: libiutil.a
	install -c -m 644 libiutil.a $(LIB)
	ranlib $(LIB)/libiutil.a

clean:
	rm -f libiutil.a *.o *.core a.out
