CC = cc
CFLAGS = -O

BIN = ../../bin

LIBS = ../../lib/iutil ../../lib/gutil

OBJS =  branch.o buf.o clear.o clrline.o edit.o eval.o expr.o \
	getch.o getfilename.o go.o include.o mac.o main.o mcall.o \
	monitor.o newdirec.o print.o prompt.o quit.o shell.o \
	trapquery.o writeout.o xwait.o

monitor: $(OBJS)
	$(CC) $(CFLAGS) -o monitor $(OBJS) $(LIBS)

install: monitor
	cp monitor $(BIN)/monitor

clean:
	-rm -f *.o
	-rm -f monitor
