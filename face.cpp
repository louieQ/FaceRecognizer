#include <face.hpp>

Face::Face(const Mat &image, Rect position) {
    this->position = position;
    this->face = image(position);
}


Mat Face::getFace() {
    return this->face;
}

Rect Face::getPosition() {
    return this->position;
}

Circle Face::getLeftEye() {
    return this->eye_left;
}

Circle Face::getRightEye() {
    return eye_right;
}

Gender Face::getGender() {
    return this->gender;
}

Race Face::getRace() {
    return this->race;
}

Age Face::getAge() {
    return this->age;
}

bool Face::getBeard() {
    return this->beard;
}

void Face::setPosition(Rect position) {
    this->position = position;
}

void Face::setGender(Gender gender) {
    if ((int)gender_temp.size() < nprevisions) {
        gender_temp.push_back(gender);
        int c=0, max=0;
        for (std::vector<Gender>::iterator ci = gender_temp.begin(); ci != gender_temp.end(); ++ci) {
            int counts = std::count(gender_temp.begin(), gender_temp.end(), *ci);
            if (counts > max) {
                max = counts;
                c = *ci;
            }
        }
        this->gender = (Gender)c;
    }
}

void Face::setRace(Race race){
    if ((int)race_temp.size() < nprevisions) {
        race_temp.push_back(race);
        int c=0, max=0;
        for (std::vector<Race>::iterator ci = race_temp.begin(); ci != race_temp.end(); ++ci) {
            int counts = std::count(race_temp.begin(), race_temp.end(), *ci);
            if (counts > max) {
                max = counts;
                c = *ci;
            }
        }
        this->race = (Race)c;
    }
}

void Face::setAge(Age age) {
    if ((int)age_temp.size() < nprevisions) {
        age_temp.push_back(age);
        int c=0, max=0;
        for (std::vector<Age>::iterator ci = age_temp.begin(); ci != age_temp.end(); ++ci) {
            int counts = std::count(age_temp.begin(), age_temp.end(), *ci);
            if (counts > max) {
                max = counts;
                c = *ci;
            }
        }
        this->age = (Age)c;
    }
}

void Face::setBeard(bool beard) {
    if ((int)beard_temp.size() < nprevisions) {
        beard_temp.push_back(beard);
        bool c=false; int max=0;
        for (std::vector<bool>::iterator ci = beard_temp.begin(); ci != beard_temp.end(); ++ci) {
            int counts = std::count(beard_temp.begin(), beard_temp.end(), *ci);
            if (counts > max) {
                max = counts;
                c = *ci;
            }
        }
        this->beard = beard;
    }
}

bool Face::operator==(Face &face) {
    return this->position.x == face.getPosition().x && this->position.y == face.getPosition().y;
}
