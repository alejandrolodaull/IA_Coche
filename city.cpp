#include "city.hpp"


city::city(int size, int obs){
	srand(time(NULL));
	int x, y;
	std::string f;

	y_=x_=size;
	obs_=obs;

	f_=3;


	x=x_;
	y=y_;
		
		std::cout<<obs_<<" "<<x_<<" "<<y_<<" ";
		obs_ = x_*y_ * obs_/100;
	

	c_=new int*[x+2];
	for(int i=0;i<x_+2;i++)
		c_[i]=new int[y_+2];


	for(int i=0;i<(x+2);i++){
		for(int j=0;j<(y+2);j++){
			if(i==0 || i==(x_+1) || j==0 || j==(y_+1))	c_[i][j]=0;
			else c_[i][j]=1;
		}
	}


		gen_car();
		gen_obs();
		gen_end();



ver=false;

	auto_move();
}


int city::get_val(int x, int y){
	return c_[x][y];
}
void city::gen_car(){
	x_car=((rand()%x_)+1);
	y_car=((rand()%y_)+1);
	c_[x_car][y_car]=3;
}
void city::gen_obs(){
	int contador=0;
	int x,y;
	while(contador<obs_){
		x=((rand()%x_)+1);
		y=((rand()%y_)+1);
		if(get_val(x,y)==1){
			c_[x][y]=2;
			contador++;
		}
	}
}
void city::gen_end(){
	int x,y;
	do{
		x=((rand()%x_)+1);
		y=((rand()%y_)+1);
	}while(get_val(x,y)!=1);
	c_[x][y]=4;
	x_v=x;
	y_v=y;
}



void city::auto_move(){
	
	double t;
	t = clock();

	n_movimientos = 0;

	
	mapa.resize(x_+2);
	for(int i = 0; i < x_+2; i++) {
		mapa[i].resize(y_+2);
		for(int j = 0; j < y_+2; j++)
			mapa[i][j] = false;
	}
	mapa[x_car][y_car] = true;
	std::vector<int> pos_objetivo;
	pos_objetivo = {x_car, y_car};

	bool nollego=true;
	bool nosalir=true;

	mov='e';
	int x_tem,y_tem;
	do {
		//if(metropolis)imprimir_metropolis();
		//else imprimir();

		x_tem=x_car;
		y_tem=y_car;


		//usleep(90000);
		

		mov=get_next_move(pos_objetivo);
		if(mov != 'n')
			n_movimientos++;


		switch(mov) {
			case 'w':x_tem-=1;break;
			case 'a':y_tem-=1;break;
			case 's':x_tem+=1;break;
			case 'd':y_tem+=1;break;

			case 'n': nosalir=false;
		}
		mov='e';

		if(c_[x_tem][y_tem]==1){
			c_[x_car][y_car]=1;
			c_[x_tem][y_tem]=3;
			x_car=x_tem;
			y_car=y_tem;
		}
		else if(c_[x_tem][y_tem]==4){
			nollego=false;
		}
		
		bool found = false;
		int i = 0;
		while((i < posibilidades.size()) && (!found)) {
			if((posibilidades[i][0] == x_car) && (posibilidades[i][1] == y_car))
				found = true;
			i++;
		}
		if(found) posibilidades.erase(posibilidades.begin() + i - 1);




	} while(nollego & nosalir);
	
	t = clock() - t;
	if(!nollego)std::cout<<"1 ";
	else std::cout<<"0 ";
	
	std::cout << n_movimientos;
	std::cout << " " << 	t / (double) CLOCKS_PER_SEC<<"\n";
}


char city::get_next_move(std::vector<int>& pos_objetivo) {
	char move;
	std::vector<int> aux, victoria = {x_v, y_v}, car = {x_car, y_car};
	std::vector<std::vector<int> > opciones, opciones_aux;
	
	//if(car == pos_objetivo) en_camino = false;

	if((get_val(x_car-1, y_car) == 1) || (get_val(x_car-1, y_car) == 4)) {
		aux = {x_car-1, y_car};
		opciones.push_back(aux);
	}
	if((get_val(x_car+1, y_car) == 1) || (get_val(x_car+1, y_car) == 4)) {
		aux = {x_car+1, y_car};
		opciones.push_back(aux);
    }
	if((get_val(x_car, y_car-1) == 1) || (get_val(x_car, y_car-1) == 4)) {
		aux = {x_car, y_car-1};
		opciones.push_back(aux);
    }
	if((get_val(x_car, y_car+1) == 1) || (get_val(x_car, y_car+1) == 4)) {
		aux = {x_car, y_car+1};
		opciones.push_back(aux);
    }

	std::vector<bool> borrar;
	borrar.resize(opciones.size());
	for(int i = 0; i < opciones.size(); i++) {
		if(mapa[opciones[i][0]][opciones[i][1]])
			borrar[i] = true;
	}
	opciones_aux = opciones;
	opciones.clear();
	for(int i = 0; i < opciones_aux.size(); i++) {
		if(!borrar[i])
			opciones.push_back(opciones_aux[i]);
	}
	

	for(int i = 0; i < opciones.size(); i++) {
		posibilidades.push_back(opciones[i]);
		mapa[opciones[i][0]][opciones[i][1]] = true;
	}


	if(!posibilidades.empty()) {
		if(car == pos_objetivo) {
			float min = INFINITY;
			int elegida = 0;
			for(int i = 0; i < posibilidades.size(); i++)
				if(f(posibilidades[i], victoria, f_) + f(car, posibilidades[i], f_)/2 < min) {
					min = f(posibilidades[i], victoria, f_) + f(car, posibilidades[i], f_)/2;
					elegida = i;
				}
			pos_objetivo = posibilidades[elegida];
		}
		aux = encontrar_camino(pos_objetivo);
		
		if(aux[0] == x_car) {
			if(aux[1] == y_car-1)
				move = 'a';
			else if(aux[1] == y_car+1)
				move = 'd';
		}
		else
			if(aux[0] == x_car-1)
				move = 'w';
			else if(aux[0] == x_car+1)
				move = 's';
	}
	else
		move = 'n';

	
	
	return move;
}


float city::f(std::vector<int> casilla, std::vector<int> objetivo, int formula) {
	
	switch(formula)
	{
		case 1: return (abs(casilla[0] - objetivo[0]) + abs(casilla[1] - objetivo[1]));break;
		
		case 2: return sqrt(pow(casilla[0] - objetivo[0], 2) + pow(casilla[1] - objetivo[1], 2)); break;
		
		case 3: return abs(casilla[0] - objetivo[0]) > abs(casilla[1] - objetivo[1])? abs(casilla[0] - objetivo[0]) : abs(casilla[1] - objetivo[1]); break;
		
		default: break;
	}
	
}


std::vector<int> city::encontrar_camino(std::vector<int> objetivo) {
	std::set<recorrido> lista;
	std::set<recorrido> l_cerrada;
	recorrido aux;
	aux.add(x_car, y_car);
	lista.insert(aux);
	
	while((lista.begin()->get_end() != objetivo) && (!lista.empty())) {
		aux = *lista.begin();
		lista.erase(lista.begin());
		l_cerrada.insert(aux);
		if(mapa[aux.get_end()[0] - 1][aux.get_end()[1]] && !aux.existe(aux.get_end()[0] - 1, aux.get_end()[1]))
			insert_check(lista, l_cerrada, aux.create(aux.get_end()[0] - 1, aux.get_end()[1]));
		if(mapa[aux.get_end()[0] + 1][aux.get_end()[1]] && !aux.existe(aux.get_end()[0] + 1, aux.get_end()[1]))
			insert_check(lista, l_cerrada, aux.create(aux.get_end()[0] + 1, aux.get_end()[1]));
		if(mapa[aux.get_end()[0]][aux.get_end()[1] - 1] && !aux.existe(aux.get_end()[0], aux.get_end()[1] - 1))
			insert_check(lista, l_cerrada, aux.create(aux.get_end()[0], aux.get_end()[1] - 1));
		if(mapa[aux.get_end()[0]][aux.get_end()[1] + 1] && !aux.existe(aux.get_end()[0], aux.get_end()[1] + 1))
			insert_check(lista, l_cerrada, aux.create(aux.get_end()[0], aux.get_end()[1] + 1));
	}
	
	if(lista.empty()) std::cout << "ALGO VA MAL, LAS POSIBILIDADES DEBEN ESTAR EN EL MAPA" << std::endl;
	
	return lista.begin()->get_first();
}


void city::insert_check(std::set<recorrido>& lista, std::set<recorrido>& l_cerrada, recorrido rec) {
	std::set<recorrido>::iterator it = l_cerrada.begin();
	bool found = false;
	while((it != l_cerrada.end()) && !found) {
		if(rec.get_end() == it->get_end()) {
			found = true;
			if(rec.get_coste() < it->get_coste()) {
				l_cerrada.erase(*it);
				lista.insert(rec);
			}
		}
		it++;
	}

	// si no estaba en la lista cerrada se comprueba en la abierta
	// si estaba en la cerrada y era mejor ya se insertó, si no lo era no hay que hacer nada más
	if(!found) {
		it = lista.begin();
		while((it != lista.end()) && !found) {
			if(rec.get_end() == it->get_end()) {
				found = true;
				if(rec.get_coste() < it->get_coste()) {
					lista.erase(*it);
					lista.insert(rec);
				}
			}
			it++;
		}
	}
	
	if(!found)		//si tampoco se encontró en la lista abierta se inserta sin más
		lista.insert(rec);
	
}









