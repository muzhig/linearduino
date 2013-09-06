#ifndef MATRIX_H_
#define MATRIX_H_

class Matrix {
public:
	Matrix(int m=0, int n=0, double* data=0, bool transposed=false);
	Matrix(const Matrix &rhs);
	virtual ~Matrix();
	static Matrix identity(int m);
	Matrix& operator=(const Matrix &rhs);
	Matrix& operator+=(const Matrix &rhs);
	Matrix& operator-=(const Matrix &rhs);
	Matrix& operator*=(double scalar);
	Matrix& multiplySelf(const Matrix &rhs);
	Matrix& dotSelf(const Matrix &rhs, bool left=false);
	Matrix& normalize();
	double norm() const;
	double sum() const;

	Matrix  dot(const Matrix &rhs, bool left=false) const;
	Matrix  cross(const Matrix &rhs, bool left=false) const;
	Matrix multiply(const Matrix &rhs) const;
	Matrix operator+(const Matrix &rhs) const;
	Matrix operator-(const Matrix &rhs) const;
	Matrix operator-() const;
	Matrix operator~() const; // inverse
	Matrix operator*(double scalar) const;
	double& operator()(int i, int j=0);

	const double& get(int i, int j) const;
	double& set(int i, int j);
	int index(int i, int j) const;

	bool  operator==(const Matrix &other) const;
	bool  operator!=(const Matrix &other) const;

	Matrix&  inverse();
	double trace() const;

	Matrix transpose() const;
	void release();

	double* data;
	int m;
	int n;
	bool external_data;
	bool transposed;
};

#endif /* MATRIX_H_ */
