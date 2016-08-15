// Copyright (C) 2013, Intel Corporation, all rights reserved.

#include "umf/umf.hpp"

#define METERS_IN_KM 1000.0f
#define KMS_IN_METER (1 / METERS_IN_KM)
#define EARTH_RADIUS 6371.0f * METERS_IN_KM

umf::umf_integer calculateFrameDelay(double fps);

float radians(float degrees);

float distance(float latitude1, float longitude1, float latitude2, float longitude2);
