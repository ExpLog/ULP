#include <iostream>
#include <list>

#include "matrix.h"
#include "input.h"

class solver {
private:
	matrix _costMatrix ;	//holds the c_ij
	matrix _wij ;			//holds the w_ij
	matrix _uj ;			//holds the u_j
	double v ;				//holds the v

public:
	solver(std::string s){
		_costMatrix =  matrix() ;
		loadFromFile(s,_costMatrix) ;

		_wij = matrix(_costMatrix.getRow(), _costMatrix.getColumn(), 0) ;
		_uj = matrix(1, _costMatrix.getColumn(), 0) ;
		v = 0 ;

		if(unitTest(_costMatrix) == false)
			std::cout << "Unit test on file " << s << " has failed" << std::endl ;
		else
			std::cout << "Unit test on file " << s << " has succeeded" << std::endl ;
	}

};