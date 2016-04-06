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
    size_t generatedDataIndex;
    
    CFRunLoopSourceRef _rls;
    bool _isShutdown;
    
    long long _videoStreamStartTime;
}

@property (readwrite) CFSocketRef dataSocket;
@property (readonly) bool isShutdown;
@property (readwrite) long long videoStreamStartTime;

+ (VmfClientConnection*) createWithSocket:(CFSocketNativeHandle) s server:(RTSPServer*) server;
- (void) onSocketData:(CFDataRef)data;

@end
