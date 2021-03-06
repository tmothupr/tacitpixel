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
#include <tp/version.h>

#import "rendersurface_cocoa.h"


@interface tpGLRenderSurfaceCocoaDelegate : NSResponder
{
	tpGLRenderSurfaceCocoa* rendersurface;
};
- (BOOL)acceptsFirstResponder;
- (BOOL)becomeFirstResponder;
- (BOOL)resignFirstResponder;

-(void) mouseMoved:(NSEvent*)theEvent;

@end

@implementation tpGLRenderSurfaceCocoaDelegate;

-(id)init
{
	self = [super init];
	return self;
}

-(void)setRenderSurface: (tpGLRenderSurfaceCocoa*)arendersurface
{
	[[NSNotificationCenter defaultCenter]
		addObserver:self
		selector:@selector(tryClose:)
		name:NSWindowWillCloseNotification
		object:arendersurface->getWindow()];

	rendersurface = arendersurface;
}

-(void)tryClose:(NSNotification*)notification
{
	[[NSNotificationCenter defaultCenter]
		removeObserver:self
		name:NSWindowWillCloseNotification
		object:rendersurface->getWindow()];

	if (rendersurface) rendersurface->setDone(true);

}

-(void) mouseMoved:(NSEvent*)theEvent
{
	tpLogNotify("Moved!");
}

- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (BOOL)becomeFirstResponder
{
	return YES;
}

- (BOOL)resignFirstResponder
{
	return YES;
}

@end


tpGLRenderSurfaceCocoa::tpGLRenderSurfaceCocoa(tpRenderSurfaceTraits* traits)
	: tpRenderSurface(traits),
	window(NULL),
	oglcontext(NULL),
	delegate(NULL)
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	[NSApplication sharedApplication];

	int width = (traits) ? traits->getSize()[0] : 640;
	int height = (traits) ? traits->getSize()[1] : 480;

	tpLogNotify("%s creating window %dx%d",__FUNCTION__,width,height);

	window = [[NSWindow alloc]
			  initWithContentRect: NSMakeRect(0,0,width,height)
			  styleMask:NSTitledWindowMask+NSClosableWindowMask+NSResizableWindowMask
			  backing:NSBackingStoreBuffered
			  defer:false];

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
	//[window setDelegate:[NSApp delegate]];
	[oglcontext setView:[window contentView]];
	[window setAcceptsMouseMovedEvents:YES];
	[window makeKeyAndOrderFront:nil];
	[window isMainWindow];

	CGLContextObj cgl_context = (CGLContextObj) [oglcontext CGLContextObj];

	CGLSetCurrentContext(cgl_context);

	// @todo implement way to handle this via traits
	tpInt swap_interval = (true) ? 1 : 0;
	CGLSetParameter(cgl_context, kCGLCPSwapInterval, &swap_interval);


	/* set notification interfaces */
	tpGLRenderSurfaceCocoaDelegate* mdelegate = [[tpGLRenderSurfaceCocoaDelegate alloc] init];
	[mdelegate setRenderSurface:this];
	delegate = mdelegate;


	/* title */
	if (traits) this->setCaption(traits->getTitle());

	/* now make application active */
	NSRunningApplication* app = [NSRunningApplication currentApplication];
	[app activateWithOptions: NSApplicationActivateAllWindows];

	[NSApp finishLaunching];

	[pool release];

}


tpGLRenderSurfaceCocoa::~tpGLRenderSurfaceCocoa()
{
	window = NULL;
}

void
tpGLRenderSurfaceCocoa::frame()
{

}

bool
tpGLRenderSurfaceCocoa::makeCurrent()
{
	[oglcontext update];
	[oglcontext makeCurrentContext];

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	NSEvent* event = [NSApp
		nextEventMatchingMask:NSAnyEventMask
		untilDate:nil
		inMode:NSDefaultRunLoopMode
		dequeue:YES];

	if (event)
	{
		[NSApp sendEvent:event];
	}

	[pool release];

	return true;
}

bool
tpGLRenderSurfaceCocoa::swapBuffers()
{
	[oglcontext flushBuffer];
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


void
tpGLRenderSurfaceCocoa::setCaption(const tpString& caption)
{
	NSString* name = [[NSString alloc] initWithCString: caption.c_str() encoding: NSASCIIStringEncoding ];
	[window setTitle:name];
	[name release];
}


//////////////////////////////////////////////////////////////////////////

class tpRenderSurfaceFactoryCocoa : public tpRenderSurfaceFactory {
public:

	TP_TYPE_DECLARE;

	tpRenderSurfaceFactoryCocoa() : tpRenderSurfaceFactory()
	{
		tpLogNotify("%s Cocoa rendering surface",tpGetVersionString());
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


