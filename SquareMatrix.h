#ifndef SQUARE_MATRIX_H
#define SQUARE_MATRIX_H

#include "AbstractMatrix.h"
#include <stdexcept>

template <class Scalar>
class SquareMatrix final : public AbstractMatrix<Scalar> {
public:
    typedef typename AbstractMatrix<Scalar>::iterator iterator;
    typedef typename AbstractMatrix<Scalar>::const_iterator const_iterator;
    
    SquareMatrix() {}
    
    SquareMatrix(unsigned size, Scalar value = Scalar()) : data(size * size, value), size(size) {}

    SquareMatrix(unsigned size, std::vector<Scalar> values) : size(size) {
        if (size * size != values.size())
            throw std::runtime_error("Wrong number of elements.");
        data.assign(values.begin(), values.end());
    }
    
    SquareMatrix(const SquareMatrix& m) : data(m.data), size(m.size) {}
    
    SquareMatrix(const AbstractMatrix<Scalar>& m) {
        if (!m.isSquare())
            throw std::runtime_error("Not a square matrix.");
        size = m.getRows();
        data.assign(m.begin(), m.end());
    }

    virtual bool isSquare() const override {
        return true;
    }

    virtual bool isVector() const override {
        return size == 1;
    }
    
    virtual bool isDiagonal() const override {
        for (int i=0; i<size; i++)
            for (int j=0; j<size; j++)
                if (i != j && data[i * size + j] != 0)
                    return false;
        return true;
    }
    
    virtual bool isZero() const override {
        for (auto i = begin(); i != end(); i++)
            if (*i != 0)
                return false;
        return true;
    }
    
    virtual bool isIdentity() const override {
        if (!isDiagonal())
            return false;
        for (int i = 0; i < size*size; i += size +1 )
            if (data[i] != 1)
                return false;
        return true;
    }
    
    virtual unsigned getRows() const override { return size; }
    virtual unsigned getColumns() const override { return size; }

    virtual iterator begin() override { return data.begin(); }
    virtual iterator end() override { return data.end(); }
    virtual const_iterator begin() const override { return data.begin(); }
    virtual const_iterator end() const override { return data.end(); }

    virtual Scalar operator()(unsigned r, unsigned c) const {
        if (r >= size || c >= size)
            throw std::out_of_range("d");
        return data[r * size + c];
    }

    virtual Scalar& operator()(unsigned r, unsigned c) {
        if (r >= size || c >= size)
            throw std::out_of_range("d");
        return data[r * size + c];
    }
    
    bool operator==(const AbstractMatrix<Scalar>& m) {
        if (!m.isSquare())
            return false;
        if (size !=m.getRows())
            return false;
        for (const_iterator i=begin(), j=m.begin(); i!=end(), j!=m.end(); ++i, ++j)
            if (*i != *j)
                return false;
        return true;
    }
    
    bool operator!=(const AbstractMatrix<Scalar>& m) {
        return this == &m;
    }
    
    SquareMatrix& operator=(const AbstractMatrix<Scalar>& m) {
        if (this != &m){
            if (!m.isSquare())
                throw std::runtime_error("Not a square matrix.");
            size = m.getRows();
            data.assign(m.begin(), m.end());
        }
        return *this;
    }
    
    SquareMatrix operator-() {
        std::vector<Scalar> temp;
        for (auto element : *this)
            temp.push_back(-element);
        return SquareMatrix(size, temp);
    }
    
    SquareMatrix operator+(const AbstractMatrix<Scalar>& m) {
        if (size != m.getRows() || size != m.getColumns())
            throw std::runtime_error("Wrong number of elements");
        std::vector<Scalar> sum;
        for (const_iterator i=begin(), j=m.begin(); i!=end(), j!=m.end(); ++i, ++j)
            sum.push_back(*i + *j);
        return SquareMatrix(size, sum);
    }
    
    SquareMatrix& operator+=(const AbstractMatrix<Scalar>& m){
        *this = *this + m;
        return *this;
    }
    
    SquareMatrix operator-(const AbstractMatrix<Scalar>& m) {
        if (size != m.getRows() || size != m.getColumns())
            throw std::runtime_error("Wrong number of elements");
        std::vector<Scalar> sum;
        for (const_iterator i=begin(), j=m.begin(); i!=end(), j!=m.end(); ++i, ++j)
            sum.push_back(*i - *j);
        return SquareMatrix(size, sum);
    }
    
    SquareMatrix& operator-=(const AbstractMatrix<Scalar>& m){
        *this = *this - m;
        return *this;
    }
    
    Matrix<Scalar> operator*(const AbstractMatrix<Scalar>& m) {
        if (size != m.getRows())
            throw std::runtime_error("Wrong size");
        std::vector<Scalar> elements;
        for(unsigned i = 0; i < size; i++){
            std::vector<Scalar> row(m.getColumns(), 0);
            for(unsigned j = 0; j < size; j++)
                for(unsigned k = 0; k < m.getColumns(); k++)
                    row[k] = row[k] + this->operator()(i,j) * m(j,k);
            for(auto it = row.begin(); it != row.end(); it++)
                elements.push_back(*it);
        }
        return Matrix<Scalar>(size, m.getColumns(), elements);
    }
    
    SquareMatrix& operator*=(const AbstractMatrix<Scalar>& m){
        *this = *this * m;
        return *this;
    }
    
    SquareMatrix operator*(const Scalar& c) {
        std::vector<Scalar> elements;
        for(auto it = this->begin(); it != this->end(); it++)
            elements.push_back(*it * c);
        return SquareMatrix<Scalar>(size, elements);
    }
    
    SquareMatrix& operator*=(const Scalar& c){
        *this = *this * c;
        return *this;
    }
    
    template<typename Scalar_>
    friend SquareMatrix<Scalar_> operator*(const Scalar_& c, const SquareMatrix<Scalar_>& m);
    
    SquareMatrix transpone(){
        std::vector<Scalar> elements(size * size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                elements[j * size + i] = data[i * size + j];
        return SquareMatrix(size, elements);
    }

private:
    std::vector<Scalar> data;
    unsigned size;
};

template<typename Scalar>
SquareMatrix<Scalar> operator*(const Scalar& c, SquareMatrix<Scalar>& m) {
   return m*c;
}

#endif
