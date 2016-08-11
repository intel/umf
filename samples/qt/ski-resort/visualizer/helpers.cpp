// Copyright (C) 2013, Intel Corporation, all rights reserved.

#include "helpers.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#include <string>

using namespace std;
using namespace vmf;

vmf::umf_integer calculateFrameDelay(double fps)
{
    vmf::umf_integer frameDelay = (vmf::umf_integer) floor(1000.0 / fps);
    return frameDelay;
}

float radians(float degrees)
{
    return degrees * float(M_PI) / 180.0f;
}

float distance(float latitude1, float longitude1, float latitude2, float longitude2)
{
    float lat1, lat2, long1, long2;
    lat1 = radians(latitude1);
    lat2 = radians(latitude2);
    long1 = radians(longitude1);
    long2 = radians(longitude2);
    return 2*EARTH_RADIUS*asin(sqrt(pow(sin((lat2 - lat1) / 2.0f), 2.0f) + cos(lat1) * cos(lat2) * pow(sin((long2 - long1) / 2.0f), 2)));
}
