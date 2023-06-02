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

DEPDIR = $(BUILDDIR)
df = $(DEPDIR)/$(*F)
MAKEDEPEND = gcc -M $(CXXFLAGS)
LDFLAGS = -ldl

.SUFFIXES: .cxx .o

CXXFLAGS := $(shell dsvt-cflags) -I../include -Wall -ggdb3

.PHONY: all
all: $(BINNAME)

#.o: $(basename $(notdir $@)).cxx
$(OBJ): %.o: %.cxx
	@echo CC $(NAME)/$<
	@gcc -c $< $(CXXFLAGS) -o $(BUILDDIR)/$@

$(BINPATH): $(OBJ)
	@echo LD $(BINNAME)
	@g++ $(BUILDOBJ) -o $@ `dsvt-libs` $(LDFLAGS)

.PHONY: clean
clean:
	@rm -f $(BUILDOBJ) $(BINPATH)


# DO NOT DELETE
