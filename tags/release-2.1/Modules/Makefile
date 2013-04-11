MODULES=AudioMixer AuditoryNerveIFR AuditoryNerveSpike BasilarMembraneLinearGammatone \
		BasilarMembraneNonlinearGammatone BrugheraMSO

all: AudioMixer AuditoryNerveIFR

AudioMixer:
	cd AudioMixer && $(MAKE) -f ../Makefile.module MODULE=AudioMixer

AuditoryNerveIFR: AuditoryNerveIFR/AuditoryNerveIFR.o
	cd AuditoryNerveIFR && $(MAKE) -f ../Makefile.module MODULE=AuditoryNerveIFR
