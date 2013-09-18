#include <iostream>
#include "Matrix.h"
#include <math.h>
void mprint(const Matrix& r) {
	std::cout << "\n" << r.m << "x" << r.n << "\n";
	for(int i=0; i<r.m; i++) {
		for (int j=0; j<r.n; j++)
			std::cout << r.get(i, j) << "\t";
		std::cout << "\n";
	}
}

void test_dotSelf1() {
	double m_[] = {-679.0, -1282.0, -937.0, 1.0};
	Matrix m = Matrix(4, 1, m_);

	double GYRO_[] = {
					3.05008235e-05,  0.00000000e+00,  0.00000000e+00,  0.00000000e+00,
					0.00000000e+00, -3.05008235e-05,  0.00000000e+00,  0.00000000e+00,
					0.00000000e+00,  0.00000000e+00, -3.05008235e-05,  0.00000000e+00,
					2.55507381e-02, -4.43037425e-02, -1.58011956e-02,  3.05008235e-05
				};
	Matrix G(4, 4, GYRO_, true);

	Matrix& r = m.dotSelf(G, true);

	double rv_[] = {0.00484068, -0.00520169, 0.0127781, 3.05008e-05};
	Matrix rv = Matrix(4, 1, rv_);

	std::cout << "test_dotSelf1: ";
	if (r.closeEnough(rv))
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(rv);
	}

}


void test_dot1() {
	double m_[] = {1,2,3,4,5,6,7,8,9};
	Matrix m = Matrix(3, 3, m_);

	double v_[] = {1,2,3};
	Matrix v = Matrix(3, 1, v_);

	Matrix r = m.dot(v);

	double rv_[] = {14,32,50};
	Matrix rv = Matrix(3, 1, rv_);

	std::cout << "test_dot1: ";
	if (r == rv)
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(rv);
	}

}


void test_transpose() {
	double m_[] = {1,2,3,4,5,6,7,8,9};
	Matrix m = Matrix(3, 3, m_);
	m.transpose();


	double rv_[] = {1,4,7,2,5,8,3,6,9};
	Matrix rv = Matrix(3, 3, rv_);

	std::cout << "test_transpose: ";
	if (m == rv)
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(m);
		mprint(rv);
	}

}

void test_trace() {
	double m_[] = {1,2,3,4,5,6,7,8,9};
	Matrix m = Matrix(3, 3, m_);

	double r = m.trace();

	double rv = 1 + 5 + 9;

	std::cout << "test_trace: ";
	if (r == rv)
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		std::cout  << r << "\n" << "\n";

	}

}

void test_dot2() {
	double m_[] = {1,2,3,4,5,6,7,8,9};
	Matrix m = Matrix(3, 3, m_);

	double v_[] = {1,2,3};
	Matrix v = Matrix(3, 1, v_);

	Matrix r = v.dot(r);

	Matrix rv = Matrix(0,0);


	std::cout << "test_dot2: ";
	if (r == rv)
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(rv);
	}
}

void test_dot3() {
	double m_[] = {1,2,3,4,5,6,7,8,9};
	Matrix m = Matrix(3, 3, m_);

	Matrix v = Matrix::identity(3);

	Matrix r = m.dot(v);

	Matrix rv = m;


	std::cout << "test_dot3: ";
	if (r == rv)
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(rv);
	}
}

void test_add1() {
	double m_[] = {1,2,3,4,5,6,7,8,9};
	Matrix m = Matrix(3, 3, m_);

	double v_[] = {9,8,7,6,5,4,3,2,1};
	Matrix v = Matrix(3, 3, v_);

	Matrix r = m + v;

	double rv_[] = {10,10,10,10,10,10,10,10,10};
	Matrix rv = Matrix(3, 3, rv_);

	std::cout << "test_add1: ";
	if (r == rv)
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(rv);
	}
}

void test_minus1() {
	double m_[] = {1,2,3,4,5,6,7,8,9};
	Matrix m = Matrix(3, 3, m_);

	Matrix r = -m;

	double rv_[] = {-1,-2,-3,-4,-5,-6,-7,-8,-9};
	Matrix rv = Matrix(3, 3, rv_);

	std::cout << "test_minus1: ";
	if (r == rv)
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(rv);
	}
}

void test_subtract1() {
	double m_[] = {1,2,3,4,5,6,7,8,9};
	Matrix m = Matrix(3, 3, m_);

	double v_[] = {9,8,7,6,5,4,3,2,1};
	Matrix v = Matrix(3, 3, v_);

	Matrix r = m - v;

	double rv_[] = {-8,-6,-4,-2,0,2,4,6,8};
	Matrix rv = Matrix(3, 3, rv_);

	std::cout << "test_subtract1: ";
	if (r == rv)
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(rv);
	}
}

void test_mul1() {
	double m_[] = {1,2,3,4,5,6,7,8,9};
	Matrix m = Matrix(3, 3, m_);

	double v = 2.0;

	Matrix r = m * v;

	double rv_[] = {2,4,6,8,10,12,14,16,18};
	Matrix rv = Matrix(3, 3, rv_);

	std::cout << "test_mul1: ";
	if (r == rv)
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(rv);
	}
}

void test_inverse() {
	double m_[] = {10, -9, -12, 7, -12, 11, -10, 10, 3};
	Matrix m = Matrix(3, 3, m_);

	Matrix v = m;
	v.inverse();

	Matrix r = m.dot(v);

	Matrix rv = Matrix::identity(3);

	std::cout << "test_inverse: ";

	if (r.closeEnough(rv))
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(rv);
	}
}

void test_submatrix() {
	double m_[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	Matrix m = Matrix(4, 4, m_);

	Matrix r = m.submatrix(1,1,2,2);;

	double rv_[] = {6,7,10,11};
	Matrix rv = Matrix(2, 2, rv_);

	std::cout << "test_submatrix: ";
	if (r == rv)
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(rv);
	}
}



void test_heap1() {
	double m_[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	Matrix m = Matrix(4, 4, m_);
	Matrix r;
	r = m;



	std::cout << "test_heap1: ";
	if (r == m)
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(m);
	}
}

void test_heap2() {
	double m_[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	Matrix m = Matrix(4, 4, m_);
	{
		Matrix r = m;
		r.release();
		r = m.submatrix(0,0,2,2);
		m.release();
		m = r;
	}

	double rv_[] = {1,2,3,5,6,7,9,10,11};
	Matrix rv(3,3,rv_);
	std::cout << "test_heap2: ";
	if (rv == m)
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(m);
		mprint(rv);
	}
}
void test_q_multiply() {
	double a_[] = {0.45576804,  0.060003,    0.5406251,   0.70455634};
	Matrix a = Matrix(1, 4, a_);
	double b_[] = {0.95832283,  0.65963177,  0.91978193,  0.79914949};
	Matrix b = Matrix(1, 4, b_);
	Matrix r = a.quaternion_multiply(b);
	double rv_[] = {-0.66311,     0.1421434,   1.35409696,  0.73799541};
	Matrix rv(1, 4, rv_);
	
	std::cout << "test_q_multiply: ";

	if (r.closeEnough(rv))
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(rv);
	}
}

void test_quaternion_rotate1() {
	double A_[] = { 0.70710678,  0.0,          0.70710678};
	double B_[] = { 0.0,         0.70710678,   0.70710678};
	double A2_[] = {0, 1, 0};
	double B2_[] = {0., 0.5, 0.8660254};
	double Q_[] = {0.45576804,  0.060003,    0.5406251,   0.70455634};
	Matrix A(1,3,A_);
	Matrix B(1,3,B_);
	Matrix A2(1,3,A2_);
	Matrix B2(1,3,B2_);
	Matrix Q(1,4,Q_);
	
	Matrix r = A.quaternion_rotate(Q);
	
	Matrix rv = A2;
	std::cout << "test_quaternion_rotate1: ";
	if (rv.closeEnough(r))
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(rv);
	}
}

void test_quaternion_rotate2() {
	double A_[] = { 0.70710678,  0.0,          0.70710678};
	double B_[] = { 0.0,         0.70710678,   0.70710678};
	double A2_[] = {0, 1, 0};
	double B2_[] = {0., 0.5, 0.8660254};
	double Q_[] = {0.45576804,  0.060003,    0.5406251,   0.70455634};
	Matrix A(1,3,A_);
	Matrix B(1,3,B_);
	Matrix A2(1,3,A2_);
	Matrix B2(1,3,B2_);
	Matrix Q(1,4,Q_);
	
	Matrix r = B.quaternion_rotate(Q);
	
	Matrix rv = B2;
	std::cout << "test_quaternion_rotate2: ";
	if (rv.closeEnough(r))
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(rv);
	}
}

void test_quaternion_inverse() {
	double Q_[] = {0.6579081,   0.2558758,   0.62292432,  0.72851706};
	Matrix Q(1,4,Q_);
	
	Matrix r = Q.quaternion_inverse();
	
	double rv_[] = {0.46426787, -0.1805646,  -0.43958076, -0.51409469};
	
	Matrix rv(1, 4, rv_);
	std::cout << "test_quaternion_inverse: ";
	if (rv.closeEnough(r))
		std::cout  << "ok\n";
	else {
		std::cout  << "failed\n";
		mprint(r);
		mprint(rv);
	}
}

int main()
{
	test_dot1();
	test_dot2();
	test_dot3();
	test_dotSelf1();
	test_add1();
	test_minus1();
	test_subtract1();
	test_mul1();
	test_inverse();
	test_submatrix();
	test_trace();
	test_transpose();
	test_heap1();
	test_heap2();
	test_q_multiply();
	test_quaternion_inverse();
	test_quaternion_rotate1();
	test_quaternion_rotate2();
	

	return 0;
}
