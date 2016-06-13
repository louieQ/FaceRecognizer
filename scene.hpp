#ifndef SCENE_H
#define SCENE_H

#include <opencv2/core/core.hpp>
#include <opencv2/face.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <face.hpp>

using namespace cv;
using namespace std;

class Scene
{
    public:
        Scene();
        //Scene(Mat);

        vector<Face>* getFaces();
        void detect(Mat &);
        void draw(string parameters);

    private:
        Mat frame;
        vector<Face> faces;
        string fn_haar = "models/face/haarcascade_frontalface_alt.xml";
        string fn_nested_haar = "models/face/haarcascade_eye_tree_eyeglasses.xml";
        CascadeClassifier haar_cascade;
        CascadeClassifier nestedCascade;
};

#endif // SCENE_H
