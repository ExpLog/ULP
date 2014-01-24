#include <iostream>
#include <sstream>

#include "matrix.h"
#include "input.h"


void main(int argc, char *argv []){
	std::string s = "pmedian-dimacs/pmed01.dimacs" ;
	matrix m = matrix(1,1) ;
	loadFromFile(s,m) ;

	if(unitTest(m) == false)
		std::cout << "Unit test on file " << s << " has failed" << std::endl ;
	else
		std::cout << "Unit test on file " << s << " has succeeded" << std::endl ;

	std::system("pause") ;
}