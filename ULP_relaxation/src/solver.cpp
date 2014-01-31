
#include "solver.h"

#include <algorithm>
#include <cassert>
#include <limits>

double dualAscent(double v, matrix const& cost, std::vector<bool> const& J, 
	std::vector<double> &u, std::vector<double> &s,
	std::vector<std::vector<std::pair<double, int> > > const& sorted)
{
	assert(u.size() == cost.getColumn());
	assert(J.size() == cost.getColumn());
	assert(s.size() == cost.getRow());

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

#ifdef DEBUG
	std::cout << "k[j]: [" ;
	for(int j = 0; j < k.size(); ++j){
		std::cout << k[j] << ", ";
	}
	std::cout << "]" << std::endl;
#endif

	// step 2 - 8 -> the big for
	bool rerun = true;
	while(rerun) {
		rerun = false;
		for(int j = 0; j < J.size(); ++j) {
#ifdef DEBUG
			std::cout << "j: " << j << std::endl;
#endif
			if(J[j]) {
				int best_s = j;
				for(int i = 0; i < s.size(); ++i) {
					if(s[i] < s[best_s] && u[j] >= cost(i+1,j+1)) {
						best_s = i;
					}
				}
				double dj = s[best_s];

				assert(j < sorted.size());
				assert(k[j] < sorted[j].size());
				double reduced = sorted[j][k[j]].first - u[j];

#ifdef DEBUG
				std::cout << "dj: " << dj << " > reduced: " << reduced << std::endl;
#endif
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
	std::vector<int> &supplier, std::vector<bool> &facilities)
{
	assert( supplier.size() == cost.getColumn());
	assert( facilities.size() == cost.getRow() );
	assert( u.size() == cost.getColumn() );
	assert( s.size() == cost.getRow() );

	//retrieve yi == facilities
	int openFac = -1;
	for(int i = 0; i < facilities.size(); ++i){
		if( s[i] == 0 ){
			facilities[i] = true;
			openFac = i;	//this should be in the 0 to n-1 range
		}
		else
			facilities[i] = false;
	}
	assert(openFac != -1);

	//retrieve xij == flow
	for(int j = 0; j < supplier.size(); ++j){
		int bestCost = openFac+1;	//best cost values range from 1 to n
		for(int i = 0; i < facilities.size(); ++i){
			if( facilities[i] && cost(i+1,j+1) < cost(bestCost,j+1) ) {
				bestCost = i+1;
			}
		}
		supplier[j] = bestCost;
	}

	double opt = 0.0f;
	for(int j = 0; j < supplier.size(); ++j){
		opt += cost(supplier[j], j+1);
	}

	return opt;
}


double openFacilities(std::vector<bool> const& facilities)
{
	double numFac = 0;
	for(int i = 0; i < facilities.size(); ++i){
		numFac += facilities[i] ? 1 : 0;
	}

	return numFac;
}

double enhanceSol(matrix const& cost, std::vector<int> &supplier, std::vector<bool> &facilities, int remaining)
{
	assert(supplier.size() == cost.getColumn());
	assert(facilities.size() == cost.getRow());

	int openFac = -1;

	//calculates which facilities would grant the best savings
	std::vector<std::pair<double,int> > bestCol; 
	for(int i = 0; i < cost.getRow(); ++i) {
		if(!facilities[i]) {
			double savings = 0.0;
			for(int j = 0; j < cost.getColumn(); ++j) {
				double delta = cost(supplier[j], j+1) - cost(i+1,j+1);
				savings += std::max(0.0, delta);
			}
			bestCol.push_back(std::make_pair(savings, i));
		}
	}
	std::sort(bestCol.begin(), bestCol.end());

	//gets the ones with the highest savings
	for(int i = bestCol.size(); bestCol.size() - i < remaining; --i){
		printf("opening a facility at %d to save %f\n", bestCol[i-1].second, bestCol[i-1].first);
		facilities[ bestCol[i-1].second ] = 1;
		openFac = bestCol[i-1].second;
	}

	//recalculates the suppliers
	for(int j = 0; j < supplier.size(); ++j){
		int bestCost = openFac;
		for(int i = 0; i < facilities.size(); ++i){
			if( facilities[i] == 1 && cost(i+1,j+1) < cost(bestCost,j+1) )
				bestCost = i+1;
		}
		supplier[j] = bestCost;
	}

	double opt = 0.0f;
	for(int j = 0; j < supplier.size(); ++j){
		opt += cost(supplier[j], j+1);
	}

	return opt;
}

double binarySearch(double const k, matrix const& cost)
{
	double opened = -1;
	double opt;
	double low = 0, high = 0, mid = 0;

	//finding the maximum cost
	for(int i = 1; i <= cost.getRow(); ++i){
		for(int j = 1; j <= cost.getColumn(); ++j){
			if( cost(i,j) > high )
				high = cost(i,j);
		}
	}
	high = high*cost.getColumn();	//if v == high, then only one facility will be opened

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

	//this is a pain when debugging the rest of the stuff
//#ifdef DEBUG
//	std::cout << "Sorted indices:" << std::endl;
//	for(int j = 0; j < sorted.size(); ++j) {
//		for(int i = 0; i < sorted[j].size(); ++i) {
//			std::cout << "(" << sorted[j][i].first << "," << sorted[j][i].second << ") ";
//		}
//		std::cout << std::endl;
//	}
//#endif

	unsigned bestK = 0;
	std::vector<bool> bestFacilities(cost.getRow());
	std::vector<int> bestSupplier(cost.getColumn());

	//this is the binary search on v
	while( opened != k )
	{
		mid = (low+high)/2;
		std::cout << "beginning: low: " << low << " mid: " << mid << " high: " << high << std::endl;

		std::vector<bool> J(cost.getRow(), true);
		std::vector<double> u(cost.getColumn(), 0.0f);
		std::vector<double> s(cost.getRow(), 0.0f);

		opt = dualAscent(mid, cost, J, u, s, sorted); 

		std::cout << "Dual optimal: " << opt - mid*k << std::endl;	//subtracted -mid*k so that this is the actual dual value for k-medians

		std::vector<int> supplier(cost.getColumn());
		std::vector<bool> facilities(cost.getRow());

		opt = retrieveSol(cost, u, s, supplier, facilities);
		opened = openFacilities(facilities);

		std::cout << "Primal optimal: " << opt  << std::endl;
		std::cout << opened << " facilities opened." << std::endl;
		std::cout << k << " facilities needed." << std::endl;

		if(opened > bestK && opened <= k) {
			bestK = opened;
			bestFacilities = facilities;
			bestSupplier = supplier;
		}

		if( opened <= k ){
			high = mid;
		}
		else{
			low = mid;
		}

		std::cout.setf(std::ios::fixed, std:: ios::floatfield);
		std::cout.precision(7);
		std::cout << "end: low: " << low << " mid: " << mid << " high: " << high << std::endl << std::endl;

		if( high-low < 1e-5 ){
			//enhancing solution
			std::cout << "Starting enhancement procedure." << std::endl;
			

			opened = bestK;
			while(k > opened) {
				opt = enhanceSol(cost, bestSupplier, bestFacilities, 1 /*k - opened*/);
				opened = openFacilities(bestFacilities);
			}

			std::cout << "Primal optimal: " << opt  << std::endl;
			std::cout << opened << " facilities opened." << std::endl;
			std::cout << k << " facilities needed." << std::endl;
		}
	}

	return opt;
}

