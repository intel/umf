#include <QDebug>
#include "face_recognizer.hpp"

const cv::String FaceRecognizer::cascadeName = "haarcascade_frontalface_alt2.xml";

FaceRecognizer::FaceRecognizer(const cv::String& cascadesPath)
{
    qDebug() << "Load face cascades from " << cascadesPath.c_str();

    if (cascadesPath.empty())
    {
        haar_cascade.load("./" + cascadeName);
    }
    else
    {
        haar_cascade.load(cascadesPath + "/" + cascadeName);
    }
}

void FaceRecognizer::detectFaces(const cv::Mat& img, std::vector< cv::Rect_<int> >& faces)
{
    cv::Mat original = img.clone();

    // Convert the current frame to grayscale:
    cv::Mat gray;
    cvtColor(original, gray, cv::COLOR_RGB2GRAY);
    // Find the faces in the frame:
    haar_cascade.detectMultiScale(gray, faces);
}
