INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644
OUTPUTDIRS = bin lib modules

# Common prefix for installation directories.
# NOTE: This directory must exist when you start the install.
prefix = /usr/local/earlab
exec_prefix = $(prefix)
# Where to put the executable for the command `earlab'.
bindir = $(exec_prefix)/bin
# Where to put the computational modules
moduledir = $(exec_prefix)/modules
# Where to put the sample models
modeldir = $(exec_prefix)/models
myversion := $(shell pwd | sed -e 's,.*/,,g')

all: dirs
	@echo "Making Desktop EarLab"
	@(cd src; make)

fromsvn: dirs
	svn update
	(cd src; make fromsvn)

dirs:
	for i in $(OUTPUTDIRS); do			\
		if [ ! -d $$i ] ; then mkdir $$i; fi;	\
	done

public:
	(cd $(EARLABROOT); rm -rf bin lib modules src)
	cp -r . $(EARLABROOT)
	-@find $(EARLABROOT) -name .svn -exec rm -rf '{}' \; 2>/dev/null

clean:
	(cd src; make clean)
	
install:
	-@rm -rf $(bindir) $(moduledir) $(modeldir)
	-@mkdir $(bindir)
	-@mkdir $(moduledir)
	-@mkdir $(modeldir)
	$(INSTALL_PROGRAM) bin/earlab $(bindir)
	$(INSTALL_PROGRAM) modules/* $(moduledir)
	cp -rp models/* $(modeldir)
	chmod -R 775 $(modeldir)

rpm:
	(cd src; make clean)
	(cd ..; tar zcf ../$(myversion)/$(myversion).tar.gz $(myversion))
	(cd ../../SPECS; rpmbuild -ba $(myversion).spec)


