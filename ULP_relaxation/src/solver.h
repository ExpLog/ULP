
#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <vector>

#include "matrix.h"


double dualAscent(double v, matrix const& cost, std::vector<bool> const& J,
        std::vector<double> &u, std::vector<double> &s,
		std::vector<std::vector<std::pair<double, int> > > const& sorted);

double retrieveSol(matrix const& cost, std::vector<double> const& u, std::vector<double> const& s,
		std::vector<int> &supplier, std::vector<bool> &facilities);

double enhanceSol(matrix const& cost, std::vector<int> &supplier,
std::vector<bool> &facilities, int remaining);

double openFacilities(std::vector<bool> const& facilities);

double binarySearch(double const k, matrix const& cost);

#endif // SOLVER_H

