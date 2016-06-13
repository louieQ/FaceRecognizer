#ifndef FACE_H
#define FACE_H

#include <face_attributes.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <circle.hpp>

using namespace cv;
using namespace std;

class Face
{
    public:
        // Costruttore
        Face(const Mat &image, Rect position);
        Face(const Mat &image, Rect position, Circle eye_left, Circle eye_right) : Face(image, position) { this->eye_left = eye_left;
                                                                                                           this->eye_right = eye_right;}

        // Operatori
        Mat getFace();
        Rect getPosition();
        Gender getGender();
        Race getRace();
        Age getAge();
        bool getBeard();
        void setPosition(Rect);
        Circle getLeftEye();
        Circle getRightEye();
        void setGender(Gender gender);
        void setRace(Race race);
        void setAge(Age age);
        void setBeard(bool);
        bool operator==(Face&);

    private:
        int nprevisions = 30;
        Mat face;
        Rect position;
        Circle eye_left;
        Circle eye_right;
        Gender gender;
        Race race;
        Age age;
        bool beard;
        vector<Gender> gender_temp;
        vector<Race> race_temp;
        vector<Age> age_temp;
        vector<bool> beard_temp;
};

#endif // FACE_H
