//
//  urMusAppDelegate.m
//  urMus
//
//  Created by Georg Essl on 6/20/09.
//  Copyright Georg Essl 2009. All rights reserved. See LICENSE.txt for license conditions.
//

#import "urMusAppDelegate.h"
#import "EAGLView.h"
#include "urAPI.h"
#include "RIOAudioUnitLayer.h"
#include "lfs.h"
#include "httpServer.h"

// This enables video projector output. It's not official API hence not safe for app store.
//#define PROJECTOR_VIDEO
//#define NEW_PROJECTOR_VIDEO
#define FINAL_PROJECTOR_VIDEO

#ifdef NEW_PROJECTOR_VIDEO
#import "UIApplication+ScreenMirroring.h"
#endif

#ifdef SANDWICH_SUPPORT
#import "SandwichUpdateListener.h"
#endif

@implementation urMusAppDelegate

@synthesize window;
@synthesize glView;

#ifdef FINAL_PROJECTOR_VIDEO
//@synthesize deviceWindow;
@synthesize externalWindow;
@synthesize glView2;
#endif


// Make EAGLview global so lua interface can grab it without breaking a leg over IMP
EAGLView* g_glView;

#define EARLY_LAUNCH
extern NSString* errorstr;
extern bool newerror;

//------------------------------------------------------------------------------
// Application controls
//------------------------------------------------------------------------------

#ifdef FINAL_PROJECTOR_VIDEO
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	UIScreenMode *desiredMode = [screenModes objectAtIndex:buttonIndex];
//	[self log:[NSString stringWithFormat:@"Setting mode: %@", desiredMode]];
	externalScreen.currentMode = desiredMode;
	
//	[self log:@"Assigning externalWindow to externalScreen."];
	externalWindow = [[UIWindow alloc] init];
//	[externalWindow addSubview:g_glView];
	externalWindow.screen = externalScreen;
	
	[screenModes release];
	[externalScreen release];
	
	CGRect rect = CGRectZero;
	rect.size = desiredMode.size;
	externalWindow.frame = rect;
	externalWindow.clipsToBounds = YES;
	
//	[self log:@"Displaying externalWindow on externalScreen."];
	externalWindow.hidden = NO;
	[externalWindow makeKeyAndVisible];
}
#endif

- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
	g_glView = glView;
	/* Declare a Lua State, open the Lua State and load the libraries (see above). */
	lua = lua_open();
	luaL_openlibs(lua);
	luaopen_lfs (lua); // Added external luafilesystem, runs under lua's open license
	l_setupAPI(lua);

//	[[UIApplication] sharedApplication] startTVOut]; // This enables that the video data is send to the AV out for projection (it's a mirror)
#ifdef PROJECTOR_VIDEO
	[application startTVOut]; // This enables that the video data is send to the AV out for projection (it's a mirror)
#endif

#ifdef NEW_PROJECTOR_VIDEO
//	[[UIApplication sharedApplication] setupScreenMirroringOfMainWindow:mainWindow framesPerSecond:20];
//	[application setupScreenMirroringOfMainWindow:window framesPerSecond:20];
	[application setupScreenMirroring];
#endif
	
#ifdef FINAL_PROJECTOR_VIDEO
	// Check for external screen.
	if ([[UIScreen screens] count] > 1) {
//		[self log:@"Found an external screen."];
		
		// Internal display is 0, external is 1.
		externalScreen = [[[UIScreen screens] objectAtIndex:1] retain];
//		[self log:[NSString stringWithFormat:@"External screen: %@", externalScreen]];
		
		screenModes = [externalScreen.availableModes retain];
//		[self log:[NSString stringWithFormat:@"Available modes: %@", screenModes]];
		
		// Allow user to choose from available screen-modes (pixel-sizes).
		UIAlertView *alert = [[[UIAlertView alloc] initWithTitle:@"External Display Size" 
														 message:@"Choose a size for the external display." 
														delegate:self 
											   cancelButtonTitle:nil 
											   otherButtonTitles:nil] autorelease];
		for (UIScreenMode *mode in screenModes) {
			CGSize modeScreenSize = mode.size;
			[alert addButtonWithTitle:[NSString stringWithFormat:@"%.0f x %.0f pixels", modeScreenSize.width, modeScreenSize.height]];
		}
		[alert show];
		
	} else {
//		[self log:@"External screen not found."];
	}
#endif
	
#ifdef SANDWICH_SUPPORT
	// init SandwichUpdateListener
//	NSLog(@"Delegate: Starting Listener...");
	[SandwichUpdateListener initializeWithServerPort:4555 andDelegate:glView];
	[SandwichUpdateListener addDelegate: self];
	
	if([SandwichUpdateListener startListening])
	{
//		NSLog(@"Success!");  
	}
	else
	{
//		NSLog(@"Fail :(");  
	};
#endif
	
	[glView startAnimation];

#ifdef EARLY_LAUNCH
	NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
	NSString *filePath = [resourcePath stringByAppendingPathComponent:@"urMus.lua"];
	NSArray *paths;
	paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentPath;
	if ([paths count] > 0)
		documentPath = [paths objectAtIndex:0];
		
	// start off http server
#define HTTP_EDITING
#ifdef HTTP_EDITING
	http_start([resourcePath UTF8String],
			   [documentPath UTF8String]);
#endif
	
	const char* filestr = [filePath UTF8String];

	if(luaL_dofile(lua, filestr)!=0)
	{
		const char* error = lua_tostring(lua, -1);
		errorstr = [[NSString alloc] initWithCString:error ]; // DPrinting errors for now
		newerror = true;
	}
#endif	
}

#ifdef SANDWICH_SUPPORT
- (void) rearTouchUpdate: (SandwichEventManager * ) sender;
{
	
}
- (void) pressureUpdate: (SandwichEventManager * ) sender;
{
	NSLog(@"Appdelegate - PressureUpate");
	int pressure[4];
	
	pressure[0] = sender.pressureValues[0];
	pressure[1] = sender.pressureValues[1];
	pressure[2] = sender.pressureValues[2];
	pressure[3] = sender.pressureValues[3];
}
#endif

- (void)applicationWillResignActive:(UIApplication *)application {
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
}


- (void)dealloc {
	/* Remember to destroy the Lua State */
	http_stop();
	lua_close(lua);
	
	[window release];
	[glView release];
	[super dealloc];
}

@end
