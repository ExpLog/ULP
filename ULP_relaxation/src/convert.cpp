
#include <iostream>
#include <string>

#include <cstdio>

#include "input.h"


int main(int argc, char* argv[])
{
    if(argc < 2) {
        std::cerr << "Usage: convert FILENAME" << std::endl;
        return 1;
    }

    std::string instanceFilename(argv[1]);

    matrix problem;
	double k = loadFromFile(instanceFilename, problem);
    if(k <= 0) {
        std::cerr << "Failure loading instance file" << std::endl;
		return 2;
    }

    printf("Minimize\n");
    for(int i = 0; i < problem.getRow(); ++i) {
        for(int j = 0; j < problem.getColumn(); ++j) {
            printf("%s%lf x%d_%d ", i || j ? "+ " : "", problem(i+1,j+1), i+1, j+1);
        }
    }
    printf("\n");

    printf("\nSubject To\n");
    int c = 0;
    for(int j = 0; j < problem.getRow(); ++j) {
        printf(" c%d:", ++c);
        for(int i = 0; i < problem.getColumn(); ++i) {
           printf("%s x%d_%d", i ? " +" : "", i+1, j+1); 
        }
        printf(" = 1\n");
    }
    for(int j = 0; j < problem.getRow(); ++j) {
        for(int i = 0; i < problem.getColumn(); ++i) {
           printf(" c%d: y%d - x%d_%d >= 0\n", ++c, i+1, i+1, j+1); 
        }
    }
    printf(" c%d:", ++c);
    for(int i = 0; i < problem.getRow(); ++i) {
        printf(" %sy%d", i ? "+ " : "", i+1);
    }
    printf(" = %d\n", (int)k);

    printf("\nBounds\n");
    for(int i = 0; i < problem.getRow(); ++i) {
        for(int j = 0; j < problem.getColumn(); ++j) {
            printf("0 <= x%d_%d <= 1\n", i+1, j+1);
        }
    }

    printf("\nBinary\n");
    for(int i = 0; i < problem.getColumn(); ++i) {
        printf("y%d\n", i+1);
    }

    printf("\nEnd\n");

    return 0;
}
