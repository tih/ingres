NROFF=  nroff $(RFLAGS)
NMACS=  naa
TROFF=  troff $(RFLAGS)
TMACS=  taa
VROFF=  csh /usr/ucb/vtroff $(RFLAGS)
VMACS=  taa

ref:
	-@$(NROFF) $(NMACS) 0aa.nr 1aa.nr toc.nr
	-@chdir quel; $(NROFF) ../$(NMACS) *.nr
	-@chdir unix; $(NROFF) ../$(NMACS) *.nr
	-@chdir files; $(NROFF) ../$(NMACS) *.nr
	-@chdir error; $(NROFF) ../$(NMACS) *.nr

tref:
	-@$(TROFF) $(TMACS) 0aa.nr 1aa.nr toc.nr
	-@chdir quel; $(TROFF) ../$(TMACS) *.nr
	-@chdir unix; $(TROFF) ../$(TMACS) *.nr
	-@chdir files; $(TROFF) ../$(TMACS) *.nr
	-@chdir error; $(TROFF) ../$(TMACS) *.nr

vref:
	-@$(VROFF) $(VMACS) 0aa.nr 1aa.nr toc.nr
	-@chdir quel; $(VROFF) ../$(VMACS) *.nr
	-@chdir unix; $(VROFF) ../$(VMACS) *.nr
	-@chdir files; $(VROFF) ../$(VMACS) *.nr
	-@chdir error; $(VROFF) ../$(VMACS) *.nr

sysinstall:
	cat iaa unix/copydb.nr > /usr/man/man1/copydb.1
	cat iaa unix/creatdb.nr > /usr/man/man1/creatdb.1
	cat iaa unix/destroydb.nr > /usr/man/man1/destroydb.1
	cat iaa unix/equel.nr > /usr/man/man1/equel.1
	cat iaa unix/geoquel.nr > /usr/man/man1/geoquel.1
	cat iaa unix/helpr.nr > /usr/man/man1/helpr.1
	cat iaa unix/ingres.nr > /usr/man/man1/ingres.1
	cat iaa unix/printr.nr > /usr/man/man1/printr.1
	cat iaa unix/purge.nr > /usr/man/man1/purge.1
	cat iaa unix/restore.nr > /usr/man/man1/restore.1
	cat iaa unix/sysmod.nr > /usr/man/man1/sysmod.1
	cat iaa unix/timefix.nr > /usr/man/man1/timefix.1
	cat iaa unix/usersetup.nr > /usr/man/man1/usersetup.1
