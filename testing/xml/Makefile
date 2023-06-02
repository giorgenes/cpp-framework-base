SRC = $(wildcard *.cxx)
OBJ = $(SRC:.cxx=.o)

.SUFFIXES: .cxx .o

CXXFLAGS = `dsvt-cflags` -Wall -ggdb3 -I../include

.cxx.o:
	gcc -c $< $(CXXFLAGS)

all: $(OBJ)
	g++ $(OBJ) -o test.cgi `dsvt-libs` -ggdb3

clean:
	rm -f $(OBJ) test.cgi
	
