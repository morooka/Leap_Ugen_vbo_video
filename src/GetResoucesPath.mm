#include "GetResoucesPath.h"
#import <Cocoa/Cocoa.h>
std::string getResourcesPath()
{
	std::string result;
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	result = [[[NSBundle mainBundle] bundlePath] UTF8String];
	[pool release];
	return result;
}