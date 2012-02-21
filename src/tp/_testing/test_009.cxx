
#include <tp/viewer.h>
#include <tp/module.h>
#include <tp/primitive.h>
#include <tp/light.h>



int main(int argc,char* argv[]) {

	tpModuleManager::get()->load("jpg");

	tpRefNode root = new tpNode();
	root->addChild(tpPrimitiveFactory::get()->create(tpPrimitiveFactory::kAxis));

	tpPrimitive* p = tpPrimitiveFactory::get()->create(tpPrimitiveFactory::kPlane);

	tpImage* i = tpImage::read(argv[1]);

	tpTexture* t = new tpTexture();
	t->setImage(i);
	t->setFormat(tpTexture::kFormatRGB);
	p->setTexture(t);

	root->addChild(p);

	tpRefPtr<tpLight> light = new tpLight();
	light->setPosition(tpVec3f(2,2,2));
	light->setAmbientColor(tpVec4f(0.1f,0.1f,0.1f,1.f));


	root->addChild(light.get());

	tpRefPtr<tpViewer> viewer = new tpViewer();

	viewer->getScene().getActiveCamera()->addChild(root.get());
	viewer->getScene().getActiveCamera()->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	viewer->getScene().getActiveCamera()->setClearColor(tpVec4f(0,.15f,.3f,1.f));


	viewer->create();
	
	return viewer->run();;
}
