NAME := $(shell dsvt-component-name)

#edit this line
BINNAME := $(NAME)

TOPDIR := $(shell dsvt-topdir)
BUILDDIR := $(TOPDIR)/build/$(NAME)
$(shell mkdir -p $(BUILDDIR))
VPATH = $(BUILDDIR):$(TOPDIR)/bin
SRC = $(wildcard *.cxx)
OBJ := $(SRC:%.cxx=%.o)
BUILDOBJ := $(SRC:%.cxx=$(BUILDDIR)/%.o)
BINPATH := $(TOPDIR)/bin/$(BINNAME)
BINS = a.out b.out

.SUFFIXES: .cxx .o .out

CXXFLAGS := $(shell dsvt-cflags) -I../include -Wall -ggdb3

.PHONY: all
all: $(BINS)

#.o: $(basename $(notdir $@)).cxx
$(OBJ): %.o: %.cxx
	@echo CC $(NAME)/$<
	@gcc -c $< $(CXXFLAGS) -o $(BUILDDIR)/$@

$(BINS): %.out: %.o
	@echo LD $@ 
	@g++ $(BUILDDIR)/$< -o $@ `dsvt-libs` $(LDFLAGS)

.PHONY: clean
clean:
	@rm -f $(BUILDOBJ) $(BINPATH)


