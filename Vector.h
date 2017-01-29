#ifndef VECTOR_H
#define VECTOR_H

#include "AbstractMatrix.h"

template <class Scalar>
class Vector final : public AbstractMatrix<Scalar> {
public:
    typedef typename AbstractMatrix<Scalar>::iterator iterator;
    typedef typename AbstractMatrix<Scalar>::const_iterator const_iterator;
    
    Vector() {}
    
    Vector(unsigned size, bool vertical, Scalar value) :
        data(size, value), size(size), vertical(vertical) {}
    
    Vector(unsigned size, bool vertical, std::vector<Scalar> values) :
        size(size), vertical(vertical) {
            if (size != values.size())
                throw std::runtime_error("Wrong number of elements");
            data = std::move(values);
    }
    
    Vector(unsigned size, std::vector<Scalar> values) : size(size) {
        if (size != values.size())
            throw std::runtime_error("Wrong number of elements");
        data = std::move(values);
    }

    Vector(const Vector& m) = default;
    Vector(Vector&& m) = default;

    Vector& operator=(const Vector& m) = default;
    Vector& operator=(Vector&& m) = default;
    
    Vector(const AbstractMatrix<Scalar>& m) {
        if (!m.isVector())
            throw std::runtime_error("Not a vector");
        if (m.getRows() == 1)
            size = m.getColumns();
        else{
            size = m.getRows();
            vertical = true;
        }
        data.assign(m.begin(), m.end());
    }
    
    Vector& operator=(const AbstractMatrix<Scalar>& m){
        if (this != &m){
            if (!m.isVector())
                throw std::runtime_error("Not a vector");
            if (m.getRows() == 1){
                size = m.getColumns();
                vertical = false;
            }else{
                size = m.getRows();
                vertical = true;
            }
            data.assign(m.begin(), m.end());
        }
        return *this;
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
        if (vertical)
            return size;
        return 1;
    }
    
    virtual unsigned getColumns() const override {
        if (vertical)
            return 1;
        return size;
    }

    virtual iterator begin() override { return data.begin(); }
    virtual iterator end() override { return data.end(); }
    virtual const_iterator begin() const override { return data.begin(); }
    virtual const_iterator end() const override { return data.end(); }
    
    virtual Scalar operator()(unsigned r, unsigned c) const override {
        if (r >= getRows() || c >= getColumns())
            throw std::out_of_range("Vector::operator()");
        return data[r * getColumns() + c];
    }

    virtual Scalar& operator()(unsigned r, unsigned c) override {
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
    
    bool operator==(const AbstractMatrix<Scalar>& m) const override {
        if (getRows() !=m.getRows() || getColumns() != m.getColumns())
            return false;
        for (const_iterator i = begin(), j = m.begin(); j != m.end(); ++i, ++j)
            if (*i != *j)
                return false;
        return true;
    }
    
    Vector operator-() {
        std::vector<Scalar> temp;
        temp.reserve(data.size());
        for (auto element : *this)
            temp.push_back(-element);
        return Vector(size, vertical, std::move(temp));
    }
    
    Vector& operator+=(const AbstractMatrix<Scalar>& m) {
        if (getRows() != m.getRows() || getColumns() != m.getColumns())
            throw std::runtime_error("Wrong size");
        auto m_data = m.begin();
        for (unsigned i = 0; i < size; i++)
            data[i] += m_data[i];
        return *this;;
    }
    
    Vector operator+(const AbstractMatrix<Scalar>& m){
        Vector copy(*this);
        copy += m;
        return copy;
    }
    
    Vector& operator-=(const AbstractMatrix<Scalar>& m) {
        if (getRows() != m.getRows() || getColumns() != m.getColumns())
            throw std::runtime_error("Wrong size");
        auto m_data = m.begin();
        for (unsigned i = 0; i < size; i++)
            data[i] -= m_data[i];
        return *this;;
    }
    
    Vector operator-(const AbstractMatrix<Scalar>& m){
        Vector copy(*this);
        copy -= m;
        return copy;
    }
    
    Matrix<Scalar> operator*(const AbstractMatrix<Scalar>& m) {
        if (getColumns() != m.getRows())
            throw std::runtime_error("Wrong size");
        unsigned resRows = getRows();
        unsigned resColumns = m.getColumns();
        std::vector<Scalar> elements(resRows * resColumns, 0);
        for (unsigned i = 0; i < resRows; i++){
            Scalar* row = elements.data() + i * resColumns;
            for (unsigned j = 0; j < getColumns(); j++)
                for (unsigned k = 0; k < resColumns; k++)
                    row[k] += this->operator()(i,j) * m(j,k);
        }
        return Matrix<Scalar>(resRows, resColumns, std::move(elements));
    }
    
    Vector& operator*=(const AbstractMatrix<Scalar>& m) {
        *this = *this * m;
        return *this;
    }
    
    Vector& operator*=(const Scalar& c) {
        for (auto& e : data)
            e *= c;
        return *this;
    }
    
    Vector operator*(const Scalar& c) {
        Vector copy(*this);
        copy *= c;
        return copy;
    }
    
    virtual Scalar trace() const override {
        if (!isSquare())
            throw std::runtime_error("Not a square matrix");
        return data[0];
    }
    
    template<typename T = double>
    T det() const {
        if (!isSquare())
            throw std::runtime_error("Not a square matrix");
        return (T)data[0];
    }
    
    template<typename T = double>
    Vector<T> invert() const {
        if (!isSquare())
            throw std::runtime_error("Not a square matrix");
        return Vector<T>(1,1/(T)data[0]);
    }
    
    void swapElements(unsigned first, unsigned second) {
        if (first >= size || second >= size)
            throw std::out_of_range("Vector::swapRows");
        std::swap(data[first], data[second]);
    }
    
    Vector transpone() const {
        return Vector(size, !vertical, data);
    }
    
     virtual void transponeThis() override {
        vertical = !vertical;
    }
    
    virtual void makeIdentity() override {
        if (!isSquare())
            throw std::runtime_error("Not a square matrix");
        data[0] = 1;
    }

private:
    std::vector<Scalar> data;
    unsigned size;
    bool vertical = false;
};

template<typename Scalar>
Vector<Scalar> operator*(const Scalar& c, Vector<Scalar>& m) {
   return m * c;
}

#endif
