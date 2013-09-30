//coding: utf-8
#include "ScreenSchedule.h"

CScreenSchedule::CScreenSchedule(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image) :
	IScreen(obj,"Emploi du temps",image)
{
}

CScreenSchedule::CScreenSchedule(const CScreenSchedule &obj) :
	IScreen(obj)
{
}

CScreenSchedule::~CScreenSchedule()
{
}

CScreenSchedule& CScreenSchedule::operator=(const CScreenSchedule &obj)
{
	IScreen::operator=(obj);
	return(*this);
}

void CScreenSchedule::Init()
{
}

void CScreenSchedule::UnInit()
{
}

bool CScreenSchedule::PreRender()
{
	return(false);
}

void CScreenSchedule::Render()
{
}
