//Tryopen

#include <iostream>
#include <fstream>
#include <string>


int main (void){

	/*char place[] ="read_map/open.txt";
	std::ifstream fe; 
	fe.open(place);
	std::cout<<fe.is_open();
	char a[128];
	fe>>a;
	std::cout<<a;
	std::ifstream fa(a);
	std::cout<<fa.is_open();
	fa>>a;
	std::cout<<a;*/


	/*time_t  timev;
	time(&timev);
	std::cout<<ctime(&timev);
	std::cout<<(365*47*24*3600)<<std::endl;
	std::cout<<timev<<std::endl;

	int year=(timev/(365*24*3600));

	//std::cout<<"\n---"<<(timev-(365*24*3600*30))%(365*24*3600);
	char place[] ="read_map/open.txt";
	std::ifstream fe; 
	fe.open(place);
	if(fe.is_open()!=1){
		std::cout<<"No existe\n";
		fe.close();
		std::ofstream fa(place);
	}*/

time_t now = time(0);

  // std::cout << "Number of sec since January 1,1970:" << now << std::endl;

   tm *ltm = localtime(&now);

   // print various components of tm structure.
   /*std::cout << "Year" << 1900 + ltm->tm_year<<std::endl;
   std::cout << "Month: "<< 1 + ltm->tm_mon<< std::endl;
   std::cout << "Day: "<<  ltm->tm_mday << std::endl;
   std::cout << "Time: "<< 1 + ltm->tm_hour << ":";
   std::cout << 1 + ltm->tm_min << ":";
   std::cout << 1 + ltm->tm_sec << std::endl;*/

   std::string tim;
   tim=std::to_string(((((1900+ltm->tm_year)*100)+ltm->tm_mon+1)*100)+ltm->tm_mday);
   if(ltm->tm_hour<10)tim+='0';
   tim+=std::to_string(ltm->tm_hour);
      if(ltm->tm_min<10)tim+='0';
   tim+=std::to_string(ltm->tm_min);
      if(ltm->tm_sec<10)tim+='0';
   tim+=std::to_string(ltm->tm_sec);
   std::cout<< tim;
  

}	
