//  Copyright (C) 2014, Intel Corporation, all rights reserved.

#ifndef sample_Location_h
#define sample_Location_h
#include <string>
#include <vector>
#import <vmf/vmf.hpp>
#import <CoreLocation/CLLocation.h>
#include <stdio.h>

typedef struct LocationData
{
    CLLocationCoordinate2D coordinate;
    CLLocationAccuracy hAccuracy;
    CLLocationDistance altitude;
    CLLocationSpeed speed;
    long long time;
} LocationData;

#endif
