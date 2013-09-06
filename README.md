LineArduino
===========

Linear algebra for Arduino, C++ implementation
Implemented operations:

* addition/subtraction
* scalar multiplication
* elementwise multiplication
* dot product
* cross product (for 3D row-vectors)
* transposion
* inversion
* normalization 


Examples of usage:

    Matrix a(3,3);
    Matrix b(3,1);
    Matrix c = a.dot(b);
    Matrix d = c.transpose().dot(a);
    d += b;
    d *= 1.5;
    Matrix I = Matrix::identity(3);
    a.dotself(I);
    
    double[] x = {1,2,3,4,5,6,7,8,9};
    Matrix X(3,3,x);
    X += I;
    Matrix Y = X - a;
    
    
    
    
    


