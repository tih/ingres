INGRES=	../..
CC=	cc
CFLAGS=	-O
AR=	ar
ARFLAGS=rvu
LIB=	$(INGRES)/lib

OBJS=	printup.o resyncpipes.o rdpipe.o proc_error.o copypipes.o \
	wrpipe.o prpipe.o getuser.o getnuser.o initucode.o \
	initproc.o out_arg.o dblocks.o errfilen.o error.o \
	end_job.o ingresname.o printdesc.o paramd.o typeconv.o \
	typeunconv.o rub.o cslocks.o rllocks.o pglocks.o \
	clr_tuple.o trim_relname.o trbuild.o globals.o \
	version.o reset.o gamma.o

iutil:	$(OBJS)
	-$(AR) $(ARFLAGS) iutil $(OBJS)

install: iutil
	chmod	644	iutil
	cp iutil $(LIB)/iutil
