OUTDIR   = ../../lib
TARGET   = $(OUTDIR)/libearlab.a
DELETES  = earlibc.cpp

SOURCES:=$(wildcard *.cpp)
OBJECTS:=$(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS:=$(patsubst %.cpp,%.d,$(SOURCES))

include $(DEPENDS)

CPP      = g++
CPPFLAGS = -I. -Dstricmp=strcasecmp -DNOTWINDOWS -DUSE_PTHREADS -fPIC -g -O3

all:	$(DEPENDS) $(TARGET) Makefile

depend: $(DEPENDS)

clean:
	rm -f *.o *.rpo *.d $(OBJDIR)/*.o $(TARGET)

update:
	svn update
	rm $(DELETES)

deletes:
	rm $(DELETES)

%.d: %.cpp
	@echo 'Generating dependency file for $<'
	@$(SHELL) -ec '$(CC) -MM $(CPPFLAGS) $< | sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; [ -s $@ ] || rm -f $@'

$(TARGET): $(OBJECTS)
	ar cvr $(TARGET) $?
	ranlib $(TARGET)
