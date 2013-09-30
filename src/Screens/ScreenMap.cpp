//coding: utf-8
#include "ScreenMap.h"

CScreenMap::CScreenMap(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image) :
	IScreen(obj,"Plans des b�timents",image)
{
}

CScreenMap::CScreenMap(const CScreenMap &obj) :
	IScreen(obj)
{
}

CScreenMap::~CScreenMap()
{
}

CScreenMap& CScreenMap::operator=(const CScreenMap &obj)
{
	IScreen::operator=(obj);
	return(*this);
}

void CScreenMap::Init()
{
}

void CScreenMap::UnInit()
{
}

bool CScreenMap::PreRender()
{
	return(false);
}

void CScreenMap::Render()
{
}
