// Copyright (C) 2013, Intel Corporation, all rights reserved.
#ifndef __VMF_VIDEOMETADATASTREAM_HPP__
#define __VMF_VIDEOMETADATASTREAM_HPP__

#include <QString>
#include <QMutex>
#include <opencv2/opencv.hpp>
#include "gps_coodinates.hpp"
#include "vmf/vmf.hpp"

class VideoMetadataStream
{
public:
    VideoMetadataStream();
    void openFile(const QString& filename);
    void closeVideo();
    void closeMetadata();
    cv::Mat getCurrentFrame();
    GpsCoordinates getCurrentCoordinates();
    float getCurrentSpeed();
    int getCurrentPercentage();
    int getCurrentFrameNumber();
    vmf::umf_integer getFrameDelay();
    bool hasSpeed();
    bool isOpened();
    void next();
    void seek(int percentage);
    void calculateSpeed();
    void saveSpeed();
private:
    void init();
    void updateSpeed();
    void updateGpsCoordinates();

    std::string fileName;
    bool isFileOpened;
    cv::VideoCapture capture;
    cv::Mat currentFrame;
    GpsCoordinates currentCoordinates;
    vmf::umf_integer currentTime;
    float currentSpeed;
    double fps;
    vmf::umf_integer frameDelay;
    double duration;
    vmf::MetadataStream metaStream;
    size_t currentGpsCoordinateIndex;
    bool hasSpeedValues;
    vmf::MetadataSet gpsCoordinatesSet;
    vmf::MetadataSet speedValuesSet;
    QMutex mutex;
};

#endif // VIDEOMETADATASTREAM_H
