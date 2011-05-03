/*
 * Twisted Pair Visualization Engine
 *
 * Copyright (c) 1999-2009 Hartmut Seichter 
 * 
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the Twisted Pair License (TPL) version 1.0 or (at your option) 
 * any later version. The full license text is available in the LICENSE file 
 * included with this distribution, and on the technotecture.com website.
 *
 */

#include <tp/module.h>

#import "tpGLRenderSurfaceCocoa.h"

@interface tpGLRenderSurfaceCocoaDelegate : NSObject
{
	tpRenderSurface* rendersurface;
	
	
};
@end

@implementation tpGLRenderSurfaceCocoaDelegate;

-(id)init
{
	self = [super init];
	return self;
}

-(void)setRenderSurface: (tpGLRenderSurfaceCocoa*)arendersurface
{
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(tryClose:) name:NSWindowWillCloseNotification
											   object:arendersurface->getWindow()];	
	
	rendersurface = arendersurface;
}

-(void)tryClose:(NSNotification*)notification
{
	if (rendersurface) rendersurface->setDone(true);
}

@end

struct tpAutoReleasePool {
	NSAutoreleasePool* pool;
	tpAutoReleasePool() { pool = [[NSAutoreleasePool alloc] init]; }
	~tpAutoReleasePool() { [pool drain]; }
};

tpGLRenderSurfaceCocoa::tpGLRenderSurfaceCocoa(tpRenderSurfaceTraits* traits) 
: tpRenderSurface(traits)
{
	tpAutoReleasePool apool;
	
	[NSApplication sharedApplication];
	
	int width = (traits) ? traits->getSize()[0] : 640;
	int height = (traits) ? traits->getSize()[1] : 480;
	
	tpLogNotify("%s creating window %dx%d",__FUNCTION__,width,height);
	
	window = [[NSWindow alloc]
			  initWithContentRect: NSMakeRect(0,0,width,height)
			  styleMask:NSTitledWindowMask+NSClosableWindowMask+NSResizableWindowMask 
			  backing:NSBackingStoreBuffered 
			  defer:FALSE];
	
	NSOpenGLPixelFormatAttribute window_attribs[] =
	{
		NSOpenGLPFANoRecovery,
		NSOpenGLPFAAccelerated,
		NSOpenGLPFADepthSize,     (NSOpenGLPixelFormatAttribute)32,
		NSOpenGLPFAColorSize,     (NSOpenGLPixelFormatAttribute)32,
		NSOpenGLPFAAlphaSize,     (NSOpenGLPixelFormatAttribute)8,
		NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)1,
		NSOpenGLPFASamples,       (NSOpenGLPixelFormatAttribute)4, // anti alias
		NSOpenGLPFAStencilSize,   (NSOpenGLPixelFormatAttribute)0, // stencil buffer
		NSOpenGLPFADoubleBuffer,
		(NSOpenGLPixelFormatAttribute)nil
	};
	
	NSOpenGLPixelFormat *format(nil);
	
	format = [[NSOpenGLPixelFormat alloc] initWithAttributes:window_attribs];
	oglcontext = [[NSOpenGLContext alloc] initWithFormat:format shareContext:NULL];
	
	
	[format release];

	[window center];
	[window setDelegate:[NSApp delegate]];
	[oglcontext setView:[window contentView]];
	[window setAcceptsMouseMovedEvents:TRUE];	
	[window makeKeyAndOrderFront:nil];
	
	CGLContextObj cgl_context = (CGLContextObj) [oglcontext CGLContextObj];
	
	CGLSetCurrentContext(cgl_context);
	tpInt swap_interval = (true) ? 1 : 0;
	CGLSetParameter(cgl_context, kCGLCPSwapInterval, &swap_interval);
	
	
	/* set notification interfaces */
	tpGLRenderSurfaceCocoaDelegate* mdelegate = [[tpGLRenderSurfaceCocoaDelegate alloc] init];
	[mdelegate setRenderSurface:this];
	delegate = mdelegate;

	/* title */
	if (traits) this->setCaption(traits->getTitle());
	
	[NSApp finishLaunching];
	
}

bool tpGLRenderSurfaceCocoa::makeCurrent() 
{
	[oglcontext makeCurrentContext];
	
	NSEvent* event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
	
	
	if (event)
	{
		[NSApp sendEvent:event];
	}
	
	[event release];

	return true;
}

bool tpGLRenderSurfaceCocoa::swapBuffers() 
{	
	[oglcontext flushBuffer];
	[oglcontext update];
	
	return true;
}

tpString tpGLRenderSurfaceCocoa::getString(tpUInt glenum) 
{
	makeCurrent();
	tpString result;
	//result.set(reinterpret_cast<const tpChar*>(tpGL::GetString(glenum)));
	return result;
}

bool tpGLRenderSurfaceCocoa::show(bool doShow) 
{
	[window setIsVisible:(BOOL)doShow];

	return [window isVisible];
}


tpVoid tpGLRenderSurfaceCocoa::setCaption(const tpString& caption)
{
	NSString* name = [[NSString alloc] initWithCString: caption.c_str() encoding: NSASCIIStringEncoding ];
	[window setTitle:name];
	[name release];
}

tpGLRenderSurfaceCocoa::~tpGLRenderSurfaceCocoa() 
{
	tpAutoReleasePool apool;
	[oglcontext release];
	[window release];
}


//////////////////////////////////////////////////////////////////////////

class tpRenderSurfaceFactoryCocoa : public tpRenderSurfaceFactory {
public:
	
	TP_TYPE_DECLARE;
	
	tpRenderSurfaceFactoryCocoa() : tpRenderSurfaceFactory()
	{
		tpLogNotify("%s - added Cocoa Surface Factory");
	}
protected:	
	tpRenderSurface* create( tpRenderSurfaceTraits* traits )
	{
		return new tpGLRenderSurfaceCocoa( traits );
	}
};

//////////////////////////////////////////////////////////////////////////


TP_TYPE_REGISTER(tpRenderSurfaceFactoryCocoa,tpRenderSurfaceFactory,RenderSurfaceFactoryCocoa);
TP_TYPE_REGISTER(tpGLRenderSurfaceCocoa,tpRenderSurface,GLRenderSurfaceCocoa);

TP_MODULE_REGISTER(cocoasurface,tpRenderSurfaceFactoryCocoa);


