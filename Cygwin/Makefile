ARCH = Cygwin
COMPONENTS = SupportClasses Modules Transport

MAKESHELL = /bin/sh
SHELL     = /bin/sh

all: arch
	for i in ${COMPONENTS} ; do \
		echo Bulding $$i  ; \
		(cd $$i; make all); \
	done

arch:
	if [ ! -e Src ] ; then   \
		ln -s ../Src . ; \
	fi;
	for i in ${COMPONENTS} ; do    \
		if [ ! -d $$i ] ; then \
			mkdir $$i ;    \
			ln -s ../Src/$$i $$i/Src ; \
			cat $$i/Src/Makefile.$(ARCH) $$i/Src/Makefile.common >$$i/Makefile ; \
			(cd $$i; make arch) ; \
		fi;                    \
	done


clean:
	for i in ${COMPONENTS} ; do \
		echo Cleaning $$i  ; \
		(cd $$i; make clean); \
	done

cleanarch:
	rm -rf ${COMPONENTS}
	rm -f bin/* modbin/* lib/* Run/*.out Src

build:
	tar cvhf ${ARCH}-build-`date +%d%b%g`.tar Run bin modbin lib quickstart
