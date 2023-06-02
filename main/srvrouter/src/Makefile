SRC = $(wildcard *.cxx)
OBJ = $(SRC:.cxx=.o)

NAME := $(shell dsvt-component-name)
TOPDIR := $(shell dsvt-topdir)

.SUFFIXES: .cxx .o

CXXFLAGS := $(shell dsvt-cflags) -I../include -Wall -ggdb3

.cxx.o:
	gcc -c $< $(CXXFLAGS)

.PHONNY: all
all: libany$(NAME).so

libany$(NAME).so: $(OBJ)
	g++ $(OBJ) -o libany$(NAME).so -shared `dsvt-libs`

.PHONNY: clean
clean:
	rm -f $(OBJ) libany$(NAME).so

.PHONNY: install
install: all
	mkdir -p $(TOPDIR)/lib
	cp libany$(NAME).so $(TOPDIR)/lib/

