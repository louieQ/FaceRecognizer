#ifndef CIRCLE_H
#define CIRCLE_H

#include <opencv2/core/core.hpp>

using namespace cv;

class Circle
{
    public:
        Circle() {radius=0;}
        Circle(Point center, int radius);
        Point getCenter();
        int getRadius();

    private:
        Point center;
        int radius;
};

#endif // CIRCLE_H
