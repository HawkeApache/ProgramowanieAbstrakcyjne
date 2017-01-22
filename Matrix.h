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
            data = std::move(values);
    }

    Matrix(const Matrix& m) = default;
    Matrix(Matrix&& m) = default;

    Matrix& operator=(const Matrix& m) = default;
    Matrix& operator=(Matrix&& m) = default;
    
    Matrix(const AbstractMatrix<Scalar>& m) {
        rows = m.getRows();
        columns = m.getColumns();
        data.assign(m.begin(), m.end());
    }
    
    Matrix& operator=(const AbstractMatrix<Scalar>& m) {
        if (this != &m){
            rows = m.getRows();
            columns = m.getColumns;
            data.assign(m.begin(), m.end());
        }
        return *this;
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
        for (unsigned i = 0; i < rows; i++)
            for (unsigned j = 0; j < columns; j++)
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
        for (unsigned i = 0; i < rows * columns; i += columns + 1 )
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

    virtual Scalar operator()(unsigned r, unsigned c) const override{
        if (r >= rows || c >= columns)
            throw std::out_of_range("Matrix::operator()");
        return data[r * columns + c];
    }

    virtual Scalar& operator()(unsigned r, unsigned c) override {
        if (r >= rows || c >= columns)
            throw std::out_of_range("Matrix::operator()");
        return data[r * columns + c];
    }
    
    bool operator==(const AbstractMatrix<Scalar>& m) const override {
        if (rows != m.getRows() || columns != m.getColumns())
            return false;
        for (const_iterator i = begin(), j = m.begin(); j != m.end(); ++i, ++j)
            if (*i != *j)
                return false;
        return true;
    }
    
    Matrix operator-() const {
        std::vector<Scalar> temp;
        temp.reserve(data.size());
        for (auto element : *this)
            temp.push_back(-element);
        return Matrix(rows, columns, std::move(temp));
    }
    
    Matrix& operator+=(const AbstractMatrix<Scalar>& m){
        if (rows != m.getRows() || columns != m.getColumns())
            throw std::runtime_error("Wrong size");
        unsigned size = rows * columns;
        auto m_data = m.begin();
        for (unsigned i = 0; i < size; i++)
            data[i] += m_data[i];
        return *this;
    }
    
    Matrix operator+(const AbstractMatrix<Scalar>& m) {
        Matrix copy(*this);
        copy += m;
        return copy;
    }
    
    Matrix& operator-=(const AbstractMatrix<Scalar>& m){
        if (rows != m.getRows() || columns != m.getColumns())
            throw std::runtime_error("Wrong size");
        unsigned size = rows * columns;
        auto m_data = m.begin();
        for (unsigned i = 0; i < size; i++)
            data[i] -= m_data[i];
        return *this;
    }
    
    Matrix operator-(const AbstractMatrix<Scalar>& m) {
        Matrix copy(*this);
        copy -= m;
        return copy;
    }
    
    Matrix operator*(const AbstractMatrix<Scalar>& m) {
        if (columns != m.getRows())
            throw std::runtime_error("Wrong size");
        unsigned resColumns = m.getColumns();
        std::vector<Scalar> elements(rows * resColumns, 0);
        for (unsigned i = 0; i < rows; i++){
            Scalar* row = elements.data() + i * resColumns;
            for (unsigned j = 0; j < columns; j++)
                for (unsigned k = 0; k < resColumns; k++)
                    row[k] += this->operator()(i,j) * m(j,k);
        }
        return Matrix<Scalar>(rows, resColumns, std::move(elements));
    }
    
    Matrix& operator*=(const AbstractMatrix<Scalar>& m) {
        *this = *this * m;
        return *this;
    }
    
    Matrix& operator*=(const Scalar& c) {
        for (auto& e : data)
            e *= c;
        return *this;
    }
    
    Matrix operator*(const Scalar& c) {
        Matrix copy(*this);
        copy *= c;
        return copy;
    }
    
    virtual Scalar trace() const override {
        if (!isSquare())
            throw std::runtime_error("Not a square matrix");
        Scalar trace = 0;
        for (unsigned i = 0; i < rows*columns; i += columns +1 )
            trace += data[i];
        return trace;
    }
    
    template<typename T = double>
    T det() const {
        if (!isSquare())
            throw std::runtime_error("Not a square matrix");
        if (rows == 1)
            return data[0];
        if (rows == 2)
            return (data[0]*data[3] - data[1]*data[2]);
        
        std::vector<T> v(data.begin(), data.end());
        Matrix<T> m(rows, columns, v);
        
        bool swap = false;
        for (unsigned k = 0; k < rows-1; k++){
            if (m(k,k) == 0){
                bool detZero = true;
                for (unsigned i = k+1; i < rows; i++)
                    if (m(i,k) != 0){
                        m.swapRows(i,k);
                        swap = !swap;
                        detZero = false;
                        break;
                    }
                if (detZero)
                    return 0;
            }
            for (unsigned i = k+1; i < rows; i++){
                T factor = m(i,k) / m(k,k);
                for ( unsigned j = k; j < rows; j++){
                    T element = m(i,j) - factor * m(k,j);
                    m(i, j) = element;
                }
            }
        }
                
        T det = m(0,0);
        for (unsigned i = 1; i < rows; i++)
            det *= m(i,i);
        
        if (swap)
            return -det;
        
        return det;
    }
    
    void swapRows(unsigned first, unsigned second) {
        if (first >= rows || second >= rows)
            throw std::out_of_range("Matrix::swapRows");
        for (unsigned i = 0; i < columns; i++)
            std::swap(data[first * columns + i], data[second * columns + i]);
    }
    
    void swapColumns(unsigned first, unsigned second) {
        if (first >= rows || second >= rows)
            throw std::out_of_range("Matrix::swapColumns");
        for (unsigned i = 0; i < rows; i++)
            std::swap(data[i * columns + first], data[i * columns + second]);
    }
    
    Matrix transpone() const {
        std::vector<Scalar> elements(rows * columns);
        for (unsigned i = 0; i < rows; i++)
            for (unsigned j = 0; j < columns; j++)
                elements[j * rows + i] = data[i * columns + j];
        return Matrix(columns, rows, std::move(elements));
    }
    
    void transponeThis() {
        *this = transpone();
    }

private:
    std::vector<Scalar> data;
    unsigned rows = 0, columns = 0;
};

template<typename Scalar>
Matrix<Scalar> operator*(const Scalar& c, Matrix<Scalar>& m) {
   return m * c;
}

#endif
