#ifndef MATRIX_H_
#define MATRIX_H_

class Matrix {
public:
	Matrix(int m, int n);
	Matrix(const Matrix &rhs);
	virtual ~Matrix();

	Matrix& operator=(const Matrix &rhs);
	const Matrix& operator+=(const Matrix &rhs);
	const Matrix& operator-=(const Matrix &rhs);
	const Matrix& operator*=(double scalar);

	const Matrix operator+(const Matrix &rhs) const;
	const Matrix operator-(const Matrix &rhs) const;
	const Matrix operator-() const;
	const Matrix operator~(); // inverse
	const Matrix operator*(const Matrix &rhs) const;
	const Matrix operator*(double scalar) const;

	bool  operator==(const Matrix &other);
	bool  operator!=(const Matrix &other);

	const Matrix inverse(bool selfmodify=false);
	const Matrix determinant();
	double trace();
	const Matrix transpose();

	double* data;
	int m;
	int n;
};

#endif /* MATRIX_H_ */
