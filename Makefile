#Standard C++
CC=g++
CC_OMP=g++-9
WARNINGS=-w -Wno-deprecated
STD=-std=c++14
CFLAGS=-framework OpenGL -framework GLUT
OMP=-fopenmp
LIB=-lm -ldl -lrt
LDFLAGS=-L/usr/local/opt/llvm/lib
CPPFLAGS=-I/usr/local/opt/llvm/include

#Optimization levels
OPT=-O3
OPT_D=-g -O1

#Target Rules
all: main main_d

release r: main
debug d: main_d
omp: main_omp
omp_d: main_omp_d


cr: clean release
cd: clean debug

rr: release
	./main

dr: debug
	./main_d

crr: clean release
	./main

cdr: clean debug
	./main_d

cor: clean omp
	./main_omp

cod: clean omp_d
	./main_omp_d


#Compile Target
CPU_TARGET=Main.cpp

#Release
main.o: $(CPU_TARGET)
	$(CC) $(WARNINGS) $(OPT) $(STD) $(CFLAGS) -o $@ -c $<
	
main: main.o
	$(CC) $(WARNINGS) $(OPT) $(STD) $(CFLAGS) -o $@ $+


#Debug
main_d.o: $(CPU_TARGET)
	$(CC) $(WARNINGS) $(OPT_D) $(STD) $(CFLAGS) -o $@ -c $<
	
main_d: main_d.o
	$(CC) $(WARNINGS) $(OPT_D) $(STD) $(CFLAGS) -o $@ $+

tct.o: tinycthread.c
	$(CC) $(WARNINGS) $(OPT) $(STD) $(CFLAGS) -o $@ -c $<


#OpenMP
main_omp.o: $(CPU_TARGET)
	$(CC_OMP) $(WARNINGS) $(OPT) $(STD) $(CFLAGS) $(OMP) -o $@ -c $<
	
main_omp: main_omp.o
	$(CC_OMP) $(WARNINGS) $(OPT) $(STD) $(CFLAGS) $(OMP) -o $@ $+

#OpenMP Debug
main_omp_d.o: $(CPU_TARGET)
	$(CC_OMP) $(WARNINGS) $(OPT_D) $(STD) $(CFLAGS) $(OMP) -o $@ -c $<
	
main_omp_d: main_omp_d.o
	$(CC_OMP) $(WARNINGS) $(OPT_D) $(STD) $(CFLAGS) $(OMP) -o $@ $+

clean c:
	rm -rf *o main main_d main_omp main_omp_d *.gch
