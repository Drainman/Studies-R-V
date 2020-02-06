#include <stdio.h>
#include <iostream>

using namespace std;


void logger(string type,string msg){
	cout << "["<< type << "] ~ " << msg << endl;
}


void print_seperator(){
	cout << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << endl;
}
