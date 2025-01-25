#include "vector3d.h"
#include "constants.h"
#include <iostream>
#include <valarray>

Vector3D::Vector3D()
    :x{0.0}, y{0.0}, z{0.0} {}

Vector3D::Vector3D(double x, double y, double z)
    :x{x}, y{y}, z{z} {}

Vector3D& Vector3D::operator+=(const Vector3D& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

Vector3D& Vector3D::operator*=(const Vector3D& rhs) {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}
  
Vector3D& Vector3D::operator*=(const double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3D& Vector3D::operator/=(const double scalar) {
    if (scalar == 0) {
        throw std::overflow_error("Division by zero!");
    }
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

bool almost_equal(double x, double y) {
    return std::abs(x - y) < Constants::epsilon;
}

bool operator==(const Vector3D& lhs, const Vector3D& rhs) {
    return almost_equal(lhs.x, rhs.x)
        && almost_equal(lhs.y, rhs.y)
        && almost_equal(lhs.z, rhs.z);
}

bool operator!=(const Vector3D& lhs, const Vector3D& rhs) {
    return !(lhs == rhs);
}

Vector3D operator+(Vector3D lhs, const Vector3D& rhs) {
    lhs += rhs;
    return lhs;
}

Vector3D operator-(Vector3D lhs, const Vector3D& rhs) {
    lhs -= rhs;
    return lhs;
}

Vector3D operator-(const Vector3D& rhs) {
    return -1 * rhs;
}

Vector3D operator*(Vector3D lhs, const Vector3D& rhs) {
    lhs *= rhs;
    return lhs;
}

Vector3D operator*(Vector3D lhs, const double scalar) {
    lhs *= scalar;
    return lhs;
}

Vector3D operator*(const double scalar, Vector3D rhs) {
    rhs *= scalar;
    return rhs;
}

Vector3D operator/(Vector3D lhs, const double scalar) {
    lhs /= scalar;
    return lhs;
}

Vector3D pow(Vector3D v, const double n) { // not sure if the set 'v.? =' is necessary
    v.x = std::pow(v.x, n);
    v.y = std::pow(v.y, n);
    v.z = std::pow(v.z, n);
    return v;
}

double dot(const Vector3D& a, const Vector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
  
Vector3D cross(const Vector3D& a, const Vector3D& b) {
    double x = a.y * b.z - a.z * b.y;
    double y = a.z * b.x - a.x * b.z;
    double z = a.x * b.y - a.y * b.x;
    return Vector3D{x, y, z};
}
  
double length(const Vector3D& v) {
    // distance formula
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3D unit(const Vector3D& v) {
    if (length(v) == 0) {
        throw std::overflow_error("Unit Length = 0!");
    }
    double b{1/length(v)};
    return Vector3D{b, b, b};
}

std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
    return os << "(" << v.x << " " << v.y << " " << v.z << ")";
}

std::istream& operator>>(std::istream& is, Vector3D& v) {
    char paren;
    return is >> paren >> v.x >> v.y >> v.z >> paren;
}
