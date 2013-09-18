#include "Matrix.h"
#include <math.h>

Matrix::Matrix(int m, int n, double* data, bool transposed) {
	this->isTransposed = transposed;
	this->m = m;
	this->n = n;
	this->data = 0;
	if (data) {
		this->data = data;
		externalData = true;
	} else {
		allocate();
		for (int i=0; i<m * n; i++) {
			this->data[i] = 0.0;
		}
	}
}

Matrix::Matrix(const Matrix &rhs) {
	isTransposed = rhs.isTransposed;
	m = rhs.m;
	n = rhs.n;
	data = 0;
	allocate();
	for (int i=0; i<m * n; i++)
		data[i] = rhs.data[i];
}

Matrix::~Matrix() {
	release();
}

Matrix& Matrix::operator=(const Matrix &rhs) {
	if (this != &rhs) {
		if (m * n != rhs.m * rhs.n) {
			m = rhs.m;
			n = rhs.n;
			allocate();
		}
		m = rhs.m;
		n = rhs.n;
		isTransposed = rhs.isTransposed;
		for (int i=0; i < m*n; i++)
			data[i] = rhs.data[i];
	}

	return *this;
}

Matrix& Matrix::operator+=(const Matrix &rhs) {
	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++)
			set(i, j) += rhs.get(i, j);
	return *this;
}

Matrix& Matrix::operator-=(const Matrix &rhs) {
	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++)
			set(i, j) -= rhs.get(i, j);
	return *this;
}

Matrix& Matrix::multiplySelf(const Matrix &rhs) {
	// element-wise multiplication with self-modification
	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++)
			set(i, j) *= rhs.get(i, j);
	return *this;
}
Matrix Matrix::multiply(const Matrix &rhs) const{
	// element-wise multiplication
	return Matrix(*this).multiplySelf(rhs);
}

Matrix& Matrix::dotSelf(const Matrix &b, bool left){
	Matrix& a = *this;

	if (( left ? a.m : a.n ) == ( left ? b.n : b.m )) {
		if (( left ? b.m : b.n ) == ( left ? a.m : a.n )) { // very memory-effective. using only n extra floats.
			if (left) {
				a.transpose();
			}
			double* row = new double[a.n];

			for (int i=0; i<a.m; i++) {
				for (int jj=0; jj<a.n; jj++) {
					row[jj] = a.get(i, jj);
					a(i, jj) = 0.0;
				}
				for (int j=0; j<( left ? b.m : b.n ); j++)
					for (int k=0; k < a.n; k++)
						a(i, j) += row[k] * ( left ? b.get(j, k) : b.get(k, j));
			}
			delete[] row;
			if (left) {
				transpose();
			}
		} else {
			*this = a.dot(b, left);
		}
	}

	return *this;
}

Matrix Matrix::dot(const Matrix &other, bool left) const {
	if ((left && m != other.n) || (!left && n != other.m)) {
		return Matrix(0, 0);
	}
	Matrix result(left ? n : m, left ? other.m : other.n);
	for (int i=0; i < (left ? n : m); i++)
		for (int j=0; j < (left ? other.m : other.n); j++)
			for (int k=0; k < (left ? m : n); k++)
				result(i, j) += (left ? get(k, i) : get(i, k)) * (left ? other.get(j, k) : other.get(k, j));
	if(left) {
		result.transpose();
	}
	return result;
}

Matrix& Matrix::operator*=(double scalar){
	for (int i=0; i<m * n; i++)
		data[i] *= scalar;
	return *this;
}



Matrix Matrix::operator+(const Matrix &rhs) const{
	return Matrix(*this) += rhs;
};

Matrix Matrix::operator-(const Matrix &rhs) const{
	return Matrix(*this) -= rhs;
}
Matrix Matrix::operator-() const{
	return Matrix(*this) *= -1.0;
}
Matrix Matrix::operator~() const{
	Matrix result(*this);
	result.inverse();
	return result;
}


Matrix Matrix::operator*(double scalar) const {
	return Matrix(*this) *= scalar;
}

double& Matrix::operator()(int i, int j){
	return set(i, j);
}

const double& Matrix::get(int i, int j) const{
	return data[index(i, j)];
}

double& Matrix::set(int i, int j) {
	return data[index(i, j)];
}
int Matrix::index(int i, int j) const{
	if (isTransposed)
		return j * m + i;
	else
		return i * n + j;
}


bool  Matrix::operator==(const Matrix &other) const{
	if (m != other.m || n != other.n)
		return false;

	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++)
			if (get(i, j) != other.get(i,j))
				return false;
	return true;
}

bool  Matrix::operator!=(const Matrix &other) const{
	return !(*this == other);
}


Matrix& Matrix::transpose() {
	isTransposed = !isTransposed;
	int tmp = m; m = n; n=tmp;
	return *this;
}

Matrix Matrix::transposed() const { // extremely memory-optimal but be aware- data reference is copied! Do not use transposed after parent deletion.
	return Matrix(n,m,data,!isTransposed);
}

Matrix& Matrix::inverse() {
    int pivrow;     // keeps track of current pivot row
    int k,i,j;      // k: overall index along diagonal; i: row index; j: col index
    int* pivrows = new int[n]; // keeps track of rows swaps to undo at end
    double tmp;      // used for finding max value and making column swaps

    for (k = 0; k < n; k++)
    {
        // find pivot row, the row with biggest entry in current column
        tmp = 0;
        for (i = k; i < n; i++)
        {
            if (fabs(get(i,k)) >= tmp)   // 'Avoid using other functions inside abs()?'
            {
                tmp = fabs(get(i,k));
                pivrow = i;
            }
        }

        // check for singular matrix
        if (get(pivrow,k) == 0.0)
        {
        	delete[] pivrows;
        	this->release();
            return *this;
        }

        // Execute pivot (row swap) if needed
        if (pivrow != k)
        {
            // swap row k with pivrow
            for (j = 0; j < n; j++)
            {
                tmp = get(k, j);
                set(k, j) = get(pivrow,j);
                set(pivrow, j) = tmp;
            }
        }
        pivrows[k] = pivrow;    // record row swap (even if no swap happened)

        tmp = 1.0/get(k,k);    // invert pivot element
        set(k,k) = 1.0;        // This element of input matrix becomes result matrix

        // Perform row reduction (divide every element by pivot)
        for (j = 0; j < n; j++)
        {
            set(k,j) *= tmp;
        }

        // Now eliminate all other entries in this column
        for (i = 0; i < n; i++)
        {
            if (i != k)
            {
                tmp = get(i,k);
                set(i,k) = 0.0;  // The other place where in matrix becomes result mat
                for (j = 0; j < n; j++)
                {
                    set(i,j) -= get(k,j) * tmp;
                }
            }
        }
    }

    // Done, now need to undo pivot row swaps by doing column swaps in reverse order
    for (k = n-1; k >= 0; k--)
    {
        if (pivrows[k] != k)
        {
            for (i = 0; i < n; i++)
            {
                tmp = get(i,k);
                set(i, k) = get(i,pivrows[k]);
                set(i,pivrows[k]) = tmp;
            }
        }
    }
    delete[] pivrows;
    return *this;
}

double Matrix::trace() const {
	double result = 0.0;
	for (int i=0; i<m && i<n; i++) {
		result += get(i, i);
	}
	return result;
}

Matrix Matrix::cross(const Matrix& rhs, bool left) const {
	if (m!=1 || rhs.m!=1 || n!=3 || rhs.n!=3) // for row vectors only
		return Matrix(); //empty
	const Matrix& u = left ? rhs : *this;
	const Matrix& v = left ? *this : rhs;

	Matrix result(1,3);
	result(0,0) = u.get(0,1) * v.get(0,2) - u.get(0,2) * v.get(0,1);
	result(0,1) = u.get(0,2) * v.get(0,0) - u.get(0,0) * v.get(0,2);
	result(0,2) = u.get(0,0) * v.get(0,1) - u.get(0,1) * v.get(0,0);
	return result;
}

Matrix Matrix::quaternion_multiply(const Matrix& rhs, bool left) const {
	if (m!=1 || rhs.m!=1 || (n!=4 && n!=3) || (rhs.n!=4 && rhs.n!=3)) // for row vectors only
		return Matrix(); //empty
	const Matrix& v = left ? rhs : *this;
	const Matrix& u = left ? *this : rhs;

	double w0,x0,y0,z0;
	if (u.n==4) {
		w0 = u.get(0,0);
		x0 = u.get(0,1);
		y0 = u.get(0,2);
		z0 = u.get(0,3);
	} else {
		w0 = 0;
		x0 = u.get(0,0);
		y0 = u.get(0,1);
		z0 = u.get(0,2);
	}

	double w1,x1,y1,z1;
	if (v.n==4) {
		w1 = v.get(0,0);
		x1 = v.get(0,1);
		y1 = v.get(0,2);
		z1 = v.get(0,3);
	} else {
		w1 = 0;
		x1 = v.get(0,0);
		y1 = v.get(0,1);
		z1 = v.get(0,2);
	}

	double res_[4] = {
		-x1*x0 - y1*y0 - z1*z0 + w1*w0,
		 x1*w0 + y1*z0 - z1*y0 + w1*x0,
		-x1*z0 + y1*w0 + z1*x0 + w1*y0,
		 x1*y0 - y1*x0 + z1*w0 + w1*z0
	};
	Matrix result(1,4,res_);
	return result;
}

Matrix Matrix::quaternion_inverse() const {
	if (m!=1 || n!=4) // for row vectors only
		return Matrix(); //empty

	double w0 = get(0,0);
	double x0 = get(0,1);
	double y0 = get(0,2);
	double z0 = get(0,3);

	double sqr_norm_inv = 1.0/(w0*w0 + x0*x0 + y0*y0 + z0*z0);

	double res_[4] = {
		 w0 * sqr_norm_inv,
		-x0 * sqr_norm_inv,
		-y0 * sqr_norm_inv,
		-z0 * sqr_norm_inv
	};
	Matrix result(1,4,res_);
	return result;
}

Matrix Matrix::quaternion_rotate(Matrix& Q) const {
	if (m!=1 || n!=3|| Q.m!=1 || Q.n!=4) // for row vectors only
		return Matrix();
		
	
	Matrix tmp = Q.quaternion_multiply(*this).quaternion_multiply(Q.quaternion_inverse());
	
	/*
	def rotate_vector(v, q):
		v = np.hstack(([0.0], v))
		v = tr.quaternion_multiply(q, v)
		q = tr.quaternion_inverse(q)
		v = tr.quaternion_multiply(v, q)
		return v[1:]
	*/
	return tmp.submatrix(0,1,0,3);
}

Matrix& Matrix::normalize() {
	double k = 1 / norm();
	*this *= k;
	return *this;
}

double Matrix::norm() const {
	double result = 0.0;
	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++)
			result += get(i, j)*get(i, j);
	result = sqrt(result);
	return result;
}

double Matrix::sum() const {
	double result = 0.0;
	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++)
			result += get(i, j);
	return result;
}

Matrix Matrix::identity(int m) {
	Matrix result(m,m);
	for(int i=0; i<m; i++) {
		result(i,i) = 1.0;
	}
	return result;
}

void Matrix::release() {
	if (data && !externalData) {
		delete[] data;
		data = 0;
	}
}
void Matrix::allocate() {
	if (data) {
		release();
	}
	if (m && n)
		this->data = new double[m * n];
	else
		this->data = 0;
	externalData = false;
}

Matrix Matrix::submatrix(int row_top, int col_left, int row_bottom, int col_right) {
	int rows = row_bottom-row_top+1;
	int cols = col_right-col_left+1;
	Matrix result(rows, cols);

	for(int i=0;i<rows; i++)
		for(int j=0; j<cols; j++)
			result(i,j) = get(row_top+i, col_left+j);
	return result;
}

bool Matrix::closeEnough(const Matrix& another) {
	if (another.m !=m || another.n != n) {
		return false;
	}
	bool result = true;

	for (int i=0; result && i<m; i++)
	{
		for(int j=0; j<n; j++){
			if (fabs(get(i,j)-another.get(i,j)) > 1.0e-6) {
				result = false;
				break;
			}
		}
	}
	return result;
}
