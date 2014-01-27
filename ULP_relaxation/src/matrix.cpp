
#include "matrix.h"

#define NULL 0


matrix::matrix() {				//Empty Constructor
    row		= 0 ;
    column	= 0 ;
    _matrix	= NULL ;
}

matrix::matrix(int n, int m, int def) {	//Constructor
    row = n ; 
    column = m ;
    _matrix = new value[row*column] ;
    for(int i = 0; i < row*column; i++) {
        _matrix[i] = def ;
    }
}

matrix::matrix(matrix& that){	//Copy constructor
    row		= that.row ;
    column	= that.column ;
    _matrix	= new value[row*column] ;
    for(int i = 0; i < row*column; i++) _matrix[i] = that._matrix[i] ;
}

matrix::~matrix(){ delete _matrix ; }	//Destructor

// Resizes the matrix
void matrix::resize(int n, int m){
    row = n;
    column = m;

    if(_matrix != NULL) {
        delete _matrix;
    }

    _matrix = new value[row*column];
    for(int i = 0; i < row*column; i++) {
        _matrix[i] = std::numeric_limits<value>::infinity();
    }
}


//Access operator. Accesses a_ij, where 1<=i<=n and 1<=j<=m
value& matrix::operator()(int i, int j) {
    return _matrix[column*(i-1)+(j-1)];
}


const value& matrix::operator()(int i, int j) const {
    return _matrix[column*(i-1)+(j-1)];
}

