//coding: utf-8
#ifndef _ScreenTransport_h
	#define _ScreenTransport_h

#include "Screen.h"

class CScreenTransport : public IScreen
{
public:
	CScreenTransport(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image=NULL);
	CScreenTransport(const CScreenTransport &obj);
	virtual ~CScreenTransport();
	CScreenTransport& operator=(const CScreenTransport &obj);
	void Init();
	void UnInit();
	bool PreRender();
	void Render();
};

#endif
