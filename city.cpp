#include "city.hpp"


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

	/*if(novictory! && nocoche!){ 
		std::cout<<"¿Guardar mapa? (s/n)";
		char a= getchar();
		if(a=='s'){
			savemap();
		}
	}*/
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
		ajustar3();
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


/*
char city::auto_move(){
	char aux;
	char aux2;

	if((x_car-x_v)>=0)aux='w';
	else aux='s';
	if((y_car-y_v)>=0)aux='a';
	else aux2='d';
	if(aux=='w' & c_[x_car-1][y_car]==4)return aux;
	else if(aux=='s' & c_[x_car+1][y_car]==4)return aux;
	else if(aux2=='a' & c_[x_car][y_car-1]==4)return aux2;
	else if(aux2=='d' & c_[x_car][y_car+1]==4)return aux2;
	else if(aux=='w' & c_[x_car-1][y_car]!=2)return aux;
	else if(aux=='s' & c_[x_car+1][y_car]!=2)return aux;
	else if(aux2=='d' & c_[x_car][y_car-1]!=2)return aux2;
	else if(aux2=='a' & c_[x_car][y_car+1]!=2)return aux2;
	else if(aux=='w' & c_[x_car-1][y_car]==6)return aux;
	else if(aux=='s' & c_[x_car+1][y_car]==6)return aux;
	else if(aux2=='d' & c_[x_car][y_car-1]==6)return aux2;
	else if(aux2=='a' & c_[x_car][y_car+1]==6)return aux2;


	//Alineado con la victoria
	if(x_car==x_v && y_car>y_v && c_[x_car-1][y_car]!=2 && c_[x_car-1][y_car]!=0)return 'a';
	if(x_car==x_v && y_car<y_v && c_[x_car+1][y_car]!=2 && c_[x_car+1][y_car]!=0)return 'd';
	if(y_car==y_v && x_car>x_v && c_[x_car][y_car-1]!=2 && c_[x_car][y_car-1]!=0)return 'w';
	if(y_car==y_v && x_car<x_v && c_[x_car][y_car+1]!=2 && c_[x_car][y_car+1]!=0)return 's';

	//Sin alinear con la victoria
	if(x_car>x_v && c_[x_car][y_car-1]!=2 && c_[x_car][y_car-1]!=0)return 'w';
	if(x_car<x_v && c_[x_car][y_car+1]!=2 && c_[x_car][y_car+1]!=0)return 's';
	if(y_car>y_v && c_[x_car-1][y_car]!=2 && c_[x_car-1][y_car]!=0)return 'a';
	if(y_car<y_v && c_[x_car+1][y_car]!=2 && c_[x_car+1][y_car]!=0)return 'd';

	if(x_car-x_v>=0 && y_car-y_v>=0)		//
	else if(x_car-x_v>=0 && y_car-y_v<=0)
	else if(x_car-x_v<=0 && y_car-y_v>=0)
	else if(x_car-x_v<=0 && y_car-y_v<=0)

int n,s,e,o;
n=s=e=o=0;
if(c_[x_car][y_car+1]==1)s++;
if(c_[x_car][y_car-1]==1)n++;
if(c_[x_car+1][y_car]==1)e++;
if(c_[x_car-1][y_car]==1)o++;
std::cout<<"vacio "<< n<<s<<e<<o;

if(c_[x_car][y_car+1]==4)s+=10;
if(c_[x_car][y_car-1]==4)n+=10;
if(c_[x_car+1][y_car]==4)e+=10;
if(c_[x_car-1][y_car]==4)o+=10;
std::cout<<"\nvictoria "<<n<<s<<e<<o;


if(c_[x_car][y_car+1]==2)s-=10;
if(c_[x_car][y_car-1]==2)n-=10;
if(c_[x_car+1][y_car]==2)e-=10;
if(c_[x_car-1][y_car]==2)o-=10;
std::cout<<"\nobstaculo "<<n<<s<<e<<o;

int aux, aux2;

if(x_car-x_v>0)o++;
else e++;
if(y_car-y_v>0)n++;
else s++;

std::cout<<"\nn vs s-e vs o "<<n<<s<<e<<o;

if( std::abs(x_car-x_v) < std::abs(y_car-y_v)) {
	if(x_car>x_v)o++;
	else e++;
}
else{
	if(y_car>y_v)n++;
	else s++;
}
std::cout<<"\nns vs eo "<<n<<s<<e<<o;


std::cout<<" "<<n<<"\n"<<o<<" "<<e<<"\n "<<s<<"\n";

char devo;
if(n>=s & n>=e & n>=o)devo= 'w';
if(s>n & s>=e & s>=o) devo= 's';
if(e>=o & e>=n & e>=s) devo = 'd';
if(o>e & o>=n & o >=s) devo = 'a';

std::cout<<devo;
getchar();
return devo;




}

*/

/*
void city::ajustar(){
	//coll = filas --> y

		initscr();				
		getmaxyx(stdscr,coll,row);	
		//row/=2;	
		//coll-=1;
		//row-=1;
		std::cout<<"x: "<<row<<" y:"<<coll<<std::endl;
		std::cout<<"max x:"<<x_<<" max y:"<<y_<<std::endl;
		getchar();
		refresh();
		endwin();

		if((row<y_) | ((coll)<x_))
			metropolis=true;
		else metropolis=false;

		if(metropolis){
			x_mo=(x_car-coll/2);
			y_mo=(y_car-row/2);
			x_mm=(x_car+coll/2);
			y_mm=(y_car+row/2);

			if(x_mo<0){
				x_mo=0;
				x_mm=coll;
			}else if(x_mm>x_){
				x_mm=x_-1;
				x_mo=x_-coll;
			}

			if(y_mo<0){
				y_mo=0;
				y_mm=row;
			}else if (x_mm>y_){
				y_mm=y_-1;
				y_mo=y_-row;
			}
			imprimir_metropolis();
		}
		else{
			imprimir();
		}

}*/
/*
void city::ajustar2(){
		//coll = filas --> y
		int columnasy,filasx;

		//y_ -->row
		//x_ -->coll


		initscr();				
		getmaxyx(stdscr,filasx,columnasy);	
		refresh();
		endwin();


		if(x_*2>=filasx){
			//x_mo
			if(x_car<=filasx/2){
				x_mo=0;
			}
			else {
				x_mo=x_car-filasx/2;
				if(x_mo>x_)x_mo=0;
			}

			//x_mm
			if(x_car>=filasx/2){
				x_mm=(x_car+filasx/2);
				if(x_mm>x_)x_mm=x_;
			}
			else{
				x_mm=x_-1;
			}


		}
		if(y_>=columnasy/2){
			//y_mo
			if(y_car<=columnasy/4){
				y_mo=0;
			}
			else {
				y_mo=y_car-columnasy/4;
				if(y_mo>y_)y_mo=0;
			}

			//y_mm
			if(y_car>=columnasy/4){
				y_mm=(y_car+columnasy/4);
				if(y_mm>y_)y_mm=y_;
			}
			else{
				y_mm=y_-1;
			}
		}

		system("clear");
		for(int i=x_mo;i<=x_mm-1;i++){
			for(int j=y_mo; j<=y_mm-1;j++){
				int aux=c_[i][j];
				std::cout<<col[aux]<<fig[aux]<<" "<<RST;
			}
			std::cout<<std::endl;
		}

		//getchar();
}*/
void city::ajustar3(){
		//coll = filas --> y
		int columnasy,filasx;

		//y_ -->row
		//x_ -->coll


		initscr();				
		getmaxyx(stdscr,filasx,columnasy);	
		refresh();
		endwin();
		filasx--;
		columnasy/=2;
		if(filasx<x_ && columnasy<y_){		//X  e  Y  se salen del cuadro
			x_mo=x_car-(filasx/2);
			x_mm=x_car+(filasx/2);
			y_mo=y_car-(columnasy/2);
			y_mm=y_car+(columnasy/2)+1;

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
			y_mo=y_car-(columnasy/2);
			y_mm=y_car+(columnasy/2);
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
			x_mo=x_car-(filasx/2);
			x_mm=x_car+(filasx/2);
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

		//getchar();
}
