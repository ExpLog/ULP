
#include "solver.h"

#include <algorithm>
#include <cassert>


double dualAscent(double v, matrix const& cost, std::vector<bool> const& J,
        std::vector<double> &u)
{
    assert(u.size() == cost.getColumn());

    // sorting the indices
    std::vector<std::vector<std::pair<double, int> > > sorted(cost.getRow());
    for(int j = 0; j < cost.getColumn(); ++j) {
        for(int i = 0; i < cost.getRow(); ++i) {
            sorted[j].push_back(std::make_pair(cost(i+1, j+1), i+1));
        }
        std::sort(sorted[j].begin(), sorted[j].end());
    }
#ifdef DEBUG
    std::cout << "Sorted indices:" << std::endl;
    for(int j = 0; j < sorted.size(); ++j) {
        for(int i = 0; i < sorted[j].size(); ++i) {
            std::cout << "(" << sorted[j][i].first << "," << sorted[j][i].second << ") ";
        }
        std::cout << std::endl;
    }
#endif

    // step 1
    for(int j = 0; j < u.size(); ++j) {
        u[j] = std::max(u[j], sorted[j][0].first);
    }
#ifdef DEBUG
    std::cout << "u[j]: " << std::endl;
    for(int j = 0; j < u.size(); ++j) {
        std::cout << u[j] << " ";
    }
    std::cout << std::endl;
#endif

    std::vector<double> s(cost.getRow());
    for(int i = 0; i < s.size(); ++i) {
        double sum = 0.0;
        for(int j = 0; j < cost.getColumn(); ++j) {
            sum += std::max(0.0, u[j] - cost(i+1, j+1));
        }
        
        s[i] = v - sum;
    }
#ifdef DEBUG
    std::cout << "s[i]: " << std::endl;
    for(int i = 0; i < s.size(); ++i) {
        std::cout << s[i] << " ";
    }
    std::cout << std::endl;
#endif

    return 0.0f;
}

