#ifndef GENDERCLASSIFIER_H
#define GENDERCLASSIFIER_H

#include <classifier.hpp>
#include <face_attributes.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>

using namespace cv;
using namespace cv::dnn;

class GenderClassifier: public Classifier
{
public:
    GenderClassifier(bool high_performances=false);
    GenderClassifier(string model_path);
    unsigned classify(Face &face, double &confidence);

private:
    Ptr<FaceRecognizer> model;
    string model_path = "models/gender/fisher64eq.yml";
    string deep_paths[2] = {"models/gender/gender.prototxt",
                            "models/gender/gender.caffemodel"};
    Net net;
    bool high_performances;
    int imwidth;
    int imheight;
};

#endif // GENDERCLASSIFIER_H
