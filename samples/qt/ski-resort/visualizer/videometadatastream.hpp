// Copyright (C) 2013, Intel Corporation, all rights reserved.
#ifndef __UMF_VIDEOMETADATASTREAM_HPP__
#define __UMF_VIDEOMETADATASTREAM_HPP__

#include <QString>
#include <QMutex>
#include <opencv2/opencv.hpp>
#include "gps_coodinates.hpp"
#include "umf/umf.hpp"

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
    umf::umf_integer getFrameDelay();
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
    umf::umf_integer currentTime;
    float currentSpeed;
    double fps;
    umf::umf_integer frameDelay;
    double duration;
    umf::MetadataStream metaStream;
    size_t currentGpsCoordinateIndex;
    bool hasSpeedValues;
    umf::MetadataSet gpsCoordinatesSet;
    umf::MetadataSet speedValuesSet;
    QMutex mutex;
};

#endif // VIDEOMETADATASTREAM_H
