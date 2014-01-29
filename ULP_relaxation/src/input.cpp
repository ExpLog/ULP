
#include "input.h"

#include <fstream>
#include <iostream>

void floydWarshall(matrix& m) {
    int v = m.getColumn() ;

    for(int i = 1; i <= v; i++)
        m(i,i) = 0 ;

    for(int k = 1; k <= v; k++){
        for(int i = 1; i <= v; i++){
            for(int j = 1; j <= v; j++){
                if (m(i,j) > m(i,k) + m(k,j))
                    m(i,j) = m(i,k) + m(k,j) ;
            }
        }
    }
}


int loadFromFile(std::string const& s, matrix& m){
    std::ifstream file(s.c_str());
    if(file.fail()){
        return -1;
    }

    char c;
    int i,j,k,p;

    file >> c >> i >> j >> k;	//First line is p #vertex #edges #k
    m.resize(i, i);

    while(file >> c >> i >> j >> p) {
        m(i,j) = p;
        m(j,i) = p;
    }

    file.close();

    floydWarshall(m);

    return k;
}


bool unitTest(matrix &m){	//Unit test to check if every distance has been correctly calculated
    std::ofstream file("teste.txt") ;

    if(file.fail()){
        std::cout << "Output file teste.txt has failed to open" << std::endl ;
        return false ;
    }

    file.clear() ;

    bool pass = true ;
    for(int i = 1; i <= m.getRow(); i++){
        for(int j = 1; j <= m.getColumn(); j++){
            file << m(i,j) << " " ;
            if(std::numeric_limits<double>::infinity() == m(i,j))
                pass = false ;
        }
        file << std::endl ;
    }

    file.close() ;
    return pass ;
}

