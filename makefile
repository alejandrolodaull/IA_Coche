all: 1

1: recorrido.o city.o main.o
	g++ -std=c++11 recorrido.o city.o main.o -o citycar -lcurses
	
recorrido.o: recorrido.cpp
	g++ -std=c++11 -c recorrido.cpp

city.o: city.cpp 
	g++ -std=c++11 -c city.cpp

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

make clean:
	rm *.o