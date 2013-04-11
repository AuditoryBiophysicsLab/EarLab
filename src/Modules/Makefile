MODULES:=$(wildcard *)

BADMODULES = \
	AudioOutput				\
	TestMod					\
	MiddleEarBilinear			\
	MiddleEarStapedius

SHELL     = /bin/sh
MAKESHELL = /bin/sh

all: 
	for i in $(MODULES); do							\
		if [ -d $$i ] ; then						\
			if [ -f $$i/ModuleSpecific.h ] ; then			\
				(cd $$i; make all);				\
			fi;							\
		fi;								\
	done

deletes:
	rm -rf $(BADMODULES)

update: svnupdate deletes

svnupdate:
	svn update
	rm -rf $(BADMODULES)
	

makefiles: 
	for i in $(MODULES); do							\
		if [ -d $$i ] ; then						\
			if [ -f $$i/ModuleSpecific.h ] ; then			\
				cp Makefile.ModuleGeneric $$i;			\
				(cd $$i; make -f Makefile.ModuleGeneric makefile);\
			fi;							\
		fi;								\
	done

longtoint:
	for i in $(MODULES); do							\
		if [ -f $$i/ModuleSpecific.h ] ; then				\
			(cd $$i; make longtoint);				\
		fi;								\
	done


clean:
	-for i in $(MODULES); do						\
		if [ -f $$i/Makefile ] ; then					\
			(cd $$i; make clean) ;					\
		fi;								\
	done


