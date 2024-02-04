DESTBIN = /usr/bin
DESTLIB = /usr/lib

INSTALL = install

SUALL = creatdb ingres destroydb sysmod \
	purge printr helpr restore

SYSALL = creatdb ingres destroydb equel sysmod \
	purge printr helpr restore demodb

INGRES = ingres
SUMODE = 4751
SSMODE = 5711

MAKE = make

install:
	chdir gutil; $(MAKE) $(INSTALL)
	chdir iutil; $(MAKE) $(INSTALL)
	chdir access; $(MAKE) $(INSTALL)
	chdir dbu; $(MAKE) $(INSTALL)
	chdir support; $(MAKE) $(INSTALL)
	chdir monitor; $(MAKE) $(INSTALL)
	chdir scanner; $(MAKE) $(INSTALL)
	chdir yyyacc; $(MAKE) $(INSTALL)
	chdir parser; $(MAKE) $(INSTALL)
	chdir qrymod; $(MAKE) $(INSTALL)
	chdir ovqp; $(MAKE) $(INSTALL)
	chdir decomp; $(MAKE) $(INSTALL)
	chdir equel; $(MAKE) $(INSTALL)
	chdir equel/runtime; $(MAKE) $(INSTALL)

sysinstall:
	chdir ../bin; chmod $(SSMODE) ksort
	chdir ../bin; ls $(SYSALL) | cpio -p $(DESTBIN)
	chdir $(DESTBIN); chown $(INGRES) $(SUALL); chmod $(SUMODE) $(SUALL)
	chdir ../lib; cp libq.a $(DESTLIB)
	chdir ../doc; $(MAKE) sysinstall

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
