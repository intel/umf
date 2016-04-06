//
//  VmfClientConnection.m
//  Encoder Demo
//
//  Created by Itseez on 05/04/16.
//  Copyright © 2016 Geraint Davies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VmfClientConnection.h"
#import "CameraServer.h"
#import "RTSPServer.h"
#include <string>
#include <vector>
#import "Location.h"
#define MILLISEC_PER_SEC 1000

@interface VmfClientConnection ()
{
    std::vector<LocationData> gpsDataVector;
    std::vector<LocationData> buffer;
    std::vector<LocationData> gpsGeneratedData;
    BOOL _vmfMetadataSessionSetup;
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
    LocationData newLocation;
    newLocation.coordinate.latitude = 37.388350;
    newLocation.coordinate.longitude = -121.964500;
    
    for (size_t i = 0; i < 10; i++)
    {
        newLocation.coordinate.latitude -= 0.000045;
        newLocation.coordinate.longitude -= 0.000130;
        gpsGeneratedData.push_back(newLocation);
    }
    
    for (size_t i = 0; i < 10; i++)
    {
        newLocation.coordinate.latitude -= 0.000090;
        newLocation.coordinate.longitude += 0.000050;
        gpsGeneratedData.push_back(newLocation);
    }
    
    for (size_t i = 0; i < 10; i++)
    {
        newLocation.coordinate.latitude += 0.000060;
        newLocation.coordinate.longitude += 0.000150;
        gpsGeneratedData.push_back(newLocation);
    }
    
    for (size_t i = 0; i < 10; i++)
    {
        newLocation.coordinate.latitude += 0.000025;
        newLocation.coordinate.longitude -= 0.000070;
        gpsGeneratedData.push_back(newLocation);
    }
    
    _vmfMetadataSessionSetup = false;
    generatedDataIndex = 0;
    locationGenerationTimer = nil;
    
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
    
    CFDataRef msgDataRef = (__bridge CFDataRef)(msgData);
    
    __uint32_t bytes = CFDataGetLength(msgDataRef);
    
    NSData* data = [NSData dataWithBytes: &bytes length: sizeof(bytes)];
    CFSocketError e = CFSocketSendData(_dataSocket, NULL, (__bridge CFDataRef)(data), 2);
    
    if (e)
    {
        NSLog(@"send size %ld", e);
    }
    
    e = CFSocketSendData(_dataSocket, NULL, msgDataRef, 2);
    
    if (e)
    {
        NSLog(@"send %ld", e);
    }
    
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
        
        generatedDataIndex = 0;
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
        
        CFDataRef msgDataRef = (__bridge CFDataRef)(msgData);
        
        __uint32_t bytes = CFDataGetLength(msgDataRef);
        
        NSData* data = [NSData dataWithBytes: &bytes length: sizeof(bytes)];
        CFSocketError e = CFSocketSendData(_dataSocket, NULL, (__bridge CFDataRef)(data), 2);
        
        if (e)
        {
            NSLog(@"send size %ld", e);
        }
        
        e = CFSocketSendData(_dataSocket, NULL, (__bridge CFDataRef)(msgData), 2);
        if (e)
        {
            NSLog(@"send %ld", e);
        }
    }
    else if (([clientResponse caseInsensitiveCompare:@"OK"] == NSOrderedSame))
    {
        buffer.clear();
        gpsDataVector.clear();
        
        [[CameraServer server].delegate setIPAddrLabel:@"Metadata session is setup"];
        
        //if (locationManager)
        //  [locationManager startUpdatingLocation];
        
        generatedDataIndex = 0;
        
        if (locationGenerationTimer)
        {
            [locationGenerationTimer invalidate];
            locationGenerationTimer = nil;
        }
        
        _vmfMetadataSessionSetup = true;
        vmf::FormatXML xml;
        
        if (_videoStreamStartTime < 0)
            throw "Start time of video streaming isn't initialized!";
        
        std::shared_ptr<vmf::MetadataStream::VideoSegment> segment = std::make_shared<vmf::MetadataStream::VideoSegment>("iOS", 25, _videoStreamStartTime, 0, 720, 480);
        std::shared_ptr<vmf::MetadataSchema> spSchema = vmf::MetadataSchema::getStdSchema();
        
        std::string segmentStr = xml.store({}, {}, {segment});
        std::string schemaStr = xml.store({}, {spSchema});
        
        NSString *segmentMsg = [NSString stringWithUTF8String:segmentStr.c_str()];
        NSData* msgSegData = [segmentMsg dataUsingEncoding:NSUTF8StringEncoding];
        CFDataRef msgSegDataRef = (__bridge CFDataRef)(msgSegData);
        
        __uint32_t segBytes = CFDataGetLength(msgSegDataRef);
        
        NSData* segData = [NSData dataWithBytes: &segBytes length: sizeof(segBytes)];
        CFSocketError e = CFSocketSendData(_dataSocket, NULL, (__bridge CFDataRef)(segData), 2);
        
        if (e)
        {
            NSLog(@"send size %ld", e);
        }
        
        e = CFSocketSendData(_dataSocket, NULL, (__bridge CFDataRef)(msgSegData), 2);
        if (e)
        {
            NSLog(@"send %ld", e);
        }
        
        NSString *schemaMsg = [NSString stringWithUTF8String:schemaStr.c_str()];
        NSData* msgSchemaData = [schemaMsg dataUsingEncoding:NSUTF8StringEncoding];
        
        CFDataRef msgSchemaDataRef = (__bridge CFDataRef)(msgSchemaData);
        __uint32_t schemaBytes = CFDataGetLength(msgSchemaDataRef);
        
        NSData* schemaData = [NSData dataWithBytes: &schemaBytes length: sizeof(schemaBytes)];
        e = CFSocketSendData(_dataSocket, NULL, (__bridge CFDataRef)(schemaData), 2);
        
        if (e)
        {
            NSLog(@"send size %ld", e);
        }
        
        
        e = CFSocketSendData(_dataSocket, NULL, (__bridge CFDataRef)(msgSchemaData), 2);
        if (e)
        {
            NSLog(@"send %ld", e);
        }
        
        locationGenerationTimer = [NSTimer scheduledTimerWithTimeInterval: 1.0 target: self selector:@selector(sendNewLocationOnTimer:) userInfo: nil repeats:YES];
    }

}

- (void) sendNewLocationOnTimer:(NSTimer*)timer
{
    if (generatedDataIndex == gpsGeneratedData.size())
        generatedDataIndex -= gpsGeneratedData.size();
    
    LocationData newLocation = gpsGeneratedData[generatedDataIndex];
    
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
    CFDataRef msgDataRef = (__bridge CFDataRef)(msgData);
    __uint32_t size = [msgData length];
    NSLog(@"Size of message is %d", size);
    __uint32_t bytes = size;
    
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
        generatedDataIndex++;
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
    long long timeSinceStartInSecond = (currentTime - _videoStreamStartTime)/MILLISEC_PER_SEC;
    
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
        CFSocketError e = CFSocketSendData(_dataSocket, NULL, (__bridge CFDataRef)(msgData), 2);
        if (e)
        {
            NSLog(@"send %ld", e);
        }
    }
    else if (timeSinceStartInSecond == ((gpsDataVector.back().time - _videoStreamStartTime)/MILLISEC_PER_SEC))
    {
        buffer.push_back (currentLocation);
    }
    else if ((timeSinceStartInSecond > ((gpsDataVector.back ().time - _videoStreamStartTime)/MILLISEC_PER_SEC)))
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
        CFSocketError e = CFSocketSendData(_dataSocket, NULL, (__bridge CFDataRef)(msgData), 2);
        if (e)
        {
            NSLog(@"send %ld", e);
        }
    }
    
}

@end
