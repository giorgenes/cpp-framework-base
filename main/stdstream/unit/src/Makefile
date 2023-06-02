PROGRAMS := queue-cat file-cat
NAME := $(shell dsvt-component-name)
BINNAME := $(NAME)
TOPDIR := $(shell dsvt-topdir)
BUILDDIR := $(TOPDIR)/build/$(NAME)-unit
$(shell mkdir -p $(BUILDDIR))
VPATH = $(BUILDDIR):$(TOPDIR)/bin
BINPATH := $(TOPDIR)/bin
$(shell mkdir -p $(BINPATH))
CXXFLAGS := $(shell dsvt-cflags) -I../include -Wall -ggdb3
BINS = $(PROGRAMS:%=$(BINPATH)/%)

.SUFFIXES: .cxx .o

.cxx.o:
	@echo CC $<
	@gcc -c $< $(CXXFLAGS) -o $(BUILDDIR)/$@

all: $(PROGRAMS)

$(PROGRAMS): %: %.o
	@echo LD $@
	@gcc -o $(BINPATH)/$@ $(BUILDDIR)/$<

clean:
	@rm -f $(BUILDDIR)/*.o $(BINPATH)/$(PROGRAMS)
