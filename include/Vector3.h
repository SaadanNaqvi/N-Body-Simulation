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
        Vector3 operator*(double scalar) const;
        Vector3 operator/(double scalar) const;
        Vector3& operator+=(const Vector3& other);
        Vector3& operator*=(double scalar);
        
        // Getters
        double getX();
        double getY();
        double getZ();

        // Setters
        void setX(double x);
        void setY(double y);
        void setZ(double z);
};


#endif