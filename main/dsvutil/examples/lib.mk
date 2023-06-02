SRC = $(wildcard *.cxx)
OBJ = $(SRC:.cxx=.o)

.SUFFIXES: .cxx .o

CXXFLAGS = `dsvt-cflags` -Wall -ggdb3

.cxx.o:
	gcc -c $< $(CXXFLAGS)

all: $(OBJ)
	g++ $(OBJ) -o libany`dsvt-component-name`.so `dsvt-libs` -shared

clean:
	rm -f $(OBJ) libany`dsvt-component-name`.so
	
