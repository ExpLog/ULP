
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

    matrix problem;
        
    if(!loadFromFile(instanceFilename, problem)) {
        std::cerr << "Failure loading instance file" << std::endl;
		return 1;
    }

    std::vector<bool> J(problem.getRow(), true);
    std::vector<double> u(problem.getRow(), 0.0f);

    double opt = dualAscent(5, problem, J, u); 

    std::cout << "Optimal: " << opt << std::endl;
	
	std::cout << "u:"; 
	for(int i = 0; i < u.size(); ++i) {
		std::cout << " " << u[i];
	}
	std::cout << std::endl;

    return 0;
}

