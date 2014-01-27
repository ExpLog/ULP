
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


bool loadFromFile(std::string const& s, matrix& m){
    std::ifstream file(s.c_str());
    if(file.fail()){
        return false;
    }

    char c;
    int i,j,k;

    file >> c >> i >> j >> k;	//First line is p #vertex #edges #k
    m.resize(i, i);

    while(file >> c >> i >> j >> k) {
        m(i,j) = k;
        m(j,i) = k;
    }

    file.close();

    floydWarshall(m);

    return true;
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

