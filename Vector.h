#ifndef VECTOR_H
#define VECTOR_H

#include "AbstractMatrix.h"

template <class Scalar>
class Vector final : public AbstractMatrix<Scalar> {
public:
    typedef typename AbstractMatrix<Scalar>::iterator iterator;
    typedef typename AbstractMatrix<Scalar>::const_iterator const_iterator;
    
    Vector() {}
    
    Vector(unsigned size, bool transposed, Scalar value) :
        data(size, value), size(size), transposed(transposed) {}
    
    Vector(unsigned size, bool transposed, std::vector<Scalar> values) :
        size(size), transposed(transposed) {
            if (size != values.size())
                throw std::runtime_error("Wrong number of elements.");
            data.assign(values.begin(), values.end());
    }
    
    Vector(unsigned size, std::vector<Scalar> values) : size(size) {
        if (size != values.size())
            throw std::runtime_error("Wrong number of elements.");
        data.assign(values.begin(), values.end());
    }
    
    Vector(const AbstractMatrix<Scalar>& m) {
        if (!m.isVector())
            throw std::runtime_error("Not a vector.");
        if (m.getRows() == 1)
            size = m.getColumns();
        else{
            size = m.getRows();
            transposed = true;
        }
        data.assign(m.begin(), m.end());
    }
    
    virtual bool isSquare() const override {
        return data.size() == 1;
    }
    
    virtual bool isVector() const override {
        return true;
    }
    
    virtual bool isDiagonal() const override {
        return size == 1;
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
        return data[0] == 1;
    }
    
    virtual unsigned getRows() const override {
        if (transposed)
            return size;
        return 1;
    }
    
    virtual unsigned getColumns() const override {
        if (transposed)
            return 1;
        return size;
    }

    virtual iterator begin() override { return data.begin(); }
    virtual iterator end() override { return data.end(); }
    virtual const_iterator begin() const override { return data.begin(); }
    virtual const_iterator end() const override { return data.end(); }
    
    virtual Scalar operator()(unsigned r, unsigned c) const {
        if (r >= getRows() || c >= getColumns())
            throw std::out_of_range("Vector::operator()");
        return data[r * getColumns() + c];
    }

    virtual Scalar& operator()(unsigned r, unsigned c) {
        if (r >= getRows() || c >= getColumns())
            throw std::out_of_range("Vector::operator()");
        return data[r * getColumns() + c];
    }

    virtual Scalar operator()(unsigned p) const {
        if (p >= size)
            throw std::out_of_range("Vector::operator()");
        return data[p];
    }

    virtual Scalar& operator()(unsigned p) {
        if (p >= size)
            throw std::out_of_range("Vector::operator()");
        return data[p];
    }
    
    bool operator==(const AbstractMatrix<Scalar>& m) {
        if (getRows() !=m.getRows() || getColumns() != m.getColumns())
            return false;
        for (const_iterator i=begin(), j=m.begin(); i!=end(), j!=m.end(); ++i, ++j)
            if (*i != *j)
                return false;
        return true;
    }
    
     bool operator!=(const AbstractMatrix<Scalar>& m) {
         return this == &m;
     }
    
    Vector& operator=(const AbstractMatrix<Scalar>& m){
        if (this != &m){
            if (!m.isVector())
                throw std::runtime_error("Not a vector.");
            if (m.getRows() == 1){
                size = m.getColumns();
                transposed = false;
            }else{
                size = m.getRows();
                transposed = true;
            }
            data.assign(m.begin(), m.end());
        }
        return *this;
    }
    
    Vector operator-() {
        std::vector<Scalar> temp;
        for (auto element : *this)
            temp.push_back(-element);
        return Vector(size, transposed, temp);
    }
    
    Vector operator+(const AbstractMatrix<Scalar>& m) {
        if (getRows() != m.getRows() || getColumns() != m.getColumns())
            throw std::runtime_error("dsf");
        std::vector<Scalar> sum;
        for (const_iterator i=begin(), j=m.begin(); i!=end(), j!=m.end(); ++i, ++j)
            sum.push_back(*i + *j);
        return Vector(size, sum);
    }
    
    Vector& operator+=(const AbstractMatrix<Scalar>& m){
        *this = *this + m;
        return *this;
    }
    
    Vector operator-(const AbstractMatrix<Scalar>& m) {
        if (getRows() != m.getRows() || getColumns() != m.getColumns())
            throw std::runtime_error("dsf");
        std::vector<Scalar> sum;
        for (const_iterator i=begin(), j=m.begin(); i!=end(), j!=m.end(); ++i, ++j)
            sum.push_back(*i - *j);
        return Vector(size, sum);
    }
    
    Vector& operator-=(const AbstractMatrix<Scalar>& m){
        *this = *this - m;
        return *this;
    }
    
    Matrix<Scalar> operator*(const AbstractMatrix<Scalar>& m) {
        if (getColumns() != m.getRows())
            throw std::runtime_error("Wrong size");
        std::vector<Scalar> elements;
        for(unsigned i = 0; i < getRows(); i++){
            std::vector<Scalar> row(m.getColumns(), 0);
            for(unsigned j = 0; j < getColumns(); j++)
                for(unsigned k = 0; k < m.getColumns(); k++)
                    row[k] = row[k] + this->operator()(i,j) * m(j,k);
            for(auto it = row.begin(); it != row.end(); it++)
                elements.push_back(*it);
        }
        return Matrix<Scalar>(getRows(), m.getColumns(), elements);
    }
    
    Vector& operator*=(const AbstractMatrix<Scalar>& m){
        *this = *this * m;
        return *this;
    }
    
    Vector operator*(const Scalar& c) {
        std::vector<Scalar> elements;
        for(auto it = this->begin(); it != this->end(); it++)
            elements.push_back(*it * c);
        return Vector(size, transposed, elements);
    }
    
    Vector& operator*=(const Scalar& c){
        *this = *this * c;
        return *this;
    }
    
    template<typename Scalar_>
    friend Vector<Scalar_> operator*(const Scalar_& c, const Vector<Scalar_>& m);
    
    Vector transpone() {
        return Vector(size, !transposed, data);
    }

private:
    unsigned size;
    std::vector<Scalar> data;
    bool transposed = false;
    /**
     * false - horizontal
     * true - vertical
     */
};

template<typename Scalar>
Vector<Scalar> operator*(const Scalar& c, Vector<Scalar>& m) {
   return m*c;
}

#endif

