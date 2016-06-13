#include <raceclassifier.hpp>

RaceClassifier::RaceClassifier()
{
    // Inizializza il modello per distinguere tra Black e White
    bw_model = createFisherFaceRecognizer();
    FileStorage fsbw(bw_model_path, FileStorage::READ);
    bw_model->load(fsbw);

    // Inizializza il modello per distinguere tra White e Asian
    wa_model = createFisherFaceRecognizer();
    FileStorage fswa(wa_model_path, FileStorage::READ);
    wa_model->load(fswa);

    // Inizializza il modello sul colore
    color_model = SVM::load<SVM>(color_model_path);
    imwidth=64; imheight=64;
}

unsigned RaceClassifier::classify(Face &face, double &confidence) {
    // Preprocess the color space
    Mat hsv;
    cvtColor(face.getFace(), hsv, CV_BGR2HSV);

    // Cut the nose zone
    Rect face_i = Rect(0,0,face.getFace().rows, face.getFace().cols);
    Rect nose(Point(face_i.x + face_i.width/5,face_i.y + face_i.height/2), Point(face_i.br().x-face_i.width/5,face_i.br().y-face_i.height/3));
    Mat skin(hsv,nose);
    Scalar color = mean(skin);
    Mat svm_input(1,2,CV_32F);
    for (int j=0; j<2; j++) {
        svm_input.at<float>(0,j) = color[j];
    }
    // Color prediction
    int color_prediction = color_model->predict(svm_input);
    float color_confidence = color_model->predict(svm_input, noArray(), StatModel::RAW_OUTPUT);
    color_confidence = abs(color_confidence);
    if (color_confidence > c_threshold)
        color_confidence = c_threshold;
    color_confidence /= c_threshold;

    // Trait prediction
    Mat gray, face_resized;
    cvtColor(face.getFace(), gray, CV_BGR2GRAY);
    cv::resize(gray, face_resized, Size(88, 88), 1.0, 1.0, INTER_CUBIC);
    int traits_prediction = -1;
    double traits_confidence = 0;
    bw_model->predict(face_resized, traits_prediction, traits_confidence);
    if (traits_confidence>t_threshold)
        traits_confidence = t_threshold;
    traits_confidence = 1-traits_confidence/t_threshold;

    // Final computation to classify Black or White/Oriental
    int final_prediction = -1;
    if (color_prediction == traits_prediction)
        final_prediction = color_prediction;
    else
        (color_confidence > traits_confidence) ? final_prediction = color_prediction : final_prediction = traits_prediction;

    if (final_prediction == 1) {
        cv::resize(face_resized, face_resized, Size(imwidth, imheight), 1.0, 1.0, INTER_CUBIC);
        equalizeHist(face_resized, face_resized);
        wa_model->predict(face_resized, final_prediction, traits_confidence);
        final_prediction++;
    }
    face.setRace((Race)final_prediction);

    // TODO confidence???
    confidence = traits_confidence;
    return (Race)final_prediction;
}
