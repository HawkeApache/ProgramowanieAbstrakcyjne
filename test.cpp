#include "AbstractMatrix.h"
#include "Matrix.h"
#include "SquareMatrix.h"
#include "Vector.h"
#include <vector>
#include <iostream>

int main(){
    std::vector<double> u = {1,-1,2,3,0,-4,2,3,5};
    std::vector<double> v = {0,4,1,5,2,7,0,2,7};
    std::vector<double> w= {1,2,3,4,2,3,1,2,1,1,1,-1,1,0,-2,-6};
    
    SquareMatrix<double> a(3, u);
    Matrix<double> b(3,3,v);
    Matrix<double> c = a+b;
    Matrix<double> d(3,3,1);
    SquareMatrix<double> e = a*d;
    SquareMatrix<double> f(4,w);
    SquareMatrix<double> g = f.invert();
    Vector<double> h(16,w);
    Vector<double> i(16,false,1);    
    Vector<double> j = h+i;
    
    std::cout << "a=\n" << a << "b=\n" << b << "a+b=\n" << c << "d=\n" << d << "a*d=\n" << e;
    std::cout << "det a = " << a.det() << "; det b = " << b.det() << std::endl;
    std::cout << "f=\n" << f << "odwrotna do f =\n" << g;
    std::cout << "h=\n" << h << "i=\n" << i << "h+i=\n" << j;
    std::cout << "h max = " << h.max() << "; h min == " << h.min() << std::endl;
    std::cout << "b: is square? " << b.isSquare() << ";  is vector? " << b.isVector() 
            << "; is identity? " << b.isIdentity() << ";  is diagonal? " << b.isDiagonal() 
            << "; is zero? " << b.isZero() << std::endl;
    b.makeIdentity();
    std::cout << "; is identity? " << b.isIdentity() << ";  is diagonal? " << b.isDiagonal() ; 
}
