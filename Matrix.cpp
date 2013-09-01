/*
 * Matrix.cpp
 *
 *  Created on: Aug 31, 2013
 *      Author: muzhig
 */

#include "Matrix.h"
#include <math.h>

Matrix::Matrix(int m, int n) {
	// TODO Auto-generated constructor stub
	this->m = m;
	this->n = n;
	data = new double[m * n];
	for (int i=0; i<m * n; i++)
		data[i] = 0.0;
}

Matrix::Matrix(const Matrix &rhs) {
	m = rhs.m;
	n = rhs.n;
	data = new double[m * n];
	for (int i=0; i<m * n; i++)
		data[i] = rhs.data[i];
}

Matrix::~Matrix() {
	delete[] data;
}

Matrix& Matrix::operator=(const Matrix &rhs) {
	if (this != &rhs) {
		m = rhs.m;
		n = rhs.n;
		delete[] data;
		data = new double[m * n];
		for (int i=0; i<m * n; i++)
			data[i] = rhs.data[i];
	}

	return *this;
}

const Matrix& Matrix::operator+=(const Matrix &rhs) {
	for (int i=0; i<m; i++)
		data[i] += rhs.data[i];
	return *this;
}

const Matrix& Matrix::operator-=(const Matrix &rhs) {
	for (int i=0; i<m; i++)
		data[i] -= rhs.data[i];
	return *this;
}

const Matrix& Matrix::operator*=(double scalar){
	for (int i=0; i<m * n; i++)
		data[i] *= scalar;
	return *this;
}

const Matrix Matrix::operator+(const Matrix &rhs) const {
	return Matrix(*this) += rhs;
};

const Matrix Matrix::operator-(const Matrix &rhs) const {
	return Matrix(*this) -= rhs;
}
const Matrix Matrix::operator-() const {
	return Matrix(*this) *= -1.0;
}
const Matrix Matrix::operator~() {
	return this->inverse(false);
}
const Matrix Matrix::operator*(const Matrix &other) const {
	if (n != other.m) {
		return Matrix(0, 0);
	}
	Matrix result(m, other.n);
	for (int i=0; i<m; i++)
		for (int j=0; j<other.n; j++)
			for (int k=0; k < n; k++)
				result.data[i * m + j] += data[i * m + k] * other.data[k * n + j];
	return result;
}

const Matrix Matrix::operator*(double scalar) const {
	return Matrix(*this) *= scalar;
}

bool  Matrix::operator==(const Matrix &other) {
	if (m != other.m || n != other.n)
		return false;

	for (int i=0; i<m * n; i++)
		if (data[i] != other.data[i])
			return false;
	return true;
}

bool  Matrix::operator!=(const Matrix &other) {
	return !(*this == other);
}

const Matrix Matrix::transpose() {
	Matrix result(n, m);
	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++)
			result.data[j * n + i] = data[i * m + j];
	return result;
}

const Matrix Matrix::inverse(bool selfmodify) {
	// A = input matrix AND result matrix
	// n = number of rows = number of columns in A (n x n)
	int pivrow;     // keeps track of current pivot row
	int k,i,j;      // k: overall index along diagonal; i: row index; j: col index
	int pivrows[n]; // keeps track of rows swaps to undo at end
	double tmp;      // used for finding max value and making column swaps
	Matrix& result = *this;
	if (!selfmodify) {
		result = Matrix(*this);
	}

	for (k = 0; k < n; k++)
	{
		// find pivot row, the row with biggest entry in current column
		tmp = 0;
		for (i = k; i < n; i++)
		{
			if (fabs(result.data[i*n+k]) >= tmp)
			{
				tmp = fabs(result.data[i*n+k]);
				pivrow = i;
			}
		}

		// check for singular matrix
		if (result.data[pivrow*n+k] == 0.0f)
		{
			// singular matrix
			return Matrix(0, 0);
		}

		// Execute pivot (row swap) if needed
		if (pivrow != k)
		{
			// swap row k with pivrow
			for (j = 0; j < n; j++)
			{
				tmp = result.data[k*n+j];
				result.data[k*n+j] = result.data[pivrow*n+j];
				result.data[pivrow*n+j] = tmp;
			}
		}
		pivrows[k] = pivrow;    // record row swap (even if no swap happened)

		tmp = 1.0f/result.data[k*n+k];    // invert pivot element
		result.data[k*n+k] = 1.0f;        // This element of input matrix becomes result matrix

		// Perform row reduction (divide every element by pivot)
		for (j = 0; j < n; j++)
		{
			result.data[k*n+j] = result.data[k*n+j]*tmp;
		}

		// Now eliminate all other entries in this column
		for (i = 0; i < n; i++)
		{
			if (i != k)
			{
				tmp = result.data[i*n+k];
				result.data[i*n+k] = 0.0f;  // The other place where in matrix becomes result mat
				for (j = 0; j < n; j++)
				{
					result.data[i*n+j] = result.data[i*n+j] - result.data[k*n+j]*tmp;
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
				tmp = result.data[i*n+k];
				result.data[i*n+k] = result.data[i*n+pivrows[k]];
				result.data[i*n+pivrows[k]] = tmp;
			}
		}
	}
	return result;
}

const Matrix Matrix::determinant() {

}
double Matrix::trace() {
	double result = 0.0;
	for (int i=0; i<m && i<n; i++) {
		result += data[i*n + i];
	}
	return result;
}

