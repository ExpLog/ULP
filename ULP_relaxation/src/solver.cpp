
#include "solver.h"

#include <algorithm>
#include <cassert>
#include <limits>


double dualAscent(double v, matrix const& cost, std::vector<bool> const& J,
        std::vector<double> &u)
{
    assert(u.size() == cost.getColumn());
    assert(J.size() == cost.getColumn());

    // sorting the indices
    std::vector<std::vector<std::pair<double, int> > > sorted(cost.getRow());
    for(int j = 0; j < cost.getColumn(); ++j) {
        for(int i = 0; i < cost.getRow(); ++i) {
            sorted[j].push_back(std::make_pair(cost(i+1, j+1), i+1));
        }
		// add infinity element at the end
        // sorted[j].push_back(std::make_pair(std::numeric_limits<double>::infinity(), cost.getRow()+1));
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

    // step 1 - initializing variables
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
		assert(s[i] <= v);
    }
#ifdef DEBUG
    std::cout << "s[i]: " << std::endl;
    for(int i = 0; i < s.size(); ++i) {
        std::cout << s[i] << " ";
    }
    std::cout << std::endl;
#endif

	std::vector<double> k(cost.getColumn());
	for(int j = 0; j < k.size(); ++j) {
		int inx;
		for(inx = 0; u[j] >= sorted[j][inx].first; ++inx);
		k[j] = inx; 
	}

	// step 2 - 8 -> the big for
	bool rerun = true;
	while(rerun) {
		rerun = false;
		for(int j = 0; j < J.size(); ++j) {
#ifdef DEBUG
			std::cout << "j: " << j << std::endl;
#endif
			if(J[j]) {
				int best_s = 0;
				for(int i = 0; i < s.size(); ++i) {
					if(s[i] < s[best_s] && u[j] >= cost(i+1,j+1)) {
						best_s = i;
					}
				}
				double dj = s[best_s];

				double reduced = sorted[j][k[j]].first - u[j];
				if(dj > reduced) {
#ifdef DEBUG
					std::cout << "dj: " << dj << " > reduced: " << reduced << std::endl;
#endif
					dj = reduced;
					rerun = true;
					k[j]++;
				}

				for(int i = 0; i < s.size(); ++i) {
					if(u[j] - cost(i+1,j+1) >= 0.0) {
						s[i] -= dj;
						u[j] += dj;
					}
				}
			}
		}
#ifdef DEBUG
		if(rerun) std::cout << "Rerunning" << std::endl;
#endif
	}

	// find optimal from J
    return 0.0f;
}

