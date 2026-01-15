#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3{
    private:
        double x;
        double y;
        double z;
    public:
        Vector3(double x, double y, double z);
        
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