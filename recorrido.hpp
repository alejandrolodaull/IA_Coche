#ifndef __RECORRIDO__
#define __RECORRIDO__

#include <set>
#include <vector>

class recorrido {
	std::vector<std::vector<int> > Camino;
	int Coste;

public:

	recorrido();
	int get_coste() const;
	std::vector<int> get_first() const;
	std::vector<int> get_end() const;
	void add(int, int);
	recorrido create(int, int);
	bool existe(int, int);

	bool operator<(const recorrido&) const;
	bool operator>(const recorrido&) const;
	bool operator==(const recorrido&) const;
};

#endif