#ifndef TPRENDERCONTEXTGLX_H
#define TPRENDERCONTEXTGLX_H

#include <tp/config.h>

#if defined(TP_USE_X11)

#include <tp/rendercontext.h>

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>


class tpRenderContextGLX : public tpRenderContext {

	GLXContext glxcontext;

	Display* display;
	Window window;

public:

	TP_TYPE_DECLARE

    tpRenderContextGLX();

    bool bind(tpRenderTarget *target);

    void wait(tpUInt e);
	bool makeCurrent();
	bool swapBuffers();

	void destroy();

    tpString getString(const tpUInt& e);

    void *getProcAddress(const char *name);


    tpUInt getRendererTraits() const;


protected:

    virtual ~tpRenderContextGLX();
};

#endif

#endif
