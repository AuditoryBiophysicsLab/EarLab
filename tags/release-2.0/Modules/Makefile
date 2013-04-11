
MODULES = \
	AudioMixer \
	AuditoryNerveIFR \
	BasilarMembraneLinearGammatone \
	BasilarMembraneNonlinearGammatone \
	BushyCell \
	CompoundActionPotentialIFR \
	DataSource \
	Fanout \
	InnerHaircell \
	ITDAzimuth \
	MiddleEarSimpleFilter \
	MSOXcorr \
	MSOCombiner \
	MSOQuarterWave \
	Periodicity \
	SimpleSignalGenerator \
	SimpleSpikeGenerator \

BAD  = \
	BrugheraMSO \
	MiddleEarStapedius \
	MiddleEarBilinear \

IGNORED = \
	DLLSupport \
	DataSink \
	Foobar                  \
	DataSink                \

SHELL     = /bin/sh
MAKESHELL = /bin/sh

all:
	-for i in $(MODULES); do              \
		if [ -f $$i/Makefile ] ; then \
		   (cd $$i; make module) ;    \
		fi;                           \
	done

arch:   cleanarch
	if [ ! -e Src ] ; then                  \
		ln -s ../../Src/Modules ./Src ; \
	fi
	if [ ! -e Shared ] ; then               \
		ln -s Src/Shared . ; \
	fi
	-for i in $(MODULES); do              \
		if [ ! -f $$i ]; then         \
		    mkdir $$i;                \
		fi;                           \
		cp ./Src/Shared/Makefile.Generic $$i; \
		(cd $$i; make -f Makefile.Generic arch); \
	done
	make makefiles

cleanarch:
	-rm -rf $(MODULES)

makefiles:
	-for i in $(MODULES); do                              \
		if [ -f $$i/Makefile ] ; then                 \
		   mv $$i/Makefile $$i/Makefile.bak ;         \
		fi;                                           \
		cp Src/Shared/Makefile.Generic $$i/Makefile ; \
		rm -f $$i/Makefile.Generic                  ; \
		cd $$i;        \
		make Makefile; \
		cd ..;         \
	done

clean:
	-for i in $(MODULES); do              \
		if [ -f $$i/Makefile ] ; then \
		   (cd $$i; make clean) ; \
		fi;                           \
	done

virgin:
	-for i in $(MODULES); do              \
		if [ -f $$i/Makefile ] ; then \
		   (cd $$i; make clean) ;     \
		fi;                           \
		touch $$i/Makefile.xxx ;      \
		rm $$i/Makefile*       ;      \
	done

