
MODULES = \
	AudioMixer                              \
	AuditoryNerveIFR                        \
	AuditoryNerveSpike                      \
	BasilarMembraneLinearGammatone          \
	BasilarMembraneNonlinearGammatone       \
	BrugheraMSO                             \
	BushyCell                               \
	CaiIC                                   \
	Combiner                                \
	CompoundActionPotentialIFR              \
	CurrentSpread                           \
	DataSource                              \
	Fanout                                  \
	InnerHaircell                           \
	IntegrateAndFire                        \
	ITDAzimuth                              \
	MiddleEarSimpleFilter                   \
	MiddleEarBilinear                       \
	MSOCombiner                             \
	MSOQuarterWave                          \
	MSOXcorr                                \
	Periodicity                             \
	SimpleSignalGenerator                   \
	SimpleSpikeGenerator                    \
	Splitter                                \
	StapediusReflexDriver                   \
	StatisticalAnalysis                     \
	TestMod


BAD  = \
	MiddleEarStapedius \
	MiddleEarBilinear \

IGNORED = \
	DLLSupport      \
	DataSink        \
	DataSink        \

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
		cp ./Src/Shared/Makefile.generic $$i; \
		(cd $$i; make -f Makefile.generic arch); \
	done
	make makefiles

cleanarch:
	-rm -rf $(MODULES)

makefiles:
	-for i in $(MODULES); do                              \
		if [ -f $$i/Makefile ] ; then                 \
		   mv $$i/Makefile $$i/Makefile.bak ;         \
		fi;                                           \
		cp Src/Shared/Makefile.generic $$i/Makefile ; \
		rm -f $$i/Makefile.generic                  ; \
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

