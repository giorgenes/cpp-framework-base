SRC = $(wildcard *.cxx)
OBJ = $(SRC:.cxx=.o)

.SUFFIXES: .cxx

.cxx.o:
	g++ -c $< -Wall -g

all: $(OBJ)
	g++ $(OBJ) -o eacc -g -lexpat

clean:
	rm -f *.o eacc
