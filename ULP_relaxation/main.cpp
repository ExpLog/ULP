#include <iostream>
#include <sstream>

#include "solver.h"


void main(int argc, char *argv []){
	std::string s = "pmedian-dimacs/pmed01.dimacs" ;

	solver instance(s) ;

	std::system("pause") ;
}