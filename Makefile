
CC=g++ -O3 -Wno-deprecated
STD=-std=c++11
CFLAGS=-framework OpenGL -framework GLUT 
LIB = -lm -ldl -lrt

all: main.o
	$(CC) $(STD) $(CFLAGS) -o main main.o 
	
main.o: *.cpp
	$(CC) $(STD) $(CFLAGS) -c *.cpp 

run: main
	./main

clean:
	rm -rf *o main *.gch
