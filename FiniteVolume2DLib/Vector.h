/*
 * Name  : Vector
 * Path  : 
 * Use   : 2d vector
 * Author: Sven Schmidt
 * Date  : 04/01/2012
 */
#pragma once


class Vector {
public:
    Vector(double x, double y);

    double x() const;
    double y() const;
    double norm() const;

private:
    double x_;
    double y_;
};
