all: 1

1: city.o main.o
	g++ -std=c++11 city.o main.o -o citycar -lcurses

city.o: city.cpp 
	g++ -std=c++11 -c city.cpp

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

make clean:
	rm *.o
	
