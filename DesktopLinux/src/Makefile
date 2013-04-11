COMPONENTS = \
	SupportClasses	\
	DesktopEarlab	\
	Modules
SHELL     = /bin/sh
MAKESHELL = /bin/sh

all:
	for i in $(COMPONENTS); do		\
		if [ -f $$i/Makefile ] ; then	\
			(cd $$i; make all);	\
		fi;				\
	done

fromsvn: supportclasses earlab modules

supportclasses:
	rm -rf SupportClasses
	svn co https://daffie.bu.edu/svn/earlab/src/SupportClasses
	(cd SupportClasses; make deletes; make all)

modules:
	rm -rf Modules
	svn co https://daffie.bu.edu/svn/earlab/src/Modules
	(cd Modules; make deletes; make makefiles; make all)

earlab:
	svn co https://daffie.bu.edu/svn/earlab/src/DesktopEarlab
	svn export --force https://daffie.bu.edu/svn/earlab/src/DesktopEarlabDLL DesktopEarlab
	(cd DesktopEarlab; make clean; make all)

clean:
	-for i in $(COMPONENTS); do		\
		if [ -f $$i/Makefile ] ; then	\
			(cd $$i; make clean) ;	\
		fi;				\
	done


