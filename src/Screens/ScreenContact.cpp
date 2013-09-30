//coding: utf-8
#include "ScreenContact.h"

CScreenContact::CScreenContact(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image) :
	IScreen(obj,"Personnel",image)
{
}

CScreenContact::CScreenContact(const CScreenContact &obj) :
	IScreen(obj)
{
}

CScreenContact::~CScreenContact()
{
}

CScreenContact& CScreenContact::operator=(const CScreenContact &obj)
{
	IScreen::operator=(obj);
	return(*this);
}

void CScreenContact::Init()
{
}

void CScreenContact::UnInit()
{
}

bool CScreenContact::PreRender()
{
	return(false);
}

void CScreenContact::Render()
{
}
