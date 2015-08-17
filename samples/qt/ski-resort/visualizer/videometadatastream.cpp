// Copyright (C) 2013, Intel Corporation, all rights reserved.
#include "videometadatastream.hpp"

#include <algorithm>
#include <string>
#include <cmath>

#include <QMutexLocker>

#include "helpers.hpp"

#define SKI_RESORT_SCHEMA "ski-resort"
#define SKI_RESORT_GPS_DESC "gps-data"
#define SKI_RESORT_GPS_FIELD_X "x"
#define SKI_RESORT_GPS_FIELD_Y "y"
#define SKI_RESORT_SPEED_DESC "speed"
#define SKI_RESORT_GPS_FIELD_TIME   "time"
#define SKI_RESORT_SPEED_FIELD_SPEED    "speed"
#define SKI_RESORT_SPEED_FIELD_TIME     "time"

#define MSEC_IN_SEC 1000
#define HOURS_IN_SEC (1.0/(60.0 * 60.0))

using namespace std;
using namespace cv;
using namespace vmf;

VideoMetadataStream::VideoMetadataStream()
  : mutex(QMutex::Recursive)
{
    init();
}

void VideoMetadataStream::init()
{
    isFileOpened = false;
    currentFrame = cv::Mat();
    currentCoordinates = make_pair(0.0f, 0.0f);
    currentSpeed = 0.0f;
    currentGpsCoordinateIndex = 0;
    currentTime = 0;
    hasSpeedValues = false;
}

void VideoMetadataStream::openFile(const QString& filename)
{
    QMutexLocker autoLock(&mutex);
    init();
    fileName = filename.toStdString();
    try
    {
        if (!metaStream.open(fileName, vmf::MetadataStream::ReadWrite))
        {
            VMF_EXCEPTION(vmf::NotFoundException, "Not found");
        }

        if (!metaStream.load(SKI_RESORT_SCHEMA))
        {
            VMF_EXCEPTION(vmf::DataStorageException, "Can't load metadata schema");
        }

        gpsCoordinatesSet = metaStream.queryByName(SKI_RESORT_GPS_DESC);
        speedValuesSet = metaStream.queryByName(SKI_RESORT_SPEED_DESC);

        if (!speedValuesSet.empty())
        {
            hasSpeedValues = true;
        }

        metaStream.close();

        if (!capture.open(fileName))
        {
            throw std::runtime_error("Not a video file.");
        }

        fps = capture.get(CV_CAP_PROP_FPS);
        if (fps == 0.0)
        {
            throw std::runtime_error("Cannot get FPS of specified video file.");
        }
        frameDelay = calculateFrameDelay(fps);
        next();

        isFileOpened = true;
    }
    catch (const vmf::Exception&)
    {
        throw std::runtime_error("Needed metadata doesn't exist or corrupted.");
    }
}

void VideoMetadataStream::closeVideo()
{
    QMutexLocker autoLock(&mutex);
    isFileOpened = false;
    currentFrame = cv::Mat();
    currentSpeed = 0.0f;
    currentCoordinates = make_pair(0.0f, 0.0f);
    capture.release();
}

void VideoMetadataStream::closeMetadata()
{
    metaStream.close();
}

Mat VideoMetadataStream::getCurrentFrame()
{
    QMutexLocker autoLock(&mutex);
    Mat frame;
    if (currentFrame.empty())
    {
        return currentFrame.clone();
    }

    cv::cvtColor(currentFrame, frame, COLOR_BGR2RGB);
    return frame;
}

GpsCoordinates VideoMetadataStream::getCurrentCoordinates()
{
    QMutexLocker autoLock(&mutex);
    return currentCoordinates;
}

float VideoMetadataStream::getCurrentSpeed()
{
    QMutexLocker autoLock(&mutex);
    return currentSpeed;
}

vmf::vmf_integer VideoMetadataStream::getFrameDelay()
{
    QMutexLocker autoLock(&mutex);
    return frameDelay;
}

bool VideoMetadataStream::hasSpeed()
{
    QMutexLocker autoLock(&mutex);
    return hasSpeedValues;
}

bool VideoMetadataStream::isOpened()
{
    return isFileOpened;
}

int VideoMetadataStream::getCurrentFrameNumber()
{
    int frameNumber = floor(capture.get(CV_CAP_PROP_POS_FRAMES));
    return frameNumber;
}

int VideoMetadataStream::getCurrentPercentage()
{
    QMutexLocker autoLock(&mutex);
    int ratio = floor(100*capture.get(CV_CAP_PROP_POS_FRAMES)/capture.get(CV_CAP_PROP_FRAME_COUNT));
    ratio = MAX(0, ratio);
    ratio = MIN(ratio, 100);
    return ratio;
}

void VideoMetadataStream::next()
{
    QMutexLocker autoLock(&mutex);
    capture >> currentFrame;
    currentTime = currentTime + frameDelay;

    for (; currentGpsCoordinateIndex < gpsCoordinatesSet.size(); currentGpsCoordinateIndex++)
    {
        auto item = gpsCoordinatesSet[currentGpsCoordinateIndex];
        vmf::vmf_integer time = item->getFieldValue(SKI_RESORT_GPS_FIELD_TIME);
        
        if (time >= currentTime)
        {
            break;
        }
    }

    // return to less then
    if (currentGpsCoordinateIndex != 0)
        currentGpsCoordinateIndex--;
    auto currentGpsCoordinateItem = gpsCoordinatesSet[currentGpsCoordinateIndex];
    
    currentCoordinates.first = (float) (vmf::vmf_real) currentGpsCoordinateItem->getFieldValue(SKI_RESORT_GPS_FIELD_X);
    currentCoordinates.second = (float) (vmf::vmf_real) currentGpsCoordinateItem->getFieldValue(SKI_RESORT_GPS_FIELD_Y);
    updateSpeed();
}

void VideoMetadataStream::seek(int percentage)
{
    QMutexLocker autoLock(&mutex);
    if (!isFileOpened)
    {
        return;
    }
    if (percentage == getCurrentPercentage())
    {
        //nothing to do
        return;
    }

    double totalFrames = capture.get(CV_CAP_PROP_FRAME_COUNT);
    double newFramePosition = totalFrames*percentage/100;
    capture.set(CV_CAP_PROP_POS_FRAMES, newFramePosition);
    capture.retrieve(currentFrame);
    vmf::vmf_integer msecFromStart = floor(capture.get(CV_CAP_PROP_POS_MSEC));
    currentTime = msecFromStart;

    for(currentGpsCoordinateIndex = 0;
        currentGpsCoordinateIndex < gpsCoordinatesSet.size();
        ++currentGpsCoordinateIndex)
    {
        auto item = gpsCoordinatesSet[currentGpsCoordinateIndex];
        vmf::vmf_integer time = item->getFieldValue(SKI_RESORT_GPS_FIELD_TIME);

        if (currentTime < time)
        {
            break;
        }
    }

    // return to less then
    if (currentGpsCoordinateIndex)
        currentGpsCoordinateIndex--;

    auto currentGpsCoordinateItem = gpsCoordinatesSet[currentGpsCoordinateIndex];

    currentCoordinates.first = (float) (vmf::vmf_real) currentGpsCoordinateItem->getFieldValue(SKI_RESORT_GPS_FIELD_X);
    currentCoordinates.second = (float) (vmf::vmf_real) currentGpsCoordinateItem->getFieldValue(SKI_RESORT_GPS_FIELD_Y);

    updateSpeed();
}

void VideoMetadataStream::calculateSpeed()
{
    QMutexLocker autoLock(&mutex);
    if (hasSpeedValues)
    {
        return;
    }

    size_t gpsCoordinatesNumber = gpsCoordinatesSet.size();
    auto schema = metaStream.getSchema(SKI_RESORT_SCHEMA);

    if (!schema)
    {
        return;
    }

    auto speedDesc = schema->findMetadataDesc(SKI_RESORT_SPEED_DESC);

    if (gpsCoordinatesNumber >= 2)
    {
        for(size_t i = 0; i < gpsCoordinatesNumber - 1; ++i)
        {
            auto currentCoordinatesItem = gpsCoordinatesSet[i];
            auto nextCoordinatesItem = gpsCoordinatesSet[i+1];

            float x1, x2, y1, y2;
            x1 = (float) (vmf::vmf_real) currentCoordinatesItem->getFieldValue(SKI_RESORT_GPS_FIELD_X);
            y1 = (float) (vmf::vmf_real) currentCoordinatesItem->getFieldValue(SKI_RESORT_GPS_FIELD_Y);
            x2 = (float) (vmf::vmf_real) nextCoordinatesItem->getFieldValue(SKI_RESORT_GPS_FIELD_X);
            y2 = (float) (vmf::vmf_real) nextCoordinatesItem->getFieldValue(SKI_RESORT_GPS_FIELD_Y);

            float meters = distance(x1, y1, x2, y2);
            vmf::vmf_integer t1 = currentCoordinatesItem->getFieldValue(SKI_RESORT_GPS_FIELD_TIME);
            vmf::vmf_integer t2 = nextCoordinatesItem->getFieldValue(SKI_RESORT_GPS_FIELD_TIME);
            vmf::vmf_integer timeDiff = t2 - t1;

            float seconds = (float) (timeDiff / 1000);
            float speed = (meters * KMS_IN_METER) / (seconds * HOURS_IN_SEC);

            std::shared_ptr<vmf::Metadata> speedMetadata(new vmf::Metadata(speedDesc));
            speedMetadata->setFieldValue(SKI_RESORT_SPEED_FIELD_SPEED, (vmf::vmf_real) speed);
            speedMetadata->setFieldValue(SKI_RESORT_SPEED_FIELD_TIME, (vmf::vmf_integer) t1);

            metaStream.add(speedMetadata);
        }
    }

    if (gpsCoordinatesNumber >= 1)
    {
        auto item = gpsCoordinatesSet[gpsCoordinatesNumber-1];
        vmf::vmf_integer t = item->getFieldValue(SKI_RESORT_GPS_FIELD_TIME);

        std::shared_ptr<vmf::Metadata> speedMetadata(new vmf::Metadata(speedDesc));
        speedMetadata->setFieldValue(SKI_RESORT_SPEED_FIELD_SPEED, (vmf::vmf_real) 0);
        speedMetadata->setFieldValue(SKI_RESORT_SPEED_FIELD_TIME, (vmf::vmf_integer) t);

        metaStream.add(speedMetadata);
    }

    speedValuesSet = metaStream.queryByName(SKI_RESORT_SPEED_DESC);
    updateSpeed();
    hasSpeedValues = true;
}

void VideoMetadataStream::saveSpeed()
{
    double savedPosition = capture.get(CV_CAP_PROP_POS_FRAMES);
    currentFrame = currentFrame.clone();
    capture.release();
    
    metaStream.reopen(vmf::MetadataStream::ReadWrite);
    metaStream.save();
    metaStream.close();

    capture.open(fileName);
    capture.set(CV_CAP_PROP_POS_FRAMES, savedPosition);
}

void VideoMetadataStream::updateSpeed()
{
    if (!hasSpeedValues)
    {
        return;
    }

    auto currentSpeedItem = speedValuesSet[currentGpsCoordinateIndex];
    currentSpeed = (float) (vmf::vmf_real) currentSpeedItem->getFieldValue(SKI_RESORT_SPEED_FIELD_SPEED);
}
