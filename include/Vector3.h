#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3{
    private:
        double x;
        double y;
        double z;
    public:
        Vector3();
        Vector3(double x, double y, double z);

        Vector3 operator+(const Vector3& other) const;
        Vector3 operator-(const Vector3& other) const;
        Vector3 operator*(double scalar) const;
        Vector3 operator/(double scalar) const;

        Vector3& operator+=(const Vector3& other);
        Vector3& operator*=(double scalar);
        
        // Getters & Setters
        double getX() const;
        double getY() const;
        double getZ() const;
        void setX(double x);
        void setY(double y);
        void setZ(double z);
};

inline Vector3 operator*(double scalar, const Vector3& v) {
    return v * scalar;
}

#endif