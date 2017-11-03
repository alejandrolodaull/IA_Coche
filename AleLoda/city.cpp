#include "city.hpp"

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

	move();


}

city::city(){
	srand(time(NULL));
	int x, y;
	std::cout<<"Filas: ";
	std::cin>>x;
	x_=x;
	std::cout<<"Columnas: ";
	std::cin>>y;
	y_=y;




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
		std::cout<<"Obstaculos: ";
		std::cin>>obs_;
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


	move();
	//auto_move();
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
		//imprimir();
		ajustar3(e_x,e_y);

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

int city::get_val(int x, int y){
	return c_[x][y];
}
void city::imprimir(){
	system("clear");
	std::cout<<"Filas x: "<<x_<<"\tColumnas y: "<<y_<<"\tObstaculos: "<<obs_<<" "<<x_car<<","<<y_car<<" "<<x_v<<","<<y_v<<"\n";
	for(int i=0;i<x_+2;i++){
		for(int j=0;j<y_+2;j++){
			int aux=get_val(i,j);
			std::cout<<col[aux]<<fig[aux]<<RST<<" ";
			//if(get_val(i,j)==3)std::cout<<NEG<<col[3]<<'8'<<RST<<" ";

			//else if(get_val(i,j)==4)std::cout<<col[4]<<'@'<<RST<<" ";
			//	else std::cout<<col[get_val(i,j)]<<'#'<<RST<<" ";
		}
		std::cout<<"\n";
	}
}
void city::imprimir_metropolis(){
	system("clear");
	
	//std::cout<<"Filas x: "<<x_<<"\tColumnas y: "<<y_<<"\tObstaculos: "<<obs_<<"\n";
	for(int i=x_mo;i<x_mm+2;i++){
		for(int j=y_mo;j<y_mm+2;j++){
			int aux=get_val(i,j);
			std::cout<<col[aux]<<fig[aux]<<RST<<" ";
		//	if(get_val(i,j)==3)std::cout<<NEG<<col[3]<<'8'<<RST<<" ";

			//else if(get_val(i,j)==4)std::cout<<col[4]<<'@'<<RST<<" ";
				//else std::cout<<col[get_val(i,j)]<<'#'<<RST<<" ";
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
		/*if(metropolis)imprimir_metropolis();
		else*/ //imprimir();
		ajustar3(x_car,y_car);
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
			c_[x_car][y_car]=6;
			c_[x_tem][y_tem]=3;
			x_car=x_tem;
			y_car=y_tem;
		}
		else if(c_[x_tem][y_tem]==6){
			c_[x_car][y_car]=6;
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

void city::ajustar3(int xaux, int yaux){
		int columnasy,filasx;

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

		std::cout<<"x: "<<x_mo<<" + "<<x_mm<<std::endl;
		std::cout<<"y: "<<y_mo<<" + "<<y_mm<<std::endl;
		system("clear");
		for(int i=x_mo;i<x_mm;i++){
			for(int j=y_mo; j<y_mm;j++){
				int aux=c_[i][j];
				std::cout<<col[aux]<<fig[aux]<<" "<<RST;
			}
			std::cout<<std::endl;
		}
}
