#include "Vector3.h"

Vector3::Vector3(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vector3::Vector3(double x, double y, double z){
    this->x = x;
    this->y = y;
    this->z = z;
}

double Vector3::getX(){
    return x;
}

double Vector3::getY(){
    return y;
}

double Vector3::getZ(){
    return z;
}


Vector3 Vector3::operator+(const Vector3& other) const {
    return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vector3 Vector3::operator*(double scalar) const {
    return Vector3(this->x * scalar, this->y * scalar, this->z * scalar);
}

Vector3 Vector3::operator/(double scalar) const {
    return Vector3(this->x / scalar, this->y / scalar, this->z / scalar);
}


Vector3& Vector3::operator+=(const Vector3& other) {
    x += other.x; y += other.y; z += other.z;
    return *this;
}

Vector3& Vector3::operator*=(double scalar) {
    x *= scalar; y *= scalar; z *= scalar;
    return *this;
}
