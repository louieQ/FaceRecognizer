#include <scene.hpp>
#include <iostream>


Scene::Scene()
{
    haar_cascade.load(fn_haar);
    nestedCascade.load(fn_nested_haar);
}


vector<Face>* Scene::getFaces() {
    return &(this->faces);
}


void Scene::detect(Mat &frame) {
    //cout << "Faces: " << faces->size() << endl;
    this->frame = frame;
    Mat gray;
    cvtColor(frame, gray, CV_BGR2GRAY);
    equalizeHist(gray, gray);

    // Face detector
    vector< Rect_<int> > rects;
    haar_cascade.detectMultiScale(gray, rects, 1.1, 3, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
    bool found = false;
    unsigned nfaces = faces.size();
    vector< Face > updated;
    for (size_t i=0; i<rects.size(); i++) {
        // Eye detector
        vector<Rect> nestedObjects;
        //Mat canvas;
        //frame.copyTo(canvas);
        Point center;
        Scalar color(255,0,0);
        int radius;
        Rect r = rects[i];
        Mat smallImgROI = gray(r);
        nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
            1.1, 2, 0
            //|CASCADE_FIND_BIGGEST_OBJECT
            //|CASCADE_DO_ROUGH_SEARCH
            //|CASCADE_DO_CANNY_PRUNING
            |CASCADE_SCALE_IMAGE,
            Size(20, 20));

        for ( size_t j = 0; j < nestedObjects.size(); j++ ) {
            Rect nr = nestedObjects[j];
            center.x = cvRound((r.x + nr.x + nr.width*0.5));
            center.y = cvRound((r.y + nr.y + nr.height*0.5));
            radius = cvRound((nr.width + nr.height)*0.25);
        }
        int m=0;
        unsigned min = INT_MAX;
        for (size_t j=0; j<faces.size(); j++) {
            unsigned distance = abs(rects[i].x - faces.at(j).getPosition().x) + abs(rects[i].y - faces.at(j).getPosition().y);
            //std::cout << "Distance: " << distance << std::endl;
            if (distance < min) {
                min = distance;
                m=j;
            }
        }
        if (min < 100) {
            // Aggiorna la posizione
            faces.at(m).setPosition(rects[i]);
            updated.push_back(faces.at(m));
        }
        //cout << "Updated: " << updated.size() << endl;
        else {
            Face face(frame, rects[i]);
            faces.push_back(face);
            //cout << "Created." << endl;
        }
    }
    // Cancellazione
    vector<int> delete_list;
    for (unsigned k=0; k<nfaces; k++) {
        for (size_t i=0; i<updated.size(); i++) {
            if (updated[i] == faces.at(k)) {
                found = true;
                break;
            }
        }
        if (!found) {
            delete_list.push_back(k);
        }
        found = false;
    }
    for (size_t i=0; i<delete_list.size(); i++) {
         int k = delete_list.at(i);
         faces.erase(faces.begin()+(k-i));
    }
}

void Scene::draw(string parameters) {
    for (size_t i=0; i<faces.size(); i++) {
        string output = "";
        rectangle(frame, faces.at(i).getPosition(), CV_RGB(0, 255,0), 1);
        if (faces.at(i).getLeftEye().getRadius() != 0) {
            circle(frame, faces.at(i).getLeftEye().getCenter(), faces.at(i).getLeftEye().getRadius(), CV_RGB(0, 255,0), 3, 8, 0 );
            circle(frame, faces.at(i).getRightEye().getCenter(), faces.at(i).getRightEye().getRadius(), CV_RGB(0, 255,0), 3, 8, 0 );
        }
        if (parameters.find("-g") != std::string::npos) {
            output += "G=";
            (faces.at(i).getGender()==Male) ? output+="Male " : output+="Female ";
        }
        if (parameters.find("-r") != std::string::npos) {
            output += "R=";
            (faces.at(i).getRace()==Black) ? output+="Black " : (faces.at(i).getRace()==White) ? output+="White " : output +="Asian ";
        }
        if (parameters.find("-a") != std::string::npos) {
            output += "A=";
            switch(faces.at(i).getAge()) {
                case Kid: output+="Kid "; break;
                case Teen: output+="Teen "; break;
                case Young: output+="Young "; break;
                case Mature: output+="Mature "; break;
                case Old: output+="Old "; break;
            }
        }
        if (parameters.find("-b") != std::string::npos) {
            output += "B=";
            (faces.at(i).getBeard()) ? output+="yes " : output+="no ";
        }
        int pos_x = std::max(faces.at(i).getPosition().x - 10, 0);
        int pos_y = std::max(faces.at(i).getPosition().y - 10, 0);
        putText(frame, output, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
    }
}

