//
//  VmfClientConnection.m
//  Encoder Demo
//
//  Created by Itseez on 05/04/16.
//  Copyright © 2016 Intel Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VmfClientConnection.h"
#import "CameraServer.h"
#import "RTSPServer.h"
#include <string>
#include <vector>
#import "LocationData.h"
#define MILLISEC_PER_SEC 1000
#define START_LATITUDE 37.388350
#define START_LONGITUDE -121.964500

@interface VmfClientConnection ()
{
    std::vector<LocationData> gpsDataVector;
    std::vector<LocationData> buffer;
    
    BOOL _vmfMetadataSessionSetup;
    double _radius;
    double _angle;
}
@end

static void onSocket (
                      CFSocketRef s,
                      CFSocketCallBackType callbackType,
                      CFDataRef address,
                      const void *data,
                      void *info
                      )
{
    VmfClientConnection* conn = (__bridge VmfClientConnection*)info;
    switch (callbackType)
    {
        case kCFSocketDataCallBack:
            [conn onSocketData:(CFDataRef) data];
            break;
            
        default:
            NSLog(@"unexpected socket event");
            break;
    }
    
}

@implementation VmfClientConnection

@synthesize dataSocket = _dataSocket;
@synthesize isShutdown = _isShutdown;
@synthesize videoStreamStartTime = _videoStreamStartTime;
@synthesize mdStreamStartTime = _mdStreamStartTime;

+ (VmfClientConnection*) createWithSocket:(CFSocketNativeHandle) s server:(RTSPServer*) server
{
    VmfClientConnection* conn = [VmfClientConnection alloc];
    if ([conn initWithSocket:s Server:server] != nil)
    {
        return conn;
    }
    return nil;
}

- (VmfClientConnection*) initWithSocket:(CFSocketNativeHandle)s Server:(RTSPServer *)server
{
    _vmfMetadataSessionSetup = false;
    locationGenerationTimer = nil;
    _radius = 0;
    _angle = 0;
    _mdStreamStartTime = -1;
    
    _locationManager = [[CLLocationManager alloc] init];
    _locationManager.desiredAccuracy = kCLLocationAccuracyBestForNavigation;
    _locationManager.distanceFilter = kCLDistanceFilterNone;
    _locationManager.pausesLocationUpdatesAutomatically = NO;
    
    CFSocketContext info;
    memset(&info, 0, sizeof(info));
    info.info = (void*)CFBridgingRetain(self);
    
    _dataSocket = CFSocketCreateWithNative(nil, s, kCFSocketDataCallBack, onSocket, &info);
    
    _rls = CFSocketCreateRunLoopSource(nil, _dataSocket, 0);
    CFRunLoopAddSource(CFRunLoopGetMain(), _rls, kCFRunLoopCommonModes);
    
    NSString *msg = @"VMF";
    NSData* msgData = [msg dataUsingEncoding:NSUTF8StringEncoding];
    
    [self sendMessage:msgData];
    
    return self;
}

- (void) onSocketData:(CFDataRef)data
{
    if (CFDataGetLength(data) == 0)
    {
        if (_dataSocket)
        {
            CFSocketInvalidate(_dataSocket);
            _dataSocket = nil;
        }
        
        [[CameraServer server].delegate setIPAddrLabel:@"Connection is lost"];
        _mdStreamStartTime = -1;
        _vmfMetadataSessionSetup = FALSE;
        gpsDataVector.clear();
        buffer.clear();
        
        //if (locationManager)
        //   [locationManager stopUpdatingLocation];
        
        if (locationGenerationTimer)
        {
            [locationGenerationTimer invalidate];
            locationGenerationTimer = nil;
        }
        
        _isShutdown = true;
        return;
    }
    
    NSString* msg = [[NSString alloc] initWithData:(__bridge NSData*)data encoding:NSUTF8StringEncoding];
    NSArray* lines = [msg componentsSeparatedByString:@"\r\n"];
    if ([lines count] > 1)
    {
        NSLog(@"msg parse error");
    }
    
    NSArray* lineone = [[lines objectAtIndex:0] componentsSeparatedByString:@" "];
    if ([lineone count] > 1)
    {
        NSLog(@"msg parse error");
    }
    
    NSString* clientResponse = [lineone objectAtIndex:0];
    
    if (([clientResponse caseInsensitiveCompare:@"VMF"] == NSOrderedSame))
    {
        NSString *msg = @"XML";
        NSData* msgData = [msg dataUsingEncoding:NSUTF8StringEncoding];
        
        [self sendMessage:msgData];
    }
    else if (([clientResponse caseInsensitiveCompare:@"OK"] == NSOrderedSame))
    {
        buffer.clear();
        gpsDataVector.clear();
        
        [[CameraServer server].delegate setIPAddrLabel:@"Metadata session is setup"];
        
        //if (locationManager)
        //  [locationManager startUpdatingLocation];
        
        if (locationGenerationTimer)
        {
            [locationGenerationTimer invalidate];
            locationGenerationTimer = nil;
        }
        
        _vmfMetadataSessionSetup = true;
        
        _mdStreamStartTime = vmf::getTimestamp();
        vmf::FormatXML xml;
        
        if (_videoStreamStartTime < 0)
            throw "Start time of video streaming isn't initialized!";
        
        std::shared_ptr<vmf::MetadataStream::VideoSegment> segment = std::make_shared<vmf::MetadataStream::VideoSegment>("iOS", 25, _videoStreamStartTime, 0, 720, 480);
        std::shared_ptr<vmf::MetadataSchema> spSchema = vmf::MetadataSchema::getStdSchema();
        
        std::string segmentStr = xml.store({}, {}, {segment});
        std::string schemaStr = xml.store({}, {spSchema});
        
        NSString *segmentMsg = [NSString stringWithUTF8String:segmentStr.c_str()];
        NSData* msgSegData = [segmentMsg dataUsingEncoding:NSUTF8StringEncoding];
        
        [self sendMessage:msgSegData];
        
        NSString *schemaMsg = [NSString stringWithUTF8String:schemaStr.c_str()];
        NSData* msgSchemaData = [schemaMsg dataUsingEncoding:NSUTF8StringEncoding];
        
        [self sendMessage:msgSchemaData];
        
        locationGenerationTimer = [NSTimer scheduledTimerWithTimeInterval: 1.0 target: self selector:@selector(sendNewLocationOnTimer:) userInfo: nil repeats:YES];
    }

}

- (void) sendNewLocationOnTimer:(NSTimer*)timer
{
    LocationData newLocation;
   
    newLocation.coordinate.latitude = (START_LATITUDE + _radius*cos(_angle));
    newLocation.coordinate.longitude = (START_LONGITUDE + _radius*sin(_angle));
    _radius += 0.0002;
    _angle += M_PI/20;
    
    std::shared_ptr<vmf::MetadataSchema> spSchema = vmf::MetadataSchema::getStdSchema();
    
    auto spLocationMetadata = std::make_shared<vmf::Metadata>(spSchema->findMetadataDesc("location"));
    
    spLocationMetadata->setFieldValue("longitude", newLocation.coordinate.longitude);
    spLocationMetadata->setFieldValue("latitude", newLocation.coordinate.latitude);
    spLocationMetadata->setFieldValue("altitude", 8);
    spLocationMetadata->setFieldValue("accuracy", 100);
    spLocationMetadata->setFieldValue("speed", 2);
    spLocationMetadata->setTimestamp(vmf::getTimestamp());
    
    vmf::FormatXML xml;
    vmf::MetadataSet set;
    
    set.push_back(spLocationMetadata);
    std::string mdStr = xml.store(set);
    
    NSString *msg = [NSString stringWithUTF8String:mdStr.c_str()];
    NSData* msgData = [msg dataUsingEncoding:NSUTF8StringEncoding];
    
    [self sendMessage:msgData];
}

- (void) sendMessage:(NSData*) msgData
{
    __uint32_t size = [msgData length];
    __uint32_t bytes = htonl(size);

    NSLog(@"Size of message is %d", size);
    
    CFDataRef msgDataRef = (__bridge CFDataRef)(msgData);
    
    NSData* data = [NSData dataWithBytes: &bytes length: sizeof(bytes)];
    
    if (_dataSocket)
    {
        CFSocketError e = CFSocketSendData(_dataSocket, NULL, (__bridge CFDataRef)(data), 2);
        
        if (e)
        {
            NSLog(@"send %ld", e);
        }
        
        e = CFSocketSendData(_dataSocket, NULL, msgDataRef, 2);
        if (e)
        {
            NSLog(@"send %ld", e);
        }
    }
}

#pragma mark CLLocationManagerDelegate

- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error
{
    NSLog(@"didFailWithError: %@", error);
    UIAlertView *errorAlert = [[UIAlertView alloc]
                               initWithTitle:@"Error" message:@"Failed to Get Your Location" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [errorAlert show];
}


- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation
{
    long long currentTime = vmf::getTimestamp();
    NSLog(@"didUpdateToLocation: %@", newLocation);
    
    LocationData currentLocation;
    long long timeSinceStartInSecond = (currentTime - _mdStreamStartTime)/MILLISEC_PER_SEC;
    
    if (newLocation == nil)
    {
        NSLog(@"newLocation is null pointer");
        return;
    }
    
    NSLog(@"Time stamp from vmf::getTimestamp() = %lld", currentTime);
    NSLog(@"Time stamp from newLocation = %lf", newLocation.timestamp.timeIntervalSince1970);
    currentLocation.time = currentTime;
    currentLocation.coordinate.longitude = newLocation.coordinate.longitude;
    currentLocation.coordinate.latitude = newLocation.coordinate.latitude;
    currentLocation.hAccuracy = newLocation.horizontalAccuracy;
    currentLocation.altitude = newLocation.altitude;
    currentLocation.speed = newLocation.speed;
    
    vmf::FormatXML xml;
    vmf::MetadataSet set;
    
    if (gpsDataVector.empty())
    {
        gpsDataVector.push_back (currentLocation);
        std::shared_ptr<vmf::MetadataSchema> spSchema = vmf::MetadataSchema::getStdSchema();
        
        auto spLocationMetadata = std::make_shared<vmf::Metadata>(spSchema->findMetadataDesc("location"));
        
        spLocationMetadata->setFieldValue("longitude", currentLocation.coordinate.longitude);
        spLocationMetadata->setFieldValue("latitude", currentLocation.coordinate.latitude);
        spLocationMetadata->setFieldValue("altitude", currentLocation.altitude);
        spLocationMetadata->setFieldValue("accuracy", currentLocation.hAccuracy);
        spLocationMetadata->setFieldValue("speed", currentLocation.speed);
        spLocationMetadata->setTimestamp(currentLocation.time);
        
        set.push_back(spLocationMetadata);
        std::string mdStr = xml.store(set);
        
        NSString *msg = [NSString stringWithUTF8String:mdStr.c_str()];
        NSData* msgData = [msg dataUsingEncoding:NSUTF8StringEncoding];
        
        [self sendMessage:msgData];
    }
    else if (timeSinceStartInSecond == ((gpsDataVector.back().time - _mdStreamStartTime)/MILLISEC_PER_SEC))
    {
        buffer.push_back (currentLocation);
    }
    else if ((timeSinceStartInSecond > ((gpsDataVector.back ().time - _mdStreamStartTime)/MILLISEC_PER_SEC)))
    {
        size_t bufSize = buffer.size();
        if (!buffer.empty())
        {
            CLLocationDegrees sumLongitude = currentLocation.coordinate.longitude;
            CLLocationDegrees sumLatitude = currentLocation.coordinate.latitude;
            CLLocationDistance sumAltitude = currentLocation.altitude;
            
            for (size_t i = 0; i < bufSize; i++)
            {
                sumLongitude += buffer[i].coordinate.longitude;
                sumLatitude += buffer[i].coordinate.latitude;
                sumAltitude += buffer[i].altitude;
            }
            
            LocationData averagedForSecond;
            averagedForSecond.coordinate.longitude = sumLongitude/(bufSize + 1);
            averagedForSecond.coordinate.latitude = sumLatitude/(bufSize + 1);
            averagedForSecond.altitude = sumAltitude/(bufSize + 1);
            averagedForSecond.hAccuracy = currentLocation.hAccuracy;
            averagedForSecond.time = currentLocation.time;
            gpsDataVector.push_back (averagedForSecond);
            buffer.clear();
        }
        else
        {
            gpsDataVector.push_back (currentLocation);
        }
        
        std::shared_ptr<vmf::MetadataSchema> spSchema = vmf::MetadataSchema::getStdSchema();
        
        auto spLocationMetadata = std::make_shared<vmf::Metadata>(spSchema->findMetadataDesc("location"));
        
        spLocationMetadata->setFieldValue("longitude", gpsDataVector.back().coordinate.longitude);
        spLocationMetadata->setFieldValue("latitude", gpsDataVector.back().coordinate.latitude);
        spLocationMetadata->setFieldValue("altitude", gpsDataVector.back().altitude);
        spLocationMetadata->setFieldValue("accuracy", gpsDataVector.back().hAccuracy);
        spLocationMetadata->setFieldValue("speed", gpsDataVector.back().speed);
        spLocationMetadata->setTimestamp(gpsDataVector.back().time);
        
        set.push_back(spLocationMetadata);
        std::string mdStr = xml.store(set);
        
        NSString *msg = [NSString stringWithUTF8String:mdStr.c_str()];
        NSData* msgData = [msg dataUsingEncoding:NSUTF8StringEncoding];
        
        [self sendMessage:msgData];
    }
    
}

@end
