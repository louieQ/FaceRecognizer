#ifndef AGECLASSIFIER_H
#define AGECLASSIFIER_H

#include <classifier.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>

using namespace cv;
using namespace cv::dnn;

class AgeClassifier: public Classifier
{
    string high_performances_model_path = "!!! INSERT HIGH PERFORMANCES MODEL PATH HERE !!!";
    string low_performances_model_path = "models/age/Age_cropped.yml";
    Ptr<FaceRecognizer> model;
    Net net;
    bool high_performances;
    int imwidth;
    int imheight;

public:
    AgeClassifier(bool high_performances=false);
    AgeClassifier(string model_path);
    unsigned classify(Face &face, double &confidence);
    void setModel(string);
};

#endif // AGECLASSIFIER_H
