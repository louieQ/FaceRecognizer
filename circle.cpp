#include <circle.hpp>

Circle::Circle(Point center, int radius)
{
    this->center = center;
    this->radius = radius;
}

Point Circle::getCenter() {
    return center;
}

int Circle::getRadius() {
    return radius;
}

