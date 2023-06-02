SRC = $(wildcard *.cxx)
OBJ = $(SRC:.cxx=.o)

.SUFFIXES: .cxx .o

CXXFLAGS = `dsvt-cflags` -Wall -ggdb3
LDFLAGS =  `dsvt-libs` -lanyutils

.cxx.o:
	gcc -c $< $(CXXFLAGS)

all: $(OBJ)
	g++ $(OBJ) -o `dsvt-component-name` $(LDFLAGS)

clean:
	rm -f $(OBJ) `dsvt-component-name`
	
