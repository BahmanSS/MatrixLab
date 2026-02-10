#ifndef RATIONAL
#define RATIONAL

#include <string>
#include "big_integer.hpp"

class Rational {
private:
    BigInteger numerator;
    BigInteger denominator;

    void optimiser();
public:
    Rational();

    Rational(const BigInteger& n);
    Rational(const BigInteger& num, const BigInteger& denom);

    bool operator==(const Rational& other) const; 
    bool operator!=(const Rational& other) const;

    Rational& operator+=(const Rational& other);
    Rational& operator-=(const Rational& other);
    Rational& operator*=(const Rational& other);
    Rational& operator/=(const Rational& other);

    Rational operator-() const;

    bool operator<(const Rational& other) const;
    bool operator<=(const Rational& other) const;
    bool operator>(const Rational& other) const;
    bool operator>=(const Rational& other) const;

    std::string to_string() const;
    int to_int() const;
};

Rational operator*(const Rational& a, const Rational& b);
Rational operator/(const Rational& a, const Rational& b);
Rational operator+(const Rational& a, const Rational& b);
Rational operator-(const Rational& a, const Rational& b);

#endif