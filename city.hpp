#ifndef __CITY__
#define __CITY__

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <fstream>

#include <ncurses.h>
#include <unistd.h>

#include <math.h>

#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KRED  "\x1B[31m"
#define RST  "\x1B[0m"
#define NEG  "\x1B[1m"
#define NEGWHIT "\x1B[1m\x1B[37m"
#define NEGBLU "\x1B[1m\x1B[34m"

#define KSYEL  "\x1B[93m"

#define ANCHO 65
#define ALTO 39

class city{

	int x_;
	int y_;
	int obs_;
	int** c_;

	int row,coll;


	int x_car, y_car;
	int x_v, y_v;
	
	char mov;

	bool metropolis;
	int x_mo, x_mm;
	int y_mo, y_mm;

	bool manual;

	std::vector<std::string> col={KGRN,     KWHT,     KRED,          NEGBLU,     KYEL,	KSYEL,		NEGWHIT};
	//							  lados  -  fondo  -  obstaculos  -  coche  -  final -  excavadora -Visitado
	//							  0         1         2              3         4		5			6
	char fig[7]=				{'#',		'#',	  '#',			 '8',	   '@',     '#',        '#'};

public:
	city();
	city(std::string a);
	int get_val(int,int);

	void gen_car();
	void gen_obs();
	void gen_end();

	void excavadora();
	void savemap();
	void readmap();
	

	void imprimir();
	void imprimir_metropolis();

	void move();
	char auto_move();
	void ajustar3(int xaux, int yaux);
};




#endif