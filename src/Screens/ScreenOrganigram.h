//coding: utf-8
#ifndef _ScreenOrganigram_h
	#define _ScreenOrganigram_h

#include "Screen.h"

class CScreenOrganigram : public IScreen
{
public:
	CScreenOrganigram(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image=NULL);
	CScreenOrganigram(const CScreenOrganigram &obj);
	virtual ~CScreenOrganigram();
	CScreenOrganigram& operator=(const CScreenOrganigram &obj);
	void Init();
	void UnInit();
	bool PreRender();
	void Render();

private:
	OpenUtility::CQuad *Background;
};

#endif
