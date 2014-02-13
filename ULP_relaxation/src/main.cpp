
#include <iostream>
#include <sstream>

#include "CPUTimer.h"
#include "input.h"
#include "solver.h"


int main(int argc, char *argv []){

    if(argc < 2) {
        std::cerr << "Usage: ulp FILENAME" << std::endl;
        return 1;
    }

    std::string instanceFilename(argv[1]);

    matrix problem;
        
	double k = loadFromFile(instanceFilename, problem);
    if(k <= 0) {
        std::cerr << "Failure loading instance file" << std::endl;
		return 2;
    }

    unitTest(problem);

    CPUTimer timer;

    timer.start();
	std::pair<double, double> opt = binarySearch(k, problem);
	timer.stop();
	std::cout << "Final Primal solution: " << opt.first
		<< " Gap upper bound: " << opt.second
		<< " Time: " << timer.getCPUTotalSecs() << std::endl;

//	int i = 0;
//	std::cin >> i;

    return 0;
}

