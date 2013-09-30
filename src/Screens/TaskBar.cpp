//coding: utf-8
#include "TaskBar.h"

CTaskBar::CTaskBar(IRenderingObjectComm *pClass,double height) :
	IRenderingObject(pClass),
	Font30(NULL),
	Font50(NULL),
	Day(NULL),
	Date(NULL),
	HourMin(NULL),
	Seconds(NULL),
	Logo(NULL),
	Background(NULL),
	TaskBarHeight(height)
{
}

CTaskBar::CTaskBar(const CTaskBar &obj) :
	IRenderingObject(obj)
{
//TODO: à compléter
}

CTaskBar::~CTaskBar()
{
//TODO: à compléter
}

CTaskBar& CTaskBar::operator=(const CTaskBar &obj)
{
//TODO: à compléter
	return(*this);
}

const char* CTaskBar::GetDay(int day)
{
	switch(day)
	{
	case 0:return("Dimanche");
	case 1:return("Lundi");
	case 2:return("Mardi");
	case 3:return("Mercredi");
	case 4:return("Jeudi");
	case 5:return("Vendredi");
	case 6:return("Samedi");
	}
	return("");
}

void CTaskBar::SetMatrix(OpenUtility::CMat4x4<float> &p,OpenUtility::CMat4x4<float> &mv,OpenUtility::CMat4x4<float> &n)
{
	IRenderingObject::SetMatrix(p,mv,n);
	MatrixHasChanged=true;
}

void CTaskBar::Init()
{
	Font30=new OpenUtility::CFontLoader("../content/verdana.ttf",30);
	Font50=new OpenUtility::CFontLoader("../content/verdana.ttf",50);
	Day=new OpenUtility::C3DText(Font30,0.0,0.28,0.46,1.0,0.35);
	Date=new OpenUtility::C3DText(Font30,0.0,0.28,0.46,1.0,0.35);
	HourMin=new OpenUtility::C3DText(Font50,0.0,0.28,0.46,1.0);
	Seconds=new OpenUtility::C3DText(Font50,0.0,0.28,0.46,1.0,0.75);
	Logo=new OpenUtility::CTextureQuad("../content/polytech.png",15,1.7);
	Background=new OpenUtility::CQuad(-0.5,-0.5,0.5,0.5);

	Day->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignRight,OpenUtility::CFontLoader::IFontEngine::EVAlignBaseligne);
	Date->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignRight,OpenUtility::CFontLoader::IFontEngine::EVAlignBaseligne);
	HourMin->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignRight,OpenUtility::CFontLoader::IFontEngine::EVAlignBaseligne);
	Seconds->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignRight,OpenUtility::CFontLoader::IFontEngine::EVAlignBaseligne);
	MatrixHasChanged=true;
	TaskBarAlpha=1;
}

void CTaskBar::UnInit()
{
	delete Background;
	delete Logo;
	delete Day;
	delete Date;
	delete HourMin;
	delete Seconds;
	delete Font30;
	delete Font50;
}

bool CTaskBar::PreRender()
{
	struct tm timeTM;
	bool hasChanged;
	static struct tm lastTM={0,0,0,0,0,0,0,0,0};
	OpenUtility::CStream strTime;
	time_t timeStamp;
	static bool isFirstRendering=true;
	unsigned long long timeUnit=pClass->GetTimeUnit();

	if (isFirstRendering)
	{
		isFirstRendering=false;
		Anims.ShowRotateTaskBar=new SAnim(timeUnit,1000);
	}

	// Time update
	timeStamp=time(NULL);
	localtime_r(&timeStamp,&timeTM);
	hasChanged=false;
	if (lastTM.tm_year==0 || timeTM.tm_sec!=lastTM.tm_sec)
	{
		strTime.Format("%02d",timeTM.tm_sec);
		Seconds->UpdateText(strTime.GetStream());
		hasChanged=true;
	}
	if (lastTM.tm_year==0 || timeTM.tm_hour!=lastTM.tm_hour || timeTM.tm_min!=lastTM.tm_min)
	{
		strTime.Format("%02d:%02d",timeTM.tm_hour,timeTM.tm_min);
		HourMin->UpdateText(strTime.GetStream());
		hasChanged=true;
	}
	if (lastTM.tm_year==0 || timeTM.tm_hour!=lastTM.tm_hour || timeTM.tm_min!=lastTM.tm_min)
	{
		strTime.Format("%02d/%02d/%04d",timeTM.tm_mday,timeTM.tm_mon+1,timeTM.tm_year+1900);
		Date->UpdateText(strTime.GetStream());
		Day->UpdateText(GetDay(timeTM.tm_wday));
		hasChanged=true;
	}
	if (hasChanged) lastTM=timeTM;

	// Other update
	if (MatrixHasChanged || Anims.HasAnim())
	{
		const OpenUtility::SFrustum &frustum=pClass->GetFrustum();
		double dist=OpenUtility::v3dModule(pClass->GetCameraPos().GetPos());
		double l,r,b;
		OpenUtility::CMat4x4<float> MVPmatrixBackground;

		r=dist*frustum.pRight/frustum.pNear;
		l=dist*frustum.pLeft/frustum.pNear;
		b=dist*frustum.pBottom/frustum.pNear;

		MVPmatrixSeconds=ProjectionMatrix*ModelViewMatrix;
		if (Anims.HideTaskBar || Anims.ShowTaskBar)
		{
			double translationFactor;
			int dir;
			SAnim **anim;

			if (Anims.HideTaskBar)
			{
				anim=&Anims.HideTaskBar;
				dir=0;
			}
			else
			{
				anim=&Anims.ShowTaskBar;
				dir=1;
			}

			if ((*anim)->IsEndAnim(timeUnit))
			{
				TaskBarAlpha=dir;
				translationFactor=1-dir;
//if (Anims.HideTaskBar) Anims.ShowTaskBar=new SAnim(timeUnit,500);
				delete *anim;
				*anim=NULL;
			}
			else
			{
				const double factor=0.8;
				if (Anims.HideTaskBar) TaskBarAlpha=std::max(0.0,((*anim)->Duration*factor+(*anim)->StartTime-timeUnit)/((*anim)->Duration*factor));
				else TaskBarAlpha=(*anim)->GetTimeRatio(timeUnit);
				translationFactor=dir+(1-dir*2)*(*anim)->GetTimeRatio(timeUnit);
			}
			OpenUtility::CMat4x4<float> translationMatrix;
			translationMatrix.SetTranslate(0,-TaskBarHeight*translationFactor,0);
			MVPmatrixSeconds*=translationMatrix;
			OpenUtility::RGBAd color(0.0,0.28,0.46,TaskBarAlpha);
			Day->SetDefaultShaderColor(color);
			Date->SetDefaultShaderColor(color);
			HourMin->SetDefaultShaderColor(color);
			Seconds->SetDefaultShaderColor(color);
		}
		MVPmatrixLogo=MVPmatrixBackground=MVPmatrixSeconds;

		MVPmatrixSeconds*=OpenUtility::CMat4x4<float>().SetTranslate(r-0.5,b,0);
		MVPmatrixLogo*=OpenUtility::CMat4x4<float>().SetTranslate(0,b,0);
		Background->SetAlpha(TaskBarAlpha);
		Background->UpdateCoord(l,0,r,TaskBarHeight,OpenUtility::RGBAd(1.0,1.0,1.0,1.0),OpenUtility::RGBAd(1.0,1.0,1.0,1.0),OpenUtility::RGBAd(0.7,0.7,0.7,1.0),OpenUtility::RGBAd(0.7,0.7,0.7,1.0));
		MVPmatrixBackground*=OpenUtility::CMat4x4<float>().SetTranslate(0,b,0);

		if (Anims.ShowRotateTaskBar)
		{
			if (Anims.ShowRotateTaskBar->IsEndAnim(timeUnit))
			{
				delete Anims.ShowRotateTaskBar;
				Anims.ShowRotateTaskBar=NULL;
			}
			else
			{
				OpenUtility::CMat4x4<float> rotationMatrix;
				rotationMatrix.SetRotate(-120+120*Anims.ShowRotateTaskBar->GetTimeRatio(timeUnit),1,0,0);
				MVPmatrixSeconds*=rotationMatrix;
				MVPmatrixLogo*=rotationMatrix;
				MVPmatrixBackground*=rotationMatrix;
			}
		}

//TODO: Normal matrix is incorect!!!
		Background->SetDefaultShaderMatrix(MVPmatrixBackground,ModelViewMatrix);

		double timeDecal=(TaskBarHeight-1.0)/2.0+0.1;
		MVPmatrixDay=MVPmatrixDate=MVPmatrixHourMin=MVPmatrixSeconds;
		MVPmatrixDay*=OpenUtility::CMat4x4<float>().SetTranslate(-4.3,0.4+timeDecal,0);
		MVPmatrixDate*=OpenUtility::CMat4x4<float>().SetTranslate(-4.3,0.05+timeDecal,0);
		MVPmatrixHourMin*=OpenUtility::CMat4x4<float>().SetTranslate(-1.1,0+timeDecal,0);
		MVPmatrixSeconds*=OpenUtility::CMat4x4<float>().SetTranslate(0,0.18+timeDecal,0);

		Day->SetDefaultShaderMatrix(MVPmatrixDay);
		Date->SetDefaultShaderMatrix(MVPmatrixDate);
		HourMin->SetDefaultShaderMatrix(MVPmatrixHourMin);
		Seconds->SetDefaultShaderMatrix(MVPmatrixSeconds);

		MVPmatrixLogo*=OpenUtility::CMat4x4<float>().SetTranslate(l+Logo->GetW()/2+0.7,TaskBarHeight/2,0);

		MatrixHasChanged=false;
		hasChanged=true;
	}

	return(hasChanged);
}

void CTaskBar::Render()
{
	try
	{
		glDisable(GL_DEPTH_TEST);

		Background->Draw();

		pClass->GetGlobalShader().UseProgram();
		glUniform4f(pClass->GetGlobalShader()["u_GlobalColor"],1.0,1.0,1.0,TaskBarAlpha);
		glUniformMatrix4fv(pClass->GetGlobalShader()["u_MVPmatrix"],1,GL_FALSE,MVPmatrixLogo.GetMatrix());
		Logo->AttachAttribToData(pClass->GetGlobalShader()["vPos"],pClass->GetGlobalShader()["vNorm"],pClass->GetGlobalShader()["vTexCoord"]);
		Logo->Draw();

		Day->Draw();
		Date->Draw();
		HourMin->Draw();
		Seconds->Draw();

		glEnable(GL_DEPTH_TEST);
	}
	catch(OpenUtility::CShaderProgram::Exception &e)
	{
		std::cout << e << std::endl;
	}
}
