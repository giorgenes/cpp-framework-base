SRC = $(wildcard *.cxx)
OBJ = $(SRC:.cxx=.o)

.SUFFIXES: .cxx .o

TOPDIR := $(shell dsvt-topdir)
CXXFLAGS := $(shell dsvt-cflags) -Wall -ggdb3

NAME := $(shell dsvt-component-name)

.cxx.o:
	@echo CC $(NAME)/$<
	@gcc -c $< $(CXXFLAGS)

.PHONNY: all
all: $(NAME) server

$(NAME): $(OBJ)
	@echo LD $(NAME)
	@g++ main.o -o $(NAME) `dsvt-libs` 


server: $(OBJ)
	@echo LD server
	@g++ server.o -o server `dsvt-libs` 


.PHONNY: clean
clean:
	@rm -f $(OBJ) $(NAME)

.PHONNY: install
install: all
	mkdir -p $(TOPDIR)/bin
	cp $(NAME) $(TOPDIR)/bin/
