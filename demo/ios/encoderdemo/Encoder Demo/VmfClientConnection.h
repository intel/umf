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
    
    NSTimer *locationGenerationTimer;
    
    CFRunLoopSourceRef _rls;
    bool _isShutdown;
    
    long long _videoStreamStartTime;
    long long _mdStreamStartTime;
}

@property (readwrite) CFSocketRef dataSocket;
@property (readonly) bool isShutdown;
@property (readwrite) long long videoStreamStartTime;
@property (readwrite) long long mdStreamStartTime;

+ (VmfClientConnection*) createWithSocket:(CFSocketNativeHandle) s server:(RTSPServer*) server;
- (void) onSocketData:(CFDataRef)data;
- (void) sendMessage:(NSData*) msgData;
@end
