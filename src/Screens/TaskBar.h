//coding: utf-8
#ifndef _TaskBar_h
	#define _TaskBar_h

#include <Utility/3D/C3DText.h>
#include <Utility/3D/CTextureQuad.h>
#include <Utility/3D/CQuad.h>
#include "../RenderingObject.h"

class CTaskBar : public IRenderingObject
{
private:
	struct SAnim
	{
		inline SAnim(const unsigned long long start,const unsigned long long duration) : StartTime(start),Duration(duration) {}
		inline bool IsEndAnim(unsigned long long time) {return(time>StartTime+Duration);}
		inline double GetTimeRatio(unsigned long long time) {return(1-(StartTime+Duration-time)/double(Duration));}
		unsigned long long StartTime;
		unsigned long long Duration;
	};

	struct SAnims
	{
		SAnims() : HideTaskBar(NULL),ShowTaskBar(NULL),ShowRotateTaskBar(NULL) {}
		~SAnims() {delete HideTaskBar;delete ShowTaskBar;delete ShowRotateTaskBar;}
		inline bool HasAnim() {return(HideTaskBar!=NULL || ShowTaskBar!=NULL || ShowRotateTaskBar!=NULL);}
		SAnim *HideTaskBar;
		SAnim *ShowTaskBar;
		SAnim *ShowRotateTaskBar;
	};

public:
	CTaskBar(IRenderingObjectComm *pClass,double height);
	CTaskBar(const CTaskBar &obj);
	virtual ~CTaskBar();
	CTaskBar& operator=(const CTaskBar &obj);
	void Init();
	void UnInit();
	bool PreRender();
	void Render();
	void SetMatrix(OpenUtility::CMat4x4<float> &p,OpenUtility::CMat4x4<float> &mv,OpenUtility::CMat4x4<float> &n);
	bool UseGlobalShaderProgram() {return(false);}
	inline void Hide() {Anims.HideTaskBar=new SAnim(pClass->GetTimeUnit(),500);}

private:
	const char* GetDay(int day);

private:
	bool MatrixHasChanged;
	OpenUtility::CFontLoader *Font30,*Font50;
	OpenUtility::C3DText *Day,*Date,*HourMin,*Seconds;
	OpenUtility::CTextureQuad *Logo;
	OpenUtility::CQuad *Background;
    double TaskBarHeight;
	float TaskBarAlpha;
	OpenUtility::CMat4x4<float> MVPmatrixDay,MVPmatrixDate,MVPmatrixHourMin,MVPmatrixSeconds,MVPmatrixLogo;
	// Annimations
	SAnims Anims;
};

#endif
