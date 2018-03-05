
CC=g++-7 -O3 -Wno-deprecated
STD=-std=c++11
CFLAGS=-framework OpenGL -framework GLUT -fopenmp
LIB = -lm -ldl -lrt
LDFLAGS=  -L/usr/local/opt/llvm/lib
CPPFLAGS= -I/usr/local/opt/llvm/include

all: main.o global.o vector.o
	$(CC) $(STD) $(CFLAGS) -o main *.o 
	
main.o: Main.cpp
	$(CC) $(STD) $(CFLAGS) -c Main.cpp 

global.o: Global.cpp
	$(CC) $(STD) $(CFLAGS) -c Global.cpp 

vector.o: Vector.cpp
	$(CC) $(STD) $(CFLAGS) -c Vector.cpp 

run: main.o global.o vector.o
	$(CC) $(STD) $(CFLAGS) -o main *.o 
	./main

	
clean:
	rm -rf *o main *.gch
