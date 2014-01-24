#include <limits>

typedef double value ;

class matrix{
private:
	int row, column ;	//Number of rows and columns
	value* _matrix ;	//Matrix array

	void _resize(int n, int m){
		row = n ; 
		column = m ;
		delete _matrix ;
		_matrix = new value[row*column] ;
		for(int i = 0; i < row*column; i++) _matrix[i] = std::numeric_limits<value>::infinity() ;
	}

	int _getRow(){ return row ; }
	int _getColumn(){ return column ; }

public:
	matrix(){				//Empty Constructor
		row		= 0 ;
		column	= 0 ;
		_matrix	= NULL ;
	}

	matrix(int n, int m, int def) {	//Constructor
		row = n ; 
		column = m ;
		_matrix = new value[row*column] ;
		for(int i = 0; i < row*column; i++) _matrix[i] = def ;
	}

	matrix(matrix& that){	//Copy constructor
		row		= that.row ;
		column	= that.column ;
		_matrix	= new value[row*column] ;
		for(int i = 0; i < row*column; i++) _matrix[i] = that._matrix[i] ;
	}

	~matrix(){ delete _matrix ; }	//Destructor
	
	void	resize(int n, int m){ _resize(n,m) ; }	//Resizes the matrix
	int		getRow(){ return row ; }				//Gets the number of rows
	int		getColumn(){ return column ; }			//Gets the number of columns

	//Access operator. Accesses a_ij, where 1<=i<=n and 1<=j<=m
	value& operator()(int i, int j) { return _matrix[column*(i-1)+(j-1)] ; }
	const value& operator()(int i, int j) const { return _matrix[column*(i-1)+(j-1)] ; }

} ;