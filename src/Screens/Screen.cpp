//coding: utf-8
#include "Screen.h"

CScreen::CScreen(const char *name,OpenUtility::ITextureQuad *image) :
	Name(name),
	Image(image)
{
}

CScreen::CScreen(const CScreen &obj) :
	Name(obj.Name),
	Image(obj.Image)
{
}

CScreen::~CScreen()
{
}

CScreen& CScreen::operator=(const CScreen &obj)
{
	Name=obj.Name;
	Image=obj.Image;
	return(*this);
}

void CScreen::DrawScreen()
{
}
