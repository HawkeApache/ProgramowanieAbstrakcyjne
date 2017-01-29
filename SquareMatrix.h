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
            throw std::runtime_error("Wrong number of elements");
        data = std::move(values);
    }

    SquareMatrix(const SquareMatrix& m) = default;
    SquareMatrix(SquareMatrix&& m) = default;

    SquareMatrix& operator=(const SquareMatrix& m) = default;
    SquareMatrix& operator=(SquareMatrix&& m) = default;
    
    
    SquareMatrix(const AbstractMatrix<Scalar>& m) {
        if (!m.isSquare())
            throw std::runtime_error("Not a square matrix");
        size = m.getRows();
        data.assign(m.begin(), m.end());
    }
    
    SquareMatrix& operator=(const AbstractMatrix<Scalar>& m) {
        if (this != &m){
            if (!m.isSquare())
                throw std::runtime_error("Not a square matrix");
            size = m.getRows();
            data.assign(m.begin(), m.end());
        }
        return *this;
    }

    virtual bool isSquare() const override {
        return true;
    }

    virtual bool isVector() const override {
        return size == 1;
    }
    
    virtual bool isDiagonal() const override {
        for (unsigned i = 0; i < size; i++)
            for (unsigned j = 0; j < size; j++)
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
        for (unsigned i = 0; i < size*size; i += size+1 )
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

    virtual Scalar operator()(unsigned r, unsigned c) const override {
        if (r >= size || c >= size)
            throw std::out_of_range("SquareMatrix::operator()");
        return data[r * size + c];
    }

    virtual Scalar& operator()(unsigned r, unsigned c) override {
        if (r >= size || c >= size)
            throw std::out_of_range("SquareMatrix::operator()");
        return data[r * size + c];
    }
    
    bool operator==(const AbstractMatrix<Scalar>& m) const override {
        if (!m.isSquare())
            return false;
        if (size !=m.getRows())
            return false;
        for (const_iterator i = begin(), j = m.begin(); j != m.end(); ++i, ++j)
            if (*i != *j)
                return false;
        return true;
    }
    
    SquareMatrix operator-() const {
        std::vector<Scalar> temp;
        for (auto element : *this)
            temp.push_back(-element);
        return SquareMatrix(size, std::move(temp));
    }
    
    SquareMatrix& operator+=(const AbstractMatrix<Scalar>& m) {
         if (size != m.getRows() || size != m.getColumns())
            throw std::runtime_error("Wrong size");
        auto m_data = m.begin();
        for (unsigned i = 0; i < size * size; i++)
            data[i] += m_data[i];
        return *this;
    }
    
    SquareMatrix operator+(const AbstractMatrix<Scalar>& m) const {
        SquareMatrix copy(*this);
        copy += m;
        return copy;
    }
    
    SquareMatrix& operator-=(const AbstractMatrix<Scalar>& m) {
         if (size != m.getRows() || size != m.getColumns())
            throw std::runtime_error("Wrong size");
        auto m_data = m.begin();
        for (unsigned i = 0; i < size * size; i++)
            data[i] -= m_data[i];
        return *this;
    }
    
    SquareMatrix operator-(const AbstractMatrix<Scalar>& m) const {
        SquareMatrix copy(*this);
        copy -= m;
        return copy;
    }
    
    Matrix<Scalar> operator*(const AbstractMatrix<Scalar>& m) {
        if (size != m.getRows())
            throw std::runtime_error("Wrong size");
        unsigned resColumns = m.getColumns();
        std::vector<Scalar> elements(size * resColumns, 0);
        for (unsigned i = 0; i < size; i++){
            Scalar* row = elements.data() + i * resColumns;
            for (unsigned j = 0; j < size; j++)
                for (unsigned k = 0; k < m.getColumns(); k++)
                    row[k] += this->operator()(i,j) * m(j,k);
        }
        return Matrix<Scalar>(size, resColumns, std::move(elements));
    }
    
    SquareMatrix& operator*=(const AbstractMatrix<Scalar>& m) {
        *this = *this * m;
        return *this;
    }
    
    SquareMatrix& operator*=(const Scalar& c) {
        for (auto& e : data)
            e *= c;
        return *this;
    }
    
    SquareMatrix operator*(const Scalar& c) {
        SquareMatrix copy(*this);
        copy *= c;
        return copy;
    }
    
    
    virtual Scalar trace() const override {
        if (!isSquare())
            throw std::runtime_error("Not a square matrix");
        Scalar trace = 0;
        for (unsigned i = 0; i < size*size; i += size +1 )
            trace += data[i];
        return trace;
    }
    
    template<typename T = double>
    T det() const {
        if (size == 1)
            return data[0];
        if (size == 2)
            return (data[0]*data[3] - data[1]*data[2]);
        
        std::vector<T> v(data.begin(), data.end());
        SquareMatrix<T> m(size, v);
        
        bool swap = false;
        for (unsigned k = 0; k < size-1; k++){
            if (m(k,k) == 0){
                bool detZero = true;
                for (unsigned i = k+1; i < size; i++)
                    if (m(i,k) != 0){
                        m.swapRows(i,k);
                        swap = !swap;
                        detZero = false;
                        break;
                    }
                if (detZero)
                    return 0;
            }
            for (unsigned i = k+1; i < size; i++){
                T factor = m(i,k) / m(k,k);
                for (unsigned j = k; j < size; j++){
                    T element = m(i,j) - factor * m(k,j);
                    m(i,j) = element;
                }
            }
        }
                
        T det = m(0,0);
        for (unsigned i = 1; i < size; i++)
            det *= m(i,i);
        
        if (swap)
            return -det;
        
        return det;
    }
    
    template<typename T = double>
    SquareMatrix<T> invert() const {
        if (size == 1)
            return SquareMatrix<T>(1, 1/(T)data[0]);
        
        std::vector<T> v(data.begin(), data.end());
        SquareMatrix<T> m(size, v);
        SquareMatrix<T> r(size, 1);
        r.makeIdentity();
        
        for (unsigned k = 0; k < size-1; k++){
            if (m(k,k) == 0){
                bool detZero = true;
                for (unsigned i = k+1; i < size; i++)
                    if (m(i,k) != 0){
                        m.swapRows(i,k);
                        r.swapRows(i,k);
                        detZero = false;
                        break;
                    }
                if (detZero)
                    throw std::runtime_error("Singular matrix");
            }
            for (unsigned i = k+1; i < size; i++){
                T factor = m(i,k) / m(k,k);
                for ( unsigned j = 0; j < size; j++){
                    m(i,j) -= factor * m(k,j);
                    r(i,j) -= factor * r(k,j);
                }
            }
        }
        
        if (m(size-1, size-1) == 0)
            throw std::runtime_error("Singular matrix");
        
        for (unsigned k = size - 1; k > 0; k--){
            for (unsigned i = 0; i < k; i++){
                T factor = m(i,k) / m(k,k);
                for ( unsigned j = 0; j < size; j++){
                    m(i,j) -= factor * m(k,j);
                    r(i,j) -= factor * r(k,j);
                }
            }
        }
        
        for (unsigned i = 0; i < size; i++)
            for (unsigned j = 0; j < size; j++)
                r(i,j) /= m(i,i);
        
        return r;
    }
    
    void swapRows(unsigned first, unsigned second) {
        if (first >= size || second >= size)
            throw std::out_of_range("SquareMatrix::swapRows");
        for (unsigned i = 0; i < size; i++)
            std::swap(data[first * size + i], data[second * size + i]);
    }
    
    void swapColumns(unsigned first, unsigned second) {
        if (first >= size || second >= size)
            throw std::out_of_range("SquareMatrix::swapColumns");
        for (unsigned i = 0; i < size; i++)
            std::swap(data[i * size + first], data[i * size + second]);
    }
    
    SquareMatrix transpone() const {
        std::vector<Scalar> elements(size * size);
        for (unsigned i = 0; i < size; i++)
            for (unsigned j = 0; j < size; j++)
                elements[j * size + i] = data[i * size + j];
        return SquareMatrix(size, std::move(elements));
    }
    
    virtual void transponeThis() override {
        *this = transpone();
    }
    
    virtual void makeIdentity() override {
        for (unsigned i = 0; i < size; i++)
            for (unsigned j = 0; j < size; j++)
                if (i != j)
                    data[i * size + j] = 0;
                else
                    data[i * size + j] = 1;
    }

private:
    std::vector<Scalar> data;
    unsigned size = 0;
};

template<typename Scalar>
SquareMatrix<Scalar> operator*(const Scalar& c, SquareMatrix<Scalar>& m) {
   return m * c;
}

#endif
