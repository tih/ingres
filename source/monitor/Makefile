INGRES=	../..
CC=	cc
CFLAGS=	-O
LDFLAGS=-i
AR=	ar
ARFLAGS=rvc
LIB=	$(INGRES)/lib
BIN=	$(INGRES)/bin

LIBS= -liutil -lgutil

OBJS=	branch.o buf.o clear.o clrline.o edit.o eval.o expr.o getch.o \
	getfilename.o go.o include.o mac.o main.o mcall.o monitor.o\
	newdirec.o print.o prompt.o quit.o shell.o trapquery.o writeout.o \
	xwait.o

monitor: $(OBJS)
	$(CC) $(LDFLAGS) -o monitor $(OBJS) -L$(LIB) $(LIBS)

install: monitor
	install -c -m 751 monitor $(BIN)/monitor

clean:
	rm -f monitor *.o *.core a.out
