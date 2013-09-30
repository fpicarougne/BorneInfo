//coding: utf-8
#include "ScreenEvent.h"

CScreenEvent::CScreenEvent(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image) :
	IScreen(obj,"Ev�nements",image)
{
}

CScreenEvent::CScreenEvent(const CScreenEvent &obj) :
	IScreen(obj)
{
}

CScreenEvent::~CScreenEvent()
{
}

CScreenEvent& CScreenEvent::operator=(const CScreenEvent &obj)
{
	IScreen::operator=(obj);
	return(*this);
}

void CScreenEvent::Init()
{
}

void CScreenEvent::UnInit()
{
}

bool CScreenEvent::PreRender()
{
	return(false);
}

void CScreenEvent::Render()
{
}
