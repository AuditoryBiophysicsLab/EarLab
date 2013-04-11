# Prototypical system dependent Makefile component
ARCH     = generic

CPP      = g++
CFLAGS   = -I../include -Dstricmp=strcasecmp -DWIN32 -DPARAMETER_SPACE
CPPFLAGS = $(CFLAGS)
# Makefile.common

# Makefile.arch inserted here
# Makefile.files inserted here

SRC  = $(CPPSRC) $(CSRC)
OBJ  = $(CPPOBJ) $(COBJ)

LIB = ../lib/libearlab.a

MAKESHELL = /bin/sh

install: Makefile instlib.a

instlib.a: $(OBJ)
	ar cvr $(LIB) $?
	ranlib $(LIB)

Makefile: Makefile.$(ARCH) Makefile.files Makefile.common
	mv Makefile Makefile.bak
	cat Makefile.$(ARCH) Makefile.files Makefile.common >Makefile

Makefile.common:
	if [ ! -e Makefile.common ]; then \
		ln -s Src/Makefile.common . ; \
	fi

Makefile.$(ARCH):
	if [ ! -e Makefile.$(ARCH) ]; then \
		ln -s Src/Makefile.$(ARCH) . ; \
	fi

Makefile.files:
	dofiles

arch:
	if [ ! -e Src ]; then \
		ln -s ../Src/SupportClasses ./Src ; \
	fi
	if [ ! -e Makefile.common ]; then \
		ln -s Src/Makefile.common . ; \
	fi
	if [ ! -e Makefile.$(ARCH) ]; then \
		ln -s Src/Makefile.$(ARCH) . ; \
	fi
	if [ ! -e dofiles ]; then \
		ln -s dofiles . ; \
	fi
	dofiles
	done

clean:
	rm -f *.o *.a *.exe
