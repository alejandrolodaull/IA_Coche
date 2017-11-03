#include "recorrido.hpp"

recorrido::recorrido() {
	Coste = -1;
}


int recorrido::get_coste() const {
	return Coste;
}


std::vector<int> recorrido::get_first() const {
	return Camino[1];
}


std::vector<int> recorrido::get_end() const {
	return Camino.back();
}


void recorrido::add(int x, int y) {
	std::vector<int> aux = {x, y};
	Camino.push_back(aux);
	Coste++;
}


recorrido recorrido::create(int x, int y) {
	recorrido rec;
	rec.Camino = Camino;
	std::vector<int> aux = {x, y};
	rec.Camino.push_back(aux);
	rec.Coste = Coste + 1;

	return rec;
}


bool recorrido::existe(int x, int y) {
	bool existe = false;
	for(int i = 0; i < Camino.size(); i++)
		if((Camino[i][0] == x) && (Camino[i][1] == y))
			existe = true;

	return existe;
}


bool recorrido::operator<(const recorrido& rec) const {
	if(Coste != rec.Coste)
       return (Coste < rec.Coste);
	// si el coste es igual no importa el orden, pero hace falta criterio para que el set no los vea como iguales
	else {
		for(int i = 0; i < Camino.size(); i++) {
			if(Camino[i][0] < rec.Camino[i][0])
				return true;
			else if(Camino[i][0] > rec.Camino[i][0])
				return false;
			if(Camino[i][1] < rec.Camino[i][1])
				return true;
			else if(Camino[i][1] > rec.Camino[i][1])
				return false;
		}
	}
}


bool recorrido::operator>(const recorrido& rec) const {
	if(Coste != rec.Coste)
       return (Coste > rec.Coste);
	// si el coste es igual no importa el orden, pero hace falta criterio para que el set no los vea como iguales
	else {
		for(int i = 0; i < Camino.size(); i++) {
			if(Camino[i][0] > rec.Camino[i][0])
				return true;
			else if(Camino[i][0] < rec.Camino[i][0])
				return false;
			if(Camino[i][1] > rec.Camino[i][1])
				return true;
			else if(Camino[i][1] < rec.Camino[i][1])
				return false;
		}
	}
}


bool recorrido::operator==(const recorrido& rec) const {
	bool iguales = true;
	for(int i = 0; i < Camino.size(); i++)
		if((Camino[i][0] != rec.Camino[i][0]) || (Camino[i][1] != rec.Camino[i][1]))
			iguales = false;

	return iguales;
}