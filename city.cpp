#include "city.hpp"


city::city(){
	srand(time(NULL));
	int x, y;
	std::string f;
	std::cout<<"Filas: ";
	std::cin>>x;
	x_=x;
	std::cout<<"Columnas: ";
	std::cin>>y;
	y_=y;

	getchar();
	
	do
	{
		std::cout << "Fórmula (Manhattan/Euclidea/Mahalanobis): ";
		std::getline(std::cin, f);
		
		if ((f == "Manhattan") || (f == "manhattan") || (f=="1"))
			f_ = 1;
		else if ((f == "Euclidea") || (f == "euclidea") || (f=="2"))
			f_ = 2;
		else if ((f == "Mahalanobis") || (f == "mahalanobis") || (f=="3"))
			f_ = 3;
		else
		{
			f_ = 0;
			std::cout << "Fórmula errónea" << std::endl;
		}
	}while(f_ == 0);


	if(x_>ALTO | y_>ANCHO)metropolis=true;
	else metropolis=false;


	if(metropolis){
		x_mo=y_mo=0;
		x_mm=x_;
		y_mm=y_;
		if(x_>ALTO)x_mm=ALTO;
		if(y_>ANCHO)y_mm=ANCHO;
	}


	do{
		std::cout<<"Obstaculos(%): ";
		std::cin>>obs_;
		obs_ = x*y * obs_/100;
	}while(obs_>((x*y)-2));

	c_=new int*[x+2];
	for(int i=0;i<x+2;i++)
		c_[i]=new int[y+2];


	for(int i=0;i<(x+2);i++){
		for(int j=0;j<(y+2);j++){
			if(i==0 || i==(x_+1) || j==0 || j==(y_+1))	c_[i][j]=0;
			else c_[i][j]=1;
		}
	}

	{
		bool respuesta_correcta=false;
		do{
			std::cout<<"Manual: (s/n)";
			char manual_respuesta;
			std::cin>>manual_respuesta;
			respuesta_correcta=true;
			if(manual_respuesta=='s')manual=true;
			else if(manual_respuesta=='n')manual=false;
			else {std::cout<<"Respuesta Incorrecta\n"; respuesta_correcta=false;}
		}while (!respuesta_correcta);
	}

	if(!manual){
		gen_car();
		gen_obs();
		gen_end();
	}
	else{
		excavadora();
	}

	std::cout<<"Imprimir traza? (s/n)";
	{char auxver;
		std::cin>>auxver;
		if(auxver=='s')ver=true;
		else ver=false;
	}
	//move();
	auto_move();
}


void city::excavadora(){
	c_[1][1]=5;
	int e_x=1;
	int e_y=1;
	int e_tx=1;
	int e_ty=1;

	int obs_exc=0;
	int ant_x=0;

	bool nosalir=true;

	char mov='e';

	int anterior=1;

	bool nocoche=true;
	bool novictory=true;

	do{
		if(anterior==2)ant_x=1;else ant_x=0;
		obs_=obs_exc+ant_x;
		imprimir();

		initscr(); 
		cbreak();
    	noecho();
    	nodelay(stdscr, TRUE);
    	scrollok(stdscr, TRUE);

    	usleep(90000);

		mov=getch();

		endwin();
	
		switch(mov){
			case 'w': if(e_x>1)e_tx--;
				break;
			case 'a': if(e_y>1)e_ty--;
				break;
			case 's': if(e_x<x_)e_tx++;
				break;
			case 'd': if(e_y<y_)e_ty++;
				break;
				
			case 'x': 
				if(anterior==3)nocoche=true; 
				if(anterior==4)novictory=true;
				anterior=2;break;
			case 'v': 
				if(novictory){
				if(anterior==3)nocoche=true; 
				anterior=4; novictory=false;}break;
			case 'c': 
				if(nocoche)  {nocoche=false;
					if(anterior==4)novictory=true;
					anterior=3;};break;
			case 'b': 
				if(anterior==3)nocoche=true; 
				if(anterior==4)novictory=true;
				anterior=1;break;
			case 'q': 
				c_[e_x][e_y]=anterior;
				nosalir=false; break;
		}

		if(e_tx!=e_x|e_ty!=e_y){

			if(anterior==3){
				x_car=e_x;
				y_car=e_y;
			}
			else if(anterior==4){
				x_v=e_x;
				y_v=e_y;
			}

			c_[e_x][e_y]=anterior;
			if(anterior==2)obs_exc++;

			if(c_[e_tx][e_ty]==2)obs_exc--;
			anterior=c_[e_tx][e_ty];
			c_[e_tx][e_ty]=5;
			e_x=e_tx;
			e_y=e_ty;
		}





	}while(nosalir);

	std::cin.ignore(256,'\n');

	if(novictory==false && nocoche==false){ 

		std::cout<<"¿Guardar mapa? (s/n)";

		char a= getchar();
		if(a=='s'){
			savemap();
		}
	}
}

/*void city::savemap(){
	/*Ver si existe el archivo
			-SI-> mover a otra carpeta
	  Crear archivo nuevo
	  Volcar datos
	*/
	/*char place[] ="read_map/open.txt";
	std::ifstream fe; 
	fe.open(place);
	if(fe.is_open()==1){

	}
	else{

	}

}*/

int city::get_val(int x, int y){
	return c_[x][y];
}
void city::imprimir(){
	system("clear");
	std::cout<<"Filas: "<<x_<<"\tColumnas: "<<y_<<"\tObstaculos: "<<obs_<<"\n";
	for(int i=0;i<x_+2;i++){
		for(int j=0;j<y_+2;j++){
			if(get_val(i,j)==3)std::cout<<NEG<<col[3]<<'8'<<RST<<" ";

			else if(get_val(i,j)==4)std::cout<<col[4]<<'@'<<RST<<" ";
				else std::cout<<col[get_val(i,j)]<<'#'<<RST<<" ";
		}
		std::cout<<"\n";
	}
}
void city::imprimir_metropolis(){
	system("clear");
	std::cout<<"Filas: "<<x_<<"\tColumnas: "<<y_<<"\tObstaculos: "<<obs_<<"\n";
	for(int i=x_mo;i<x_mm+2;i++){
		for(int j=y_mo;j<y_mm+2;j++){
			if(get_val(i,j)==3)std::cout<<NEG<<col[3]<<'8'<<RST<<" ";

			else if(get_val(i,j)==4)std::cout<<col[4]<<'@'<<RST<<" ";
				else std::cout<<col[get_val(i,j)]<<'#'<<RST<<" ";
		}
		std::cout<<"\n";
	}
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

void city::move(){

	bool nollego=true;
	bool nosalir=true;

	mov='e';
	int x_tem,y_tem;
	do{
		if(metropolis)imprimir_metropolis();
		else imprimir();

		x_tem=x_car;
		y_tem=y_car;

		initscr(); 
		cbreak();
    	noecho();
    	nodelay(stdscr, TRUE);
    	scrollok(stdscr, TRUE);

    	usleep(90000);

		mov=getch();

		endwin();

		switch(mov){
			case 'w':x_tem-=1;break;
			case 'a':y_tem-=1;break;
			case 's':x_tem+=1;break;
			case 'd':y_tem+=1;break;

			case 'i': 
				if(x_mo>0){
					x_mo--;
					x_mm--;
				}
				break;
			
			case 'j': 
				if(y_mo>0){
					y_mo--;
					y_mm--;
				}
				break;
			case 'k': 
				if(x_mm<x_){
					x_mo++;
					x_mm++;
				}
				break;
			case 'l': 
				if(y_mm<y_){
					y_mo++;
					y_mm++;
				}
				break;

			case 'q': nosalir=false;
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

		
	}while(nollego & nosalir);

	if(!nollego)std::cout<<col[4]<<"GANASTE\n\tGANASTE\n\t\tGANASTE\n\t\t\tGANASTE\n\t\t\t\tGANASTE\n"<<RST;
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


/////////
/*
		if(ver)
		{
			char auxver='s';
			initscr(); 
			cbreak();
	    	noecho();
	    	nodelay(stdscr, TRUE);
	    	scrollok(stdscr, TRUE);
			auxver=getch();
			endwin();			
			if(auxver=='q')ver=false;



		}*/
		if(ver)
		ajustar3(x_car,y_car);
		



		//////////



	} while(nollego & nosalir);
	
	t = clock() - t;

	if(!nollego)std::cout<<col[4]<<"GANASTE\n\tGANASTE\n\t\tGANASTE\n\t\t\tGANASTE\n\t\t\t\tGANASTE\n"<<RST;
	else std::cout<<"No se puede llegar al destino\n";
	
	std::cout << col[0] << "Número de movimientos: " << n_movimientos << RST << '\n';
	std::cout << "El algoritmo tardó: " << 	t / (double) CLOCKS_PER_SEC << " segundos\n";
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










//////////
void city::ajustar3(int xaux, int yaux){
		int columnasy,filasx;
		if(x_>100 | y_>100)
			usleep(60000);
		else usleep(100000);
		initscr();				
		getmaxyx(stdscr,filasx,columnasy);	
		refresh();
		endwin();
		
		filasx--;
		columnasy/=2;
		if(filasx<x_ && columnasy<y_){		//X  e  Y  se salen del cuadro
			x_mo=xaux-(filasx/2);
			x_mm=xaux+(filasx/2);
			y_mo=yaux-(columnasy/2);
			y_mm=yaux+(columnasy/2);

			if(y_mo<0){						//estamos pegados a y=0
				y_mo=0;
				y_mm=columnasy;
			}
			else if(y_mm>y_){				//estamos pegados a y=max
				y_mm=y_+2;
				y_mo=(y_+2)-columnasy;
			}
			if(x_mo<0){						// x=0
				x_mo=0;
				x_mm=filasx;
			}
			else if(x_mm>x_){				// x max
				x_mm=x_+2;
				x_mo=(x_+2)-filasx;
			}
		}
		else if(filasx>x_ && columnasy<y_){		//la Y es la justa, la que le falta
			y_mo=yaux-(columnasy/2);
			y_mm=yaux+(columnasy/2);
			x_mo=0;
			x_mm=x_+2;
			if(y_mo<0){
				y_mo=0;
				y_mm=columnasy;
			}
			else if(y_mm>y_){
				y_mm=y_+2;
				y_mo=(y_+2)-columnasy;
			}

		}
		else if(filasx<x_ && columnasy>y_){
			x_mo=xaux-(filasx/2);
			x_mm=xaux+(filasx/2);
			y_mo=0;
			y_mm=y_+2;

			if(x_mo<0){
				x_mo=0;
				x_mm=filasx;
			}
			else if(x_mm>x_){
				x_mm=x_+2;
				x_mo=(x_+2)-filasx;
			}
		}
		else{
			x_mo=0;
			y_mo=0;
			x_mm=x_+2;
			y_mm=y_+2;
		}

	//	std::cout<<"x: "<<x_mo<<" + "<<x_mm<<std::endl;
	//	std::cout<<"y: "<<y_mo<<" + "<<y_mm<<std::endl;
		system("clear");
		for(int i=x_mo;i<x_mm;i++){
			for(int j=y_mo; j<y_mm;j++){
				int aux=c_[i][j];
				std::cout<<col[aux]<<fig[aux]<<" "<<RST;
			}
			std::cout<<std::endl;
		}
}


void city::savemap(){
	std::string palabra;
	std::cout<<"Nombre del mapa? ";
	std::cin>>palabra;
	palabra="read_map/"+palabra+".txt";
	std::fstream file;
	file.open(palabra,std::fstream::out);
	file<<x_<<"\n"<<y_<<"\n";
	for(int i=1;i<=x_;i++){
		for (int j=1;j<=y_;j++){
			file<<c_[i][j]<<" ";
		}
		file<<"\n";
	}
}



city::city(std::string palabra){
	srand(time(NULL));
	std::ifstream file;
	file.open(palabra, std::ios::in);

	file>>x_;
	file>>y_;

	std::cout<<x_<<" "<<y_<<std::endl;

	c_=new int*[x_+2];
	for(int i=0;i<x_+2;i++)
		c_[i]=new int[y_+2];

	for(int i=0;i<(x_+2);i++){
		for(int j=0;j<(y_+2);j++){
			if(i==0 || i==(x_+1) || j==0 || j==(y_+1))	c_[i][j]=0;
			else{
				int aux;
				file>>aux;
				c_[i][j]=aux;
				if(aux==3){
					x_car=i;
					y_car=j;
				}
				else if(aux==4){
					x_v=i;
					y_v=j;
				}

			} 
		}
	}
	file.close();

	std::string f;
	do
	{
		std::cout << "Fórmula (Manhattan/Euclidea/Mahalanobis): ";
		std::getline(std::cin, f);
		
		if ((f == "Manhattan") || (f == "manhattan") || (f=="1"))
			f_ = 1;
		else if ((f == "Euclidea") || (f == "euclidea") || (f=="2"))
			f_ = 2;
		else if ((f == "Mahalanobis") || (f == "mahalanobis") || (f=="3"))
			f_ = 3;
		else
		{
			f_ = 0;
			std::cout << "Fórmula errónea" << std::endl;
		}
	}while(f_ == 0);

	std::cout<<"Imprimir traza? (s/n)";
	{char auxver;
		std::cin>>auxver;
		if(auxver=='s')ver=true;
		else ver=false;
	}
	auto_move();


}

/////////