#include <ageclassifier.hpp>

AgeClassifier::AgeClassifier(bool high_performances)
{
    this->high_performances = high_performances;

    if (!high_performances)
    {
        imwidth = 88; imheight= 88;
        model = createFisherFaceRecognizer();
        FileStorage fs(low_performances_model_path, FileStorage::READ);
        model->load(fs);
    }
    else {
        imwidth=224; imheight=224;
        // Importa il modello deep learning
        String age_modelTxt = "/home/andrea/Scaricati/age.prototxt";
        String age_modelBin = "/home/andrea/Scaricati/age.caffemodel";


        //! [Create the importer of Caffe model]
        Ptr<dnn::Importer> importer;
        try {                                    //Try to import Caffe GoogleNet model
            importer = dnn::createCaffeImporter(age_modelTxt, age_modelBin);
        }
        catch (const cv::Exception &err)        //Importer can throw errors, we will catch them
        {
            std::cerr << err.msg << std::endl;
        }
        // [Create the importer of Caffe model]

        if (!importer)
        {
            std::cerr << "Can't load network by using the following files: " << std::endl;
            std::cerr << "prototxt:   " << age_modelTxt << std::endl;
            std::cerr << "caffemodel: " << age_modelBin << std::endl;
            exit(-1);
        }

        // [Initialize network]
        importer->populateNet(net);
        importer.release();
        //We don't need importer anymore
    }
}

AgeClassifier::AgeClassifier(string model_path){
    model = createFisherFaceRecognizer();
    FileStorage fs(model_path, FileStorage::READ);
    model->load(fs);
}


unsigned AgeClassifier::classify(Face &face, double &confidence) {
    // Preprocess color
    Mat face_resized;
    int prediction = -1;
    if (!high_performances) {
        Mat gray;
        cvtColor(face.getFace(), gray, CV_BGR2GRAY);
        cv::resize(gray, face_resized, Size(imwidth, imheight), 1.0, 1.0, INTER_CUBIC);
        model->predict(face_resized, prediction, confidence);
        //model->predict(face.getFace(), prediction, confidence);
        face.setAge((Age)prediction);
    }
    else {
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
            prediction = classNumber.x;
        }
    }


    return prediction;
}

void AgeClassifier::setModel(string model_path) {
    model = createFisherFaceRecognizer();
    FileStorage fs(model_path, FileStorage::READ);
    model->load(fs);
}
