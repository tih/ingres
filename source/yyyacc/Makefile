CC = cc
LIBS =
OBJS = yyy0.o yyy1.o yyy2.o yyy3.o y4.o y5.o

BIN = ../../bin

yyyacc: $(OBJS)
	$(CC) $(OBJS) -o yyyacc $(LIBS) -lS

install: yyyacc
	cp yyyacc $(BIN)/yyyacc

clean:
	-rm -f *.o
	-rm -f yyyacc
