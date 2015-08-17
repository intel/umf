#ifndef __FACE_RECOGNIZER_H__
#define __FACE_RECOGNIZER_H__

#include <opencv2/opencv.hpp>
#include <vector>

class FaceRecognizer
{
public:
    FaceRecognizer(const cv::String& cascadesPath);

    void detectFaces(const cv::Mat& img, std::vector< cv::Rect_<int> >& faces);

private:
    cv::CascadeClassifier haar_cascade;
    static const cv::String cascadeName;
};

#endif /* __FACE_RECOGNIZER_H__ */
