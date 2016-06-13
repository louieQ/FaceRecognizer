#ifndef RACECLASSIFIER_H
#define RACECLASSIFIER_H

#include <classifier.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv::ml;

class RaceClassifier: public Classifier
{
    public:
        RaceClassifier();
        RaceClassifier(string model_path);
        unsigned classify(Face &face, double &confidence);

    private:
        string color_model_path = "models/race/hSvModel.yml";
        string bw_model_path = "models/race/BlackWhite88.yml";
        string wa_model_path = "models/race/AsianWhite64Eqr.yml";
        Ptr<FaceRecognizer> wa_model;
        Ptr<FaceRecognizer> bw_model;
        Ptr<SVM> color_model;
        int imwidth;
        int imheight;
        float c_threshold=1.15;
        int t_threshold=180;
};

#endif // RACECLASSIFIER_H
