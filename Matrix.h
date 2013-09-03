#ifndef MATRIX_H_
#define MATRIX_H_

class Matrix {
public:
	Matrix(int m=0, int n=0, double* data=0);
	Matrix(const Matrix &rhs);
	virtual ~Matrix();

	Matrix& operator=(const Matrix &rhs);
	Matrix& operator+=(const Matrix &rhs);
	Matrix& operator-=(const Matrix &rhs);
	Matrix& operator*=(const Matrix &rhs);
	Matrix& operator*=(double scalar);

	Matrix operator+(const Matrix &rhs) const;
	Matrix operator-(const Matrix &rhs) const;
	Matrix operator-() const;
	Matrix operator~() const; // inverse
	Matrix operator*(const Matrix &rhs) const;
	Matrix operator*(double scalar) const;
	double& operator()(int i, int j);

	const double& get(int i, int j) const;
	double& set(int i, int j, double v=0.0);
	int index(int i, int j) const;

	bool  operator==(const Matrix &other) const;
	bool  operator!=(const Matrix &other) const;

	void inverse();
	double trace() const;
	void transpose();

	double* data;
	int m;
	int n;
	bool external_data;
	bool transposed;
};

#endif /* MATRIX_H_ */
