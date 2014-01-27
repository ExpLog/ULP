
#ifndef MATRIX_H
#define MATRIX_H

#include <limits>


typedef double value;

class matrix{
private:
	int row, column ;	//Number of rows and columns
	value* _matrix ;	//Matrix array

public:
	matrix();				//Empty Constructor
	matrix(int n, int m, int def=0);
	matrix(matrix& that);
	~matrix();	//Destructor
	
	void resize(int n, int m); //Resizes the matrix
	int getRow() const { return row ; }				//Gets the number of rows
	int getColumn() const { return column ; }			//Gets the number of columns

	//Access operator. Accesses a_ij, where 1<=i<=n and 1<=j<=m
	value& operator()(int i, int j);
	const value& operator()(int i, int j) const;

} ;

#endif // MATRIX_H

