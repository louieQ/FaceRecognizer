#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <opencv2/face.hpp>
#include <face.hpp>

using namespace cv::face;

class Classifier
{
    public:
        virtual unsigned classify(Face& face, double &confidence) = 0;
};

#endif // CLASSIFIER_H
