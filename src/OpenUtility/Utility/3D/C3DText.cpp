#include "C3DText.h"

OpenUtility::C3DText::C3DText(const OpenUtility::CFontLoader *loader) :
	CFontLoader::CFontEngine(loader)
{
}

OpenUtility::C3DText::C3DText(const C3DText &obj) :
	CFontLoader::CFontEngine(obj)
{
}

OpenUtility::C3DText::~C3DText()
{
}

OpenUtility::C3DText& OpenUtility::C3DText::operator=(const C3DText &obj)
{
	return(*this);
}

void OpenUtility::C3DText::SetText(const char *text)
{
}

void OpenUtility::C3DText::SetAlignement(EHAlign hAlign,EVAlign vAlign)
{
}

void OpenUtility::C3DText::DrawText()
{
}
