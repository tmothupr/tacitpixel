#include <tp/rendercontext.h>
#include <tp/stringtokenizer.h>

tpRenderContext::tpRenderContext()
    : tpReferenced()
{
}

tpRenderContext::~tpRenderContext()
{
}

tpString
tpRenderContext::getString(const tpUInt &e)
{
    return tpString();
}

void
tpRenderContext::getExtensions(tpStringArray& extensions) const
{
    tpString ext = this->getExtensions();
    extensions = tpStringTokenizer::split(ext," ");
}

TP_TYPE_REGISTER(tpRenderContext,tpReferenced,RenderContext);