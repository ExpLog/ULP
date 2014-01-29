
#include "solver.h"

#include <algorithm>
#include <cassert>
#include <limits>

double dualAscent(double v, matrix const& cost, std::vector<bool> const& J, 
		std::vector<double> &u, std::vector<double> &s)
{
    assert(u.size() == cost.getColumn());
    assert(J.size() == cost.getColumn());
	assert(s.size() == cost.getRow());

    // sorting the indices
    std::vector<std::vector<std::pair<double, int> > > sorted(cost.getRow());
    for(int j = 0; j < cost.getColumn(); ++j) {
        for(int i = 0; i < cost.getRow(); ++i) {
            sorted[j].push_back(std::make_pair(cost(i+1, j+1), i+1));
        }
		// add infinity element at the end
        sorted[j].push_back(std::make_pair(std::numeric_limits<double>::infinity(), cost.getRow()+1));
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

    for(int i = 0; i < s.size(); ++i) {
        double sum = 0.0;
        for(int j = 0; j < cost.getColumn(); ++j) {
            sum += std::max(0.0, u[j] - cost(i+1, j+1));
        }
        
        s[i] = v - sum;
		//assert(s[i] <= v && s[i] >= 0);
    }
#ifdef DEBUG
    std::cout << "s[i]: " << std::endl;
    for(int i = 0; i < s.size(); ++i) {
        std::cout << s[i] << " ";
    }
    std::cout << std::endl;
#endif

	std::vector<int> k(cost.getColumn());
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

				assert(j < sorted.size());
				assert(k[j] < sorted[j].size());
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
						//u[j] += dj;	We were adding to u[j] all the time. Clearly, this is wrong.
					}
				}
				
				u[j] += dj;
			}
		}
#ifdef DEBUG
		if(rerun) std::cout << "Rerunning" << std::endl;
#endif
	}

#ifdef DEBUG
	std::cout << "s[i] = [" ;
	for(int i = 0; i < s.size(); i++){
			std::cout << s[i] << ", ";
		}
	std::cout << "]" << std::endl;
#endif

	//find zp from J
	double opt = 0.0f ;
	for(int j = 0; j < u.size(); ++j) opt+= u[j];
    return opt;
}

//retrieves the primal solution. assumes that matrix flow has every entry equal to 0.
double retrieveSol(matrix const& cost, std::vector<double> const& u, std::vector<double> const& s,
		matrix &flow, std::vector<double> &facilities)
{
	assert( flow.getRow == cost.getRow && flow.getColumn == cost.getColumn );
	assert( u.size() == cost.getColumn );
	assert( s.size() == cost.getRow );
	assert( facilities.size() == cost.getRow );

	//retrieve yi == facilities
	int openFac = -1;
	for(int i = 0; i < facilities.size(); ++i){
		if( s[i] == 0 ){
			facilities[i] = 1;
			openFac = i;
		}
		else
			facilities[i] = 0;
	}
	assert(openFac != -1);

	//retrieve xij == flow
	for(int j = 0; j < flow.getColumn(); ++j){
		int bestCost = openFac;
		for(int i = 0; i < flow.getRow(); ++i){
			if( facilities[i] == 1 && cost(i+1,j+1) < cost(bestCost,j+1) )
				bestCost = i+1;
		}
		flow(bestCost,j+1) = 1;
	}

	double opt = 0;
	for(int i = 1; i <= flow.getRow(); ++i){
		for(int j = 1; j <= flow.getColumn(); ++j){
			opt += flow(i,j)*cost(i,j);
		}
	}

	return opt;
}