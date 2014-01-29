
#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <vector>

#include "matrix.h"


double dualAscent(double v, matrix const& cost, std::vector<bool> const& J,
        std::vector<double> &u, std::vector<double> &s);

double retrieveSol(matrix const& cost, std::vector<double> const& u, std::vector<double> const& s,
		matrix &flow, std::vector<double> &facilities);

#endif // SOLVER_H

