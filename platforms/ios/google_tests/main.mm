//
//  main.m
//  ffrefe
//
//  Created by Itseez on 13/10/14.
//  Copyright (c) 2014 Itseez, inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#include "gtest.h"
#include <vmf/vmf.hpp>

std::string workingPath;
std::string tempPath;

int main(int argc, char * argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    vmf::Log::setVerbosityLevel(vmf::LOG_NO_MESSAGE);
    
    NSString * filePath = [[NSBundle mainBundle] pathForResource:@"BlueSquare.avi" ofType:nullptr];
    
    workingPath = [filePath UTF8String];
    unsigned found = workingPath.find_last_of("/");
    workingPath = workingPath.substr(0,found);
    tempPath = [NSTemporaryDirectory() UTF8String];
    
    NSLog(@"%s", workingPath.c_str());
    NSLog(@"%s", tempPath.c_str());
    
    return RUN_ALL_TESTS();
}
