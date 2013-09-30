//coding: utf-8
#ifndef _ScreenSchedule_h
	#define _ScreenSchedule_h

#include "Screen.h"

class CScreenSchedule : public IScreen
{
public:
	CScreenSchedule(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image=NULL);
	CScreenSchedule(const CScreenSchedule &obj);
	virtual ~CScreenSchedule();
	CScreenSchedule& operator=(const CScreenSchedule &obj);
	void Init();
	void UnInit();
	bool PreRender();
	void Render();
};

#endif
