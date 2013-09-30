//coding: utf-8
#include "ScreenOrganigram.h"

CScreenOrganigram::CScreenOrganigram(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image) :
	IScreen(obj,"Organigramme",image),
	Background(NULL)
{
}

CScreenOrganigram::CScreenOrganigram(const CScreenOrganigram &obj) :
	IScreen(obj),
	Background(NULL)
{
}

CScreenOrganigram::~CScreenOrganigram()
{
}

CScreenOrganigram& CScreenOrganigram::operator=(const CScreenOrganigram &obj)
{
	IScreen::operator=(obj);
	return(*this);
}

void CScreenOrganigram::Init()
{
	const OpenUtility::SFrustum &Frustum=pClass->GetFrustum();

	double ratio=Frustum.pFar/Frustum.pNear;
//	OpenUtility::RGBAd color1(1.0,1.0,1.0,1.0),color2(0.68,0.68,0.68,1.0);
	OpenUtility::RGBAd color2(0.4,0.4,0.4,1.0),color1(0.1,0.1,0.1,1.0);
	Background=new OpenUtility::CQuad(Frustum.pLeft*ratio,Frustum.pBottom*ratio,Frustum.pRight*ratio,Frustum.pTop*ratio,color1,color1,color2,color2);
	OpenUtility::CMat4x4<float> MVPmatrixBg(ProjectionMatrix);
	MVPmatrixBg*=OpenUtility::CMat4x4<float>().SetTranslate(0,0,-Frustum.pFar);
	Background->SetDefaultShaderMatrix(MVPmatrixBg,OpenUtility::CMat4x4<float>());
}

void CScreenOrganigram::UnInit()
{
	delete Background;
}

bool CScreenOrganigram::PreRender()
{
	bool MustRender=false;

	// Update background
	static bool firstInit=true;
	if (firstInit)
	{
		firstInit=false;
		MustRender=true;
	}
	return(MustRender);
}

void CScreenOrganigram::Render()
{
	Background->Draw();
}
