TOPDIR := $(shell dsvt-topdir)
NAME := $(shell dsvt-component-name)
BUILDDIR := $(TOPDIR)/build/$(NAME)
$(shell mkdir -p $(BUILDDIR))
VPATH = $(BUILDDIR)
SRC = $(wildcard *.cxx)
OBJ := $(SRC:%.cxx=%.o)
BUILDOBJ := $(SRC:%.cxx=$(BUILDDIR)/%.o)
LIBPATH := $(BUILDDIR)/libany$(NAME).so

.SUFFIXES: .cxx .o

CXXFLAGS := $(shell dsvt-cflags) -I../include -Wall -ggdb3

.PHONY: all
all: libany$(NAME).so

#.o: $(basename $(notdir $@)).cxx
$(OBJ): %.o: %.cxx
	@echo CC $(NAME)/$<
	@gcc -c $< $(CXXFLAGS) -o $(BUILDDIR)/$@

$(LIBPATH): $(OBJ)
	@echo LD libany$(NAME).so
	@g++ $(BUILDOBJ) -o $@ -shared `dsvt-libs`

.PHONY: clean
clean:
	@rm -f $(BUILDOBJ) $(LIBPATH)

.PHONNY: install
install: all
	mkdir -p $(TOPDIR)/lib
	cp $(LIBPATH) $(TOPDIR)/lib/

