#include <genderclassifier.hpp>

GenderClassifier::GenderClassifier(bool high_performances)
{
    this->high_performances = high_performances;
    if (!high_performances) {
        imwidth = 64; imheight= 64;
        model = createFisherFaceRecognizer();
        FileStorage fs(model_path, FileStorage::READ);
        model->load(fs);
    }
    else {
        imwidth=224; imheight=224;



        //! [Create the importer of Caffe model]
        Ptr<dnn::Importer> importer;
        try {                                    //Try to import Caffe GoogleNet model
            importer = dnn::createCaffeImporter(deep_paths[0], deep_paths[1]);
        }
        catch (const cv::Exception &err)        //Importer can throw errors, we will catch them
        {
            std::cerr << err.msg << std::endl;
        }
        // [Create the importer of Caffe model]

        if (!importer)
        {
            std::cerr << "Can't load network by using the following files: " << std::endl;
            std::cerr << "prototxt:   " << deep_paths[0] << std::endl;
            std::cerr << "caffemodel: " << deep_paths[1] << std::endl;
            exit(-1);
        }

        // [Initialize network]
        importer->populateNet(net);
        importer.release();
        //We don't need importer anymore
    }
}


GenderClassifier::GenderClassifier(string model_path)
{
    model = createFisherFaceRecognizer();
    FileStorage fs(model_path, FileStorage::READ);
    model->load(fs);
}

unsigned GenderClassifier::classify(Face &face, double &confidence) {
    Mat face_resized;
    int prediction = -1;
    if (!high_performances) {
        Mat gray;
        cvtColor(face.getFace(), gray, CV_BGR2GRAY);
        equalizeHist(gray, gray);
        cv::resize(gray, face_resized, Size(imwidth, imheight), 1.0, 1.0, INTER_CUBIC);
        equalizeHist(face_resized, face_resized);
        model->predict(face_resized, prediction, confidence);
    }
    else {
        // Effettua la predizione con il modello deep learning
        cv::resize(face.getFace(), face_resized, Size(imwidth, imheight));
        if (face_resized.total() != 0) {
            // [Prepare blob]
            Blob inputBlob = Blob(face_resized);   //Convert Mat to dnn::Blob image batch

            //! [Set input blob]
            net.setBlob(".data", inputBlob);        //set the network input

            // [Make forward pass]
            net.forward();                          //compute output
            //! [Make forward pass]

            // [Gather output]
            Blob probBlob = net.getBlob("prob");   //gather output of "prob" layer

            Mat probMat = probBlob.matRefConst().reshape(1, 1); //reshape the blob to 1x1000 matrix
            Point classNumber;
            minMaxLoc(probMat, NULL, &confidence, NULL, &classNumber);
            prediction = abs(classNumber.x -1);
            cout << prediction << endl;
        }
    }
    face.setGender((Gender)prediction);
    return prediction;
}

