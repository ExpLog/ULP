
#ifndef INPUT_H
#define INPUT_H

#include <string>

#include "matrix.h"


// Run of the mill Floyd-Warshall algorithm
void floydWarshall(matrix& m);

int loadFromFile(std::string const& s, matrix& m);

// Unit test to check if every distance has been correctly calculated
bool unitTest(matrix& m);

#endif // INPUT_H

