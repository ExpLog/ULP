
#include <iostream>
#include <sstream>

#include "input.h"
#include "solver.h"


int main(int argc, char *argv []){

    if(argc < 2) {
        std::cerr << "Usage: ulp FILENAME" << std::endl;
        return 1;
    }

    std::string instanceFilename(argv[1]);
	//std::string instanceFilename("pmedian-dimacs/pmed40.dimacs");

    matrix problem;
        
	double k = loadFromFile(instanceFilename, problem);
    if(k <= 0) {
        std::cerr << "Failure loading instance file" << std::endl;
		return 2;
    }

	unitTest(problem);
	
	double opt = binarySearch(k, problem);
	std::cout << "Final Primal solution: " << opt << std::endl;

    return 0;
}

