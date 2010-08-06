//
//  urMusAppDelegate.h
//  urMus
//
//  Created by Georg Essl on 6/20/09.
//  Copyright Georg Essl 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "urAPI.h"

#ifdef SANDWICH_SUPPORT
#import "SandwichTypes.h"
#endif

@class EAGLView;

#ifdef SANDWICH_SUPPORT
@interface urMusAppDelegate : NSObject <UIApplicationDelegate,SandwichUpdateDelegate> {
#else
@interface urMusAppDelegate : NSObject <UIApplicationDelegate> {
#endif
    UIWindow *window;
    EAGLView *glView;

	UIWindow *externalWindow;
	EAGLView *glView2;
	NSArray *screenModes;
	UIScreen *externalScreen;
}

//@property (nonatomic, retain) IBOutlet UIWindow *deviceWindow;
@property (nonatomic, retain) IBOutlet UIWindow *externalWindow;
@property (nonatomic, retain) IBOutlet EAGLView *glView2;	
@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet EAGLView *glView;

#ifdef SANDWICH_SUPPORT
- (void) rearTouchUpdate: (SandwichEventManager * ) sender;
- (void) pressureUpdate: (SandwichEventManager * ) sender;
#endif
	
@end

