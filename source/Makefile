INGRES=..
BIN=$(INGRES)/bin
DEMO=$(INGRES)/demo
LIB=$(INGRES)/lib
DESTDIR=/usr
DESTBIN=$(DESTDIR)/bin
DESTLIB=$(DESTDIR)/lib
INSTALL=install
INSTALLQ=installq
SYSALL = creatdb ingres destroydb sysmod purge printr helpr restore equel
MAKE=	make
CFLAGS = -O -wo -DMINIX

install:
	cd gutil; $(MAKE) $(INSTALL) CFLAGS="$(CFLAGS)"
	cd iutil; $(MAKE) $(INSTALL) CFLAGS="$(CFLAGS)"
	cd access; $(MAKE) $(INSTALL) CFLAGS="$(CFLAGS)"
	cd dbu; $(MAKE) $(INSTALL) CFLAGS="$(CFLAGS)"
	cd support; $(MAKE) $(INSTALL) CFLAGS="$(CFLAGS)"
	cd monitor; $(MAKE) $(INSTALL) CFLAGS="$(CFLAGS)"
	cd scanner; $(MAKE) $(INSTALL) CFLAGS="$(CFLAGS)"
	cd yyyacc; $(MAKE) $(INSTALL) CFLAGS="$(CFLAGS)"
	cd parser; $(MAKE) $(INSTALL) CFLAGS="$(CFLAGS)"
	cd qrymod; $(MAKE) $(INSTALL) CFLAGS="$(CFLAGS)"
	cd ovqp; $(MAKE) $(INSTALL) CFLAGS="$(CFLAGS)"
	cd decomp; $(MAKE) $(INSTALL) CFLAGS="$(CFLAGS)"
	cd equel; $(MAKE) $(INSTALL) CFLAGS="$(CFLAGS)"
	cd equel/runtime; $(MAKE) $(INSTALL) CFLAGS="$(CFLAGS)"
	cd support; $(MAKE) $(INSTALLQ) CFLAGS="$(CFLAGS)"

sysinstall:
	@echo "I hope you're doing this as root!"
	@sleep 5
	install -c -s -o ingres -g system -m 4751 $(BIN)/creatdb $(DESTBIN)/creatdb
	install -c -s -o ingres -g system -m 4751 $(BIN)/ingres $(DESTBIN)/ingres
	install -c -s -o ingres -g system -m 4751 $(BIN)/destroydb $(DESTBIN)/destroydb
	install -c -s -o ingres -g system -m 4751 $(BIN)/sysmod $(DESTBIN)/sysmod
	install -c -s -o ingres -g system -m 4751 $(BIN)/purge $(DESTBIN)/purge
	install -c -s -o ingres -g system -m 4751 $(BIN)/printr $(DESTBIN)/printr
	install -c -s -o ingres -g system -m 4751 $(BIN)/helpr $(DESTBIN)/helpr
	install -c -s -o ingres -g system -m 4751 $(BIN)/restore $(DESTBIN)/restore
	install -c -s -o bin -g system -m 751 $(BIN)/equel $(DESTBIN)/equel
	install -c -s -o bin -g system -m 751 $(BIN)/copydb $(DESTBIN)/copydb
	install -c -o bin -g system -m 755 $(DEMO)/demodb $(DESTBIN)/demodb
	install -c -o bin -g system -m 644 $(LIB)/libq.a $(DESTLIB)/libq.a

clean:
	cd gutil; $(MAKE) clean
	cd iutil; $(MAKE) clean
	cd access; $(MAKE) clean
	cd dbu; $(MAKE) clean
	cd support; $(MAKE) clean
	cd monitor; $(MAKE) clean
	cd scanner; $(MAKE) clean
	cd yyyacc; $(MAKE) clean
	cd parser; $(MAKE) clean
	cd qrymod; $(MAKE) clean
	cd ovqp; $(MAKE) clean
	cd decomp; $(MAKE) clean
	cd equel; $(MAKE) clean
	cd equel/runtime; $(MAKE) clean
