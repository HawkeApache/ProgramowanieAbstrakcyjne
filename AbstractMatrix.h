/*

X	1. Dziedziczenie
X	2. Metody wirtualne
X	3. Funkcje czysto wirtualne 
X	4. Polimorfizm dynamiczny
X	5. Funkcje szablonowe
X	6. Klasy szablonowe
X	7. Specjalizowanie klas i funkcji szablonowych
X	8. Osadzone klasy wewnątrz szablonów klas
X	9. Funktory
X	10. STL
X		a. Kontenery
X		b. Iteratorys
X		c. Algorytmy
X		d. Obiekty funkcyjne
	11. Koncepty
	12. Inteligentne wskaźniki

*/

#ifndef ABSTRACT_MATRIX_H
#define ABSTRACT_MATRIX_H

#include <algorithm>
#include <stdexcept>
#include <vector>
#include <iostream>

template <class Scalar_>
class AbstractMatrix {
public:
    typedef Scalar_ Scalar;
    typedef typename std::vector<Scalar>::iterator iterator;
    typedef typename std::vector<Scalar>::const_iterator const_iterator;

    virtual bool isSquare() const = 0;
    virtual bool isVector() const = 0;
    virtual bool isDiagonal() const = 0;
    virtual bool isZero() const = 0;
    virtual bool isIdentity() const = 0;
    
    virtual unsigned getRows() const = 0;
    virtual unsigned getColumns() const = 0;

    virtual iterator begin() = 0;
    virtual iterator end() = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    
    virtual Scalar operator()(unsigned r, unsigned c) const = 0;
    virtual Scalar& operator()(unsigned r, unsigned c) = 0;
    virtual bool operator==(const AbstractMatrix<Scalar>& m) = 0;
    virtual bool operator!=(const AbstractMatrix<Scalar>& m) = 0;
    
    template<class Scalar>
    friend std::ostream& operator<< (std::ostream& out, const AbstractMatrix<Scalar>& m);

    class MaxCompare {
    public:
        bool operator()(const AbstractMatrix& l, const AbstractMatrix& r) const {
            const_iterator lMax = std::max_element(l.begin(), l.end());
            const_iterator rMax = std::max_element(r.begin(), r.end());
            return *lMax < *rMax;
        }
    };
};

template<class Scalar>
std::ostream& operator<<(std::ostream& out, const AbstractMatrix<Scalar>& m){
    for (unsigned i = 0; i<m.getRows(); i++){
        for (unsigned j = 0; j<m.getColumns(); j++)
            out << m(i,j) << "  ";
        out << std::endl;
    }
    return out;
}



#endif
