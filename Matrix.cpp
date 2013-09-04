#include "Matrix.h"
#include <math.h>

Matrix::Matrix(int m, int n, double* data) {
	transposed = false;
	this->m = m;
	this->n = n;
	if (data) {
		this->data = data;
		external_data = true;
	} else {
		this->data = new double[m * n];
		external_data = false;
		for (int i=0; i<m * n; i++) {
			this->data[i] = 0.0;
		}
	}
}

Matrix::Matrix(const Matrix &rhs) {
	transposed = rhs.transposed;
	m = rhs.m;
	n = rhs.n;
	data = new double[m * n];
	external_data = false;
	for (int i=0; i<m * n; i++)
		data[i] = rhs.data[i];
}

Matrix::~Matrix() {
	if (!external_data) {
		delete[] data;
	}
}

Matrix& Matrix::operator=(const Matrix &rhs) {
	if (this != &rhs) {
		if (m * n != rhs.m * rhs.n) {
			if(!external_data) {
				delete[] data;
			}
			data = new double[rhs.m * rhs.n];
			if (external_data)
				external_data = false;
		}
		m = rhs.m;
		n = rhs.n;
		transposed = rhs.transposed;
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

Matrix& Matrix::dotself(const Matrix &b, bool left){
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
	if (transposed)
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


void Matrix::transpose() { // extremely optimal
	int tmp = n; n = m; m = tmp;
	transposed = !transposed;
}

void Matrix::inverse() {
	// A = input matrix AND result matrix
	// n = number of rows = number of columns in A (n x n)
	int pivrow;     // keeps track of current pivot row
	int k,i,j;      // k: overall index along diagonal; i: row index; j: col index
	int* pivrows = new int[n]; // keeps track of rows swaps to undo at end
	double tmp;      // used for finding max value and making column swaps
	Matrix& result = *this;

	for (k = 0; k < n; k++)
	{
		// find pivot row, the row with biggest entry in current column
		tmp = 0;
		for (i = k; i < n; i++)
		{
			if (fabs(result.get(i, k)) >= tmp)
			{
				tmp = fabs(result.get(i, k));
				pivrow = i;
			}
		}

		// check for singular matrix
		if (result.get(pivrow, k) == 0.0f)
		{
			// singular matrix
			return;
		}

		// Execute pivot (row swap) if needed
		if (pivrow != k)
		{
			// swap row k with pivrow
			for (j = 0; j < n; j++)
			{
				tmp = result.get(k, j);
				result.set(k,j) = result.get(pivrow, j);
				result.set(pivrow, j) = tmp;
			}
		}
		pivrows[k] = pivrow;    // record row swap (even if no swap happened)

		tmp = 1.0f/result.get(k, k);    // invert pivot element
		result.set(k, k) = 1.0f;        // This element of input matrix becomes result matrix

		// Perform row reduction (divide every element by pivot)
		for (j = 0; j < n; j++)
		{
			result.set(k,j) *= tmp;
		}

		// Now eliminate all other entries in this column
		for (i = 0; i < n; i++)
		{
			if (i != k)
			{
				tmp = result.get(i,k);
				result.set(i,k) = 0.0f;  // The other place where in matrix becomes result mat
				for (j = 0; j < n; j++)
				{
					result.set(i, j) -= result.get(k,j)*tmp;
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
				tmp = result.get(i, k);
				result.set(i, k) = result.get(i, pivrows[k]);
				result.set(i, pivrows[k]) = tmp;
			}
		}
	}
	delete[] pivrows;
}

double Matrix::trace() const {
	double result = 0.0;
	for (int i=0; i<m && i<n; i++) {
		result += get(i, i);
	}
	return result;
}

