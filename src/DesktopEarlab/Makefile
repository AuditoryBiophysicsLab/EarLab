OUTDIR   = ../../bin
TARGET   = $(OUTDIR)/earlab
SOURCES  = \
	ConsoleEarlab.cpp		\
	DesktopEarlabDLL.cpp		\
	EarlabModuleInstance.cpp	\
	Model.cpp
HEADERS  = \
	DesktopEarlabDLL.h		\
	EarlabModuleInstance.h		\
	Model.h
OBJECTS  = \
	ConsoleEarlab.o			\
	DesktopEarlabDLL.o		\
	EarlabModuleInstance.o		\
	Model.o
INCLUDE  = ../SupportClasses
LIBDIR   = ../../lib
LIBRARY	 = libearlab.a
CPP      = g++
CPPFLAGS = -g -I. -I$(INCLUDE) -Dstricmp=strcasecmp -DNOTWINDOWS -fPIC -O3
# CPPFLAGS = -g -I. -I$(INCLUDE) -Dstricmp=strcasecmp -DNOTWINDOWS -fPIC -O0

all:	$(TARGET) Makefile

clean:
	rm -f *.o $(TARGET)

$(TARGET): $(OBJECTS) Makefile $(LIBDIR)/$(LIBRARY)
	g++ $(CPPFLAGS) -g -o $(TARGET) $(OBJECTS) -L$(LIBDIR) -learlab -ldl -lpthread

