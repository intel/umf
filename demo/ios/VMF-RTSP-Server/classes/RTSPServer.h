//
//  RTSPServer.h
//  Encoder Demo
//
//  Created by Geraint Davies on 17/01/2013.
//  Copyright (c) 2013 GDCL http://www.gdcl.co.uk/license.htm
//

#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h> 
//#import "UmfClientConnection.h"
#include <sys/socket.h> 
#include <netinet/in.h>

@class UmfClientConnection;

@interface RTSPServer : NSObject

+ (NSString*) getIPAddress;
+ (RTSPServer*) setupListener:(NSData*) configData;

- (NSData*) getConfigData;
- (void) onUmfAccept:(CFSocketNativeHandle) childHandle;
- (void) onVideoData:(NSArray*) data time:(double) pts;
- (void) shutdownConnection:(id) conn;
- (void) shutdownServer;
- (void) toggleEmulatedGPS:(bool) enable;
- (void) toggleUseCompression:(bool) enable;

@property (readwrite, atomic) int bitrate;
@property (readwrite) UmfClientConnection* umf;
@property (readwrite) long long startVideoStreamTime;
@property (readonly) bool isEmulatedGPS;
@property (readonly) bool useCompression;
@end
