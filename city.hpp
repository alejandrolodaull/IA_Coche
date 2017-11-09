#ifndef __CITY__
#define __CITY__

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <set>
#include <cmath>

#include <ncurses.h>
#include <unistd.h>

#include "recorrido.hpp"

#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KRED  "\x1B[31m"
#define RST  "\x1B[0m"
#define NEG  "\x1B[1m"

#define KSYEL  "\x1B[93m"

#define ANCHO 65
#define ALTO 39

class city{

	int x_;
	int y_;
	int f_;
	int obs_;
	int** c_;

	int x_car, y_car;
	int x_v, y_v;
	
	char mov;

	bool metropolis;
	bool ver;

	int x_mo, x_mm;
	int y_mo, y_mm;

	bool manual;
	int n_movimientos;

	std::vector<std::vector<bool> > mapa;
	std::vector<std::vector<int> > posibilidades;
	

	std::vector<std::string> col={KGRN,     KWHT,     KRED,          KBLU,     KYEL,	KSYEL};
	//							  lados  -  fondo  -  obstaculos  -  coche  -  final -  excavadora
	//							  0         1         2              3         4		5
	char fig[7]=				{'#',		'#',	  '#',			 '8',	   '@',     '#',        '#'};
public:

	city(int, int);

	int get_val(int,int);

	void gen_car();
	void gen_obs();
	void gen_end();

	void excavadora();
	void savemap();
	void readmap();
	


	void move();
	void auto_move();
	char get_next_move(std::vector<int>&);

	float f(std::vector<int>, std::vector<int>, int);
	std::vector<int> encontrar_camino(std::vector<int>);
	void insert_check(std::set<recorrido>&, std::set<recorrido>&, recorrido);



/////////////////
	void ajustar3(int xaux, int yaux);
/////////////////
};



#endif
