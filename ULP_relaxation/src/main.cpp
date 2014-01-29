
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
		return 2;
    }
	
    std::vector<bool> J(problem.getRow(), true);
    std::vector<double> u(problem.getColumn(), 0.0f);
	std::vector<double> s(problem.getRow(), 0.0f);

    double opt = dualAscent(1000, problem, J, u, s); 

    std::cout << "Dual optimal: " << opt << std::endl;
	
#ifdef DEBUG
	std::cout << "s:"; 
	for(int i = 0; i < s.size(); ++i) {
		std::cout << " " << s[i];
	}
	std::cout << std::endl << std::endl;

	std::cout << "u:"; 
	for(int j = 0; j < u.size(); ++j) {
		std::cout << " " << u[j];
	}
	std::cout << std::endl;
#endif

	matrix flow(problem.getRow(), problem.getColumn());
	std::vector<double> facilities(problem.getRow());
	std::vector<double> y(problem.getRow());

	opt = retrieveSol(problem, u, s, flow, facilities);

	std::cout << "Primal optimal: " << opt << std::endl;

	int i;
	std::cin >> i;

    return 0;
}

