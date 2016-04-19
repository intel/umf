//
//  VmfClientConnection.h
//  Encoder Demo
//
//  Created by Itseez on 05/04/16.
//  Copyright © 2016 Geraint Davies. All rights reserved.
//

#import <CoreLocation/CoreLocation.h>
#import "RTSPServer.h"

@interface VmfClientConnection : NSObject <CLLocationManagerDelegate>
{
    CFSocketRef _dataSocket;
    
    CLLocationManager* _locationManager;
    
    NSTimer* _locationGenerationTimer;
    
    CFRunLoopSourceRef _rls;
    bool _isShutdown;
    
    long long _videoStreamStartTime;
    long long _mdStreamStartTime;
    
    bool _isEmulatedGPS;
    bool _useCompression;
}

@property (readwrite) CFSocketRef dataSocket;
@property (readonly) bool isShutdown;
@property (readwrite) long long videoStreamStartTime;
@property (readwrite) long long mdStreamStartTime;
@property (readwrite) bool isEmulatedGPS;
@property (readwrite) bool useCompression;

+ (VmfClientConnection*) createWithSocket:(CFSocketNativeHandle) s server:(RTSPServer*) server;
- (void) onSocketData:(CFDataRef)data;
- (void) sendMessage:(NSData*) msgData;
- (void) toggleEmulatedGPS:(bool) enable;
- (void) toggleUseCompression:(bool) enable;

@end
