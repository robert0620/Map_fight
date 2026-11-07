#ifndef PUNCT2D_H
#define PUNCT2D_H

#include <iostream>
#include <cmath>

template <typename T>
class Punct2D {
private:
    T x;
    T y;

public:
    explicit Punct2D(T xVal = 0, T yVal = 0) : x(xVal), y(yVal) {}

    T getX() const { return x; }
    T getY() const { return y; }

    T distantaManhattan(const Punct2D<T>& altul) const {
        return std::abs(x - altul.x) + std::abs(y - altul.y);
    }

    bool operator==(const Punct2D<T>& altul) const {
        return (x == altul.x && y == altul.y);
    }

    Punct2D<T> operator+(const Punct2D<T>& offset) const {
        return Punct2D<T>(x + offset.x, y + offset.y);
    }

    friend std::ostream& operator<<(std::ostream& os, const Punct2D<T>& pct) {
        os << "(" << pct.x << ", " << pct.y << ")";
        return os;
    }
};

#endif