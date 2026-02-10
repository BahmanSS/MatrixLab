#include "rational.hpp"
#include <limits>

void Rational::optimiser() {
    if (denominator == 0) {
        throw std::invalid_argument("division by zero");
    }
    if (denominator < BigInteger(0)) {
        denominator = -denominator;
        numerator = -numerator;
    }
    if (numerator == BigInteger(0)) {
        denominator = 1;
        return;
    }
    BigInteger tmp_1 = (numerator < BigInteger(0)) ? -numerator: numerator;
    BigInteger tmp_2 = denominator;
    
    BigInteger max = (tmp_1 > tmp_2) ? tmp_1 : tmp_2;
    BigInteger min = (tmp_1 < tmp_2) ? tmp_1 : tmp_2;
    //std::cout << max.to_string() << ' ' << min.to_string() << std::endl;
    BigInteger del = max % min;
    //std::cout << del.to_string() << std::endl;
    while(del != 0) {
        max = min;
        min = del;
        del = max % min;
    }
    numerator = numerator / min;
    denominator = denominator / min;
}

Rational::Rational(): numerator(0), denominator(1) {}

Rational::Rational(const BigInteger& n): numerator(n), denominator(1) {
    
}
Rational::Rational(const BigInteger& num, const BigInteger& denom): numerator(num), denominator(denom) {
    if (denominator == 0) {
        throw std::invalid_argument("denom equal zero!");
    }
    optimiser();
}

bool Rational::operator==(const Rational& other) const {
    return numerator == other.numerator && denominator == other.denominator;
}
bool Rational::operator!=(const Rational& other) const {
    return !(*this == other);
}

Rational& Rational::operator+=(const Rational& other) {
    numerator = numerator * other.denominator + other.numerator * denominator;
    denominator = denominator * other.denominator;
    //std::cout << numerator.to_string() << ' ' <<  denominator.to_string() << std::endl;
    optimiser();
    //std::cout << numerator.to_string() << ' ' <<  denominator.to_string() << std::endl;
    //std::cout << "|||\n";
    return *this;
}
Rational& Rational::operator-=(const Rational& other) {
    numerator = numerator * other.denominator - other.numerator * denominator;
    denominator = denominator * other.denominator;
    optimiser();
    return *this;
}
Rational& Rational::operator*=(const Rational& other) {
    numerator = numerator * other.numerator;
    denominator = denominator * other.denominator;
    optimiser();
    return *this;
}
Rational& Rational::operator/=(const Rational& other) {
    numerator = numerator * other.denominator;
    denominator = denominator * other.numerator;
    optimiser();
    return *this;
}

Rational Rational::operator-() const {
    Rational tmp = *this;
    tmp.numerator = -tmp.numerator;
    // ??? with order
    return tmp;
}

bool Rational::operator<(const Rational& other) const{
    return numerator * other.denominator < other.numerator * denominator;
}
bool Rational::operator<=(const Rational& other) const{
    return (*this < other || *this == other);
}
bool Rational::operator>(const Rational& other) const{
    return !(*this <= other);
}
bool Rational::operator>=(const Rational& other) const{
    return !(*this < other);
}

std::string Rational::to_string() const {
    if (numerator == 0) {
        return "0";
    }
    if (denominator == 1) {
        return numerator.to_string();
    }
    std::string res = numerator.to_string() + "/" + denominator.to_string();
    return res;
}

Rational operator*(const Rational& a, const Rational& b) {
    Rational tmp = a;
    tmp *= b;
    return tmp;
}
Rational operator/(const Rational& a, const Rational& b) {
    Rational tmp = a;
    tmp /= b;
    return tmp;
}
Rational operator+(const Rational& a, const Rational& b) {
    Rational tmp = a;
    tmp += b;
    return tmp;
}
Rational operator-(const Rational& a, const Rational& b) {
    Rational tmp = a;
    tmp -= b;
    return tmp;
}

int Rational::to_int() const {
    if (denominator != 1) {
        throw std::invalid_argument("Can not do int from this Rational");
    }
    
    BigInteger min_int = std::numeric_limits<int>::min();
    BigInteger max_int = std::numeric_limits<int>::max();
    
    if (numerator < min_int || numerator > max_int) {
        throw std::overflow_error("BigInteger too large for int");
    }
    
    // Безопасное преобразование через long long
    return static_cast<int>(stoll(numerator.to_string()));
}