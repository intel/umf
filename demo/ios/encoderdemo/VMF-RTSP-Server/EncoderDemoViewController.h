//
//  EncoderDemoViewController.h
//  Encoder Demo
//
//  Created by Geraint Davies on 11/01/2013.
//  Copyright (c) 2013 GDCL http://www.gdcl.co.uk/license.htm
//

#import <UIKit/UIKit.h>
#import "CameraServer.h"

@interface EncoderDemoViewController : UIViewController <CameraServerDelegate>
@property (strong, nonatomic) IBOutlet UIView *cameraView;
@property (strong, nonatomic) IBOutlet UILabel *serverAddress;
@property (retain, nonatomic) IBOutlet UIButton *settingsButton;

- (void) startPreview;
- (IBAction)clickButton:(id)sender;

@end
