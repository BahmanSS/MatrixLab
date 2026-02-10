#ifndef MATRIX
#define MATRIX

#include <vector>
#include "rational.hpp"

class Matrix {
private:
    size_t rows; // кол-во строчек
    size_t cols; // кол-во столбцов
    std::vector<std::vector<Rational>> data;
public:
     
    Matrix(std::vector<std::vector<Rational>> data);
    Matrix(size_t rows_, size_t cols_, const Rational& el); // матрица rows_ x cols_, состоящая из el
    static Matrix unit(size_t n); // единичная матрица n x n
    static Matrix linspace(const Rational& x_0, const Rational& x_1, const Rational& n);

    bool operator==(const Matrix& other) const;

    Matrix& operator+=(const Matrix& other); 
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(const Matrix& other);
    Matrix& operator/=(const Matrix& other);
    Matrix& elemental_multiplication(const Matrix& other);
    Matrix& elemental_division(const Matrix& other);
    Matrix& min(const Matrix& other);
    Matrix& max(const Matrix& other);
    Matrix& horzcat(const Matrix& other);
    Matrix& vertcat(const Matrix& other);
    Rational to_rational() const;

    Matrix operator-() const;

    Matrix transpose() const; // транспонированная матрица
    Matrix inverse() const; // обратная матрица
    Rational sum() const; // сумма элементов матрицы
    Rational product() const; // произведение элементов матрицы
    Rational determinant() const; // определитель матрицы

    std::string to_string() const;
};

Matrix operator+(const Matrix& a, const Matrix& b);
Matrix operator-(const Matrix& a, const Matrix& b);
Matrix operator*(const Matrix& a, const Matrix& b);
Matrix operator/(const Matrix& a, const Matrix& b);
Matrix elemental_multiplication(const Matrix& a, const Matrix& b); // поэлементное умножение
Matrix elemental_division(const Matrix& a, const Matrix& b); // поэлементное деление
Matrix min(const Matrix& a, const Matrix& b); // матрица с минимальными элеменами из матриц a и b(матрицы одинакового размера)
Matrix max(const Matrix& a, const Matrix& b);// матрица с максимальными элеменами из матриц a и b(матрицы одинакового размера)

#endif