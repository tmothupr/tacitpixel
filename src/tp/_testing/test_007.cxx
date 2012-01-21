
#include <tp/rendersurface.h>
#include <tp/log.h>
#include <tp/thread.h>
#include <tp/renderer.h>

int main(int argc, char* argv[]) {

	tpRefPtr<tpRenderSurface> window = tpRenderSurface::create();

    window->setContext(0);

    tpLogNotify("%s",window->getContext()->getVendor().c_str());
    tpLogNotify("%s",window->getContext()->getRenderer().c_str());
    tpLogNotify("%s",window->getContext()->getVersion().c_str());

	window->setCaption("Tacit Pixel 3");

	window->show(true);

    tpRefPtr<tpRenderer> renderer = tpRenderer::create();

	while (window->isValid()) {

        window->getContext()->wait(tpRenderContext::kWaitGL);
		window->update();

        tpThread::yield();
	}

	tpLogNotify("Done");

	return 0;
}