//coding: utf-8
#include "ScreenTransport.h"

CScreenTransport::CScreenTransport(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image) :
	IScreen(obj,"Transports",image)
{
}

CScreenTransport::CScreenTransport(const CScreenTransport &obj) :
	IScreen(obj)
{
}

CScreenTransport::~CScreenTransport()
{
}

CScreenTransport& CScreenTransport::operator=(const CScreenTransport &obj)
{
	IScreen::operator=(obj);
	return(*this);
}

void CScreenTransport::Init()
{
}

void CScreenTransport::UnInit()
{
}

bool CScreenTransport::PreRender()
{
	return(false);
}

void CScreenTransport::Render()
{
}
