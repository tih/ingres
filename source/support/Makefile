CFLAGS = -O
LDFLAGS =

BIN = ../../bin
LIBS = ../../lib/dbulib ../../lib/access ../../lib/iutil ../../lib/gutil

all:    creatdb destroydb purge helpr ingres printr \
	restore sysmod usersetup timefix drtst

creatdb: creatdb.o error.o
	$(CC) $(LDFLAGS) -o creatdb creatdb.o error.o $(LIBS)

destroydb: destroydb.o
	$(CC) $(LDFLAGS) -o destroydb destroydb.o $(LIBS)

purge: purge.o subs.o error.o
	$(CC) $(LDFLAGS) -o purge purge.o subs.o error.o $(LIBS)

helpr: helpr.o error.o
	$(CC) $(LDFLAGS) -o helpr helpr.o error.o $(LIBS)

ingres: ingres.o
	$(CC) $(LDFLAGS) -o ingres ingres.o $(LIBS)

printr: printr.o
	$(CC) $(LDFLAGS) -o printr printr.o $(LIBS) ../../lib/iutil

restore: restore.o error.o subs.o
	$(CC) $(LDFLAGS) -o restore restore.o error.o subs.o $(LIBS)

sysmod: sysmod.o error.o
	$(CC) $(LDFLAGS) -o sysmod sysmod.o error.o $(LIBS)

usersetup: usersetup.o
	$(CC) $(LDFLAGS) -o usersetup usersetup.o $(LIBS)

drtst: drtst.o
	$(CC) $(LDFLAGS) -o drtst drtst.o $(LIBS)

timefix: timefix.o
	$(CC) $(LDFLAGS) -o timefix timefix.o $(LIBS)

install: all
	cp usersetup $(BIN)/usersetup
	cp purge $(BIN)/purge
	cp creatdb $(BIN)/creatdb
	cp destroydb $(BIN)/destroydb
	cp helpr $(BIN)/helpr
	cp ingres $(BIN)/ingres
	cp printr $(BIN)/printr
	cp restore $(BIN)/restore
	cp sysmod $(BIN)/sysmod
	cp demodb.sh $(BIN)/demodb
	chmod 700 $(BIN)/usersetup
	chdir $(BIN); chmod 4711 purge creatdb destroydb helpr
	chdir $(BIN); chmod 4711 ingres printr restore sysmod
clean:
	-rm -f *.o
	-rm -f usersetup timefix
	-rm -f purge creatdb destroydb helpr
	-rm -f ingres printr restore sysmod
