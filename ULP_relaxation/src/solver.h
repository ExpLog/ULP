
#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <vector>

#include "matrix.h"


double dualAscent(double v, matrix const& cost, std::vector<bool> const& J,
        std::vector<double> &u);

#endif // SOLVER_H

