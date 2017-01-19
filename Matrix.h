#ifndef MATRIX_H
#define MATRIX_H

#include "AbstractMatrix.h"

template <class Scalar>
class Matrix final : public AbstractMatrix<Scalar> {
public:
    typedef typename AbstractMatrix<Scalar>::iterator iterator;
    typedef typename AbstractMatrix<Scalar>::const_iterator const_iterator;
    
    Matrix() {}
    
    Matrix(unsigned rows, unsigned columns, Scalar value = Scalar()) :
        data(rows * columns, value), rows(rows), columns(columns) {}
        
    Matrix(unsigned rows, unsigned columns, std::vector<Scalar> values) :
        rows(rows), columns(columns) {
            if (rows * columns != values.size())
                throw std::runtime_error("Wrong number of elements");
            data.assign(values.begin(), values.end());
    }

    Matrix(const Matrix& m) : data(m.data), rows(m.rows), columns(m.columns) {}
    
    Matrix(const AbstractMatrix<Scalar>& m) {
        rows = m.getRows();
        columns = m.getColumns();
        data.assign(m.begin(), m.end());
    }

    virtual bool isSquare() const override {
        return rows == columns;
    }

    virtual bool isVector() const override {
        return rows == 1 || columns == 1;
    }
    
    virtual bool isDiagonal() const override {
        if (!isSquare())
            return false;
        for (int i=0; i<rows; i++)
            for (int j=0; j<columns; j++)
                if (i != j && data[i * columns + j] != 0)
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
        for (int i = 0; i < rows*columns; i += columns +1 )
            if (data[i] != 1)
                return false;
        return true;
    } 
    
    virtual unsigned getRows() const override { return rows; }
    virtual unsigned getColumns() const override { return columns; }

    virtual iterator begin() override { return data.begin(); }
    virtual iterator end() override { return data.end(); }
    virtual const_iterator begin() const override { return data.begin(); }
    virtual const_iterator end() const override { return data.end(); }

    virtual Scalar operator()(unsigned r, unsigned c) const {
        if (r >= rows || c >= columns)
            throw std::out_of_range("Matrix::operator()");
        return data[r * columns + c];
    }

    virtual Scalar& operator()(unsigned r, unsigned c) {
        if (r >= rows || c >= columns)
            throw std::out_of_range("Matrix::operator()");
        return data[r * columns + c];
    }
    
    bool operator==(const AbstractMatrix<Scalar>& m) {
        if (rows !=m.getRows() || columns != m.getColumns())
            return false;
        for (const_iterator i=begin(), j=m.begin(); i!=end(), j!=m.end(); ++i, ++j)
            if (*i != *j)
                return false;
        return true;
    }
    
     bool operator!=(const AbstractMatrix<Scalar>& m) {
         return this == &m;
     }
    
    Matrix& operator=(const AbstractMatrix<Scalar>& m) {
        if (this != &m){
            rows = m.getRows();
            columns = m.getColumns;
            data.assign(m.begin(), m.end());
        }
        return *this;
    }
    
    Matrix operator-() {
        std::vector<Scalar> temp;
        for (auto element : *this)
            temp.push_back(-element);
        return Matrix(rows, columns, temp);
    }
    
    Matrix operator+(const AbstractMatrix<Scalar>& m) {
        if (rows != m.getRows() || columns != m.getColumns())
            throw std::runtime_error("Wrong number of elements");
        std::vector<Scalar> sum;
        for (const_iterator i=begin(), j=m.begin(); i!=end(), j!=m.end(); ++i, ++j)
            sum.push_back(*i + *j);
        return Matrix(rows, columns, sum);
    }
    
    Matrix& operator+=(const AbstractMatrix<Scalar>& m){
        *this = *this + m;
        return *this;
    }
    
    Matrix operator-(const AbstractMatrix<Scalar>& m) {
        if (rows != m.getRows() || columns != m.getColumns())
            throw std::runtime_error("Wrong number of elements");
        std::vector<Scalar> sum;
        for (const_iterator i=begin(), j=m.begin(); i!=end(), j!=m.end(); ++i, ++j)
            sum.push_back(*i - *j);
        return Matrix(rows, columns, sum);
    }
    
    Matrix& operator-=(const AbstractMatrix<Scalar>& m){
        *this = *this - m;
        return *this;
    }
    
    Matrix operator*(const AbstractMatrix<Scalar>& m) {
        if (columns != m.getRows())
            throw std::runtime_error("Wrong size");
        std::vector<Scalar> elements;
        for(unsigned i = 0; i < rows; i++){
            std::vector<Scalar> row(m.getColumns(), 0);
            for(unsigned j = 0; j < columns; j++)
                for(unsigned k = 0; k < m.getColumns(); k++)
                    row[k] = row[k] + this->operator()(i,j) * m(j,k);
            for(auto it = row.begin(); it != row.end(); it++)
                elements.push_back(*it);
        }
        return Matrix<Scalar>(rows, m.getColumns(), elements);
    }
    
    Matrix& operator*=(const AbstractMatrix<Scalar>& m){
        *this = *this * m;
        return *this;
    }
    
    Matrix operator*(const Scalar& c) {
        std::vector<Scalar> elements;
        for(auto it = this->begin(); it != this->end(); it++)
            elements.push_back(*it * c);
        return Matrix<Scalar>(rows, columns, elements);
    }
    
    Matrix& operator*=(const Scalar& c){
        *this = *this * c;
        return *this;
    }
    
    template<typename Scalar_>
    friend Matrix<Scalar_> operator*(const Scalar_& c, const Matrix<Scalar_>& m);
    
    Matrix transpone(){
        std::vector<Scalar> elements(rows * columns);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < columns; j++)
                elements[j * rows + i] = data[i * columns + j];
        return Matrix(columns, rows, elements);
    }

private:
    std::vector<Scalar> data;
    unsigned rows, columns;
};

template<typename Scalar>
Matrix<Scalar> operator*(const Scalar& c, Matrix<Scalar>& m) {
   return m*c;
}

#endif
