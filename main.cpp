#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <scene.hpp>
#include <face.hpp>
#include <genderclassifier.hpp>
#include <raceclassifier.hpp>
#include <ageclassifier.hpp>


#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


string classifyAll(vector<Face> *faces, string parameters) {
	string json = "";
	double confidence;
	for (vector<Face>::iterator i=faces->begin(); i != faces->end(); i++) {
		json += "{ ";
        Rect position = (*i).getPosition();
        json += "position: [{ x:" + to_string(position.x) + ", y:" + to_string(position.y) + " }, " + "{ x:" + to_string(position.br().x) + ", y:" + to_string(position.br().y) + " }]";;
		if (parameters.find("-g") != std::string::npos) {
			json += ", gender: [";
			GenderClassifier g;
			g.classify((*i), confidence);
            ((*i).getGender()==Male) ? json+="\"male\", " : json+="\"female\", ";
            json += to_string(confidence) + "]";
		}
		if (parameters.find("-r") != std::string::npos) {
			json += ", race: [";
			RaceClassifier r;
			r.classify((*i), confidence);
            ((*i).getRace()==Black) ? json+="\"black\", " : ((*i).getRace()==White) ? json+="\"white\", " : json +="\"asian\", ";
            json += to_string(confidence) + "]";
		}
		if (parameters.find("-a") != std::string::npos) {
			json += ", age: [";
			AgeClassifier a;
			a.classify((*i), confidence);
            switch((*i).getAge()) {
				case Kid: json+="\"kid\", "; break;
				case Teen: json+="\"teen\", "; break;
				case Young: json+="\"young\", "; break;
				case Mature: json+="\"mature\", "; break;
				case Old: json+="\"old\", "; break;
			}
            json += to_string(confidence) + "]";
		}
        json+=" } ";
	}
	//
	return json;
}


int main(int argc, char *argv[])
{
	string json; // main parameter?
	//
	if (argc < 3) {
		cerr << "Error: do you miss some arguments?" << endl;
		return -1;
	}
	// Interpret parameters
	string input = argv[1];
	if (input != "-vid" && input != "-img" && input != "-cam") {
		cerr << "Error: " << input << ". Input format incorrect! Check the documentation and try again." << endl;
		return -1;
	}

	string parameters = "";
    for (int i=3; i<argc; i++) {
		string request = argv[i];
        if (request != "-g" && request != "-r" && request != "-a" &&
            request != "-gh" && request != "-ah") {
            cerr << "Error: " << request << ". Recognition parameter is wrong. Check the documentation and try again." << endl;
            return -1;
        }
        else {
            parameters += request;
        }
    }

	// Biometrics recognition
	Mat frame;
	Scene scene;
	vector<Face> *faces;
	if (input == "-img") {
		frame = imread(argv[2], CV_LOAD_IMAGE_COLOR);
        //Mat original = frame.clone();
        scene.detect(frame);
        faces = scene.getFaces();
        json = classifyAll(faces, parameters);
        // DELETE! IT'S JUST FOR TESTING
        cout << "These are the json objects: \n" << json << endl;
	}
	if (input == "-vid") {
		// Video recognition code
		// work in progress
	}
	else
	if (input == "-cam") {
		// cast string to int
        int deviceId = atoi(argv[2]);
	    VideoCapture cap(deviceId);
	    if(!cap.isOpened()) {
	        cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
	        return -1;
	    }
	    cap.set(CAP_PROP_FRAME_WIDTH, 400);
	    cap.set(CAP_PROP_FRAME_WIDTH, 400);
	    // Holds the current frame from the Video device:
	    for(;;) {
	        cap >> frame;
	        Mat original = frame.clone();
	        scene.detect(original);
	        faces = scene.getFaces();
	        json = classifyAll(faces, parameters);
            scene.draw(parameters);
	        imshow("face_recognizer", original);
	        char key = (char) waitKey(10);
	        if(key == 27)
	            break;
	    }
    }
    //
    return 0;
}
