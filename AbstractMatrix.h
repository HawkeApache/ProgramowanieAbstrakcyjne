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
    virtual bool operator==(const AbstractMatrix<Scalar>& m) const = 0;
    bool operator!=(const AbstractMatrix<Scalar>& m) const { return !(*this == m); }
    
    virtual Scalar max() const {
        return *std::max_element(begin(), end());
    }
    
    virtual Scalar min() const {
        return *std::min_element(begin(), end());
    }
    
    virtual Scalar trace() const = 0;
    
    template<typename T = double> T det();
    
    virtual void transponeThis() = 0;
    virtual void makeIdentity() = 0;

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
