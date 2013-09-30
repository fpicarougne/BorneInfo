//coding: utf-8
#include "Screen.h"

IScreen::IScreen(IRenderingObjectComm *obj,const char *name,OpenUtility::ITextureQuad *image) :
	IRenderingObject(obj),
	Name(name),
	Image(image)
{
}

IScreen::IScreen(const IScreen &obj) :
	IRenderingObject(obj),
	Name(obj.Name),
	Image(obj.Image)
{
}

IScreen::~IScreen()
{
}

IScreen& IScreen::operator=(const IScreen &obj)
{
	IRenderingObject::operator=(obj);
	Name=obj.Name;
	Image=obj.Image;
	return(*this);
}

//#######################################################################################################################

CScreenBox::CScreenBox(IRenderingObjectComm *pClass,unsigned int id,IScreen *obj,const OpenUtility::CTexture *backTex,double w,double h,double posX,double posY,double texX,double texY,double texW,double texH) :
	IRenderingObject(pClass),
	Id(id),
	Screen(obj),
	BackTex(backTex),
	Background(NULL),
	Animation(7)
{
	for (unsigned int i=0;i<2;i++)
	{
		TexX[i]=texX;
		TexY[i]=texY;
		TexW[i]=texW;
		TexH[i]=texH;

		DimX[i]=w;
		DimY[i]=h;
		PosX[i]=posX;
		PosY[i]=posY;
		PosZ[i]=0;
		RotX[i]=0;
		RotY[i]=0;
	}
}

CScreenBox::~CScreenBox()
{
}

void CScreenBox::Init()
{
	Shaders=new SShaders;
	if (!Shaders->ShaderVertex.LoadFile("../shader/menu.vs"))
		std::cout << "-----------------------------------\nErreur vertex shader :\n" << Shaders->ShaderVertex.GetLog() << std::endl << "--------------------------" << std::endl;
	if (!Shaders->ShaderFragment.LoadFile("../shader/menu.fs"))
		std::cout << "-----------------------------------\nErreur fragment shader :\n" << Shaders->ShaderFragment.GetLog() << std::endl << "--------------------------" << std::endl;
	if (!Shaders->RenderingShader.LinkProgram())
		std::cout << "-----------------------------------\nErreur shader program :\n" << Shaders->RenderingShader.GetLog() << std::endl << "--------------------------" << std::endl;

	OpenUtility::RGBAd color1(0.8,0.8,0.8,1.0),color2(0.6,0.6,0.6,1.0);
	Background=new OpenUtility::CQuad(DimX[0]/2,DimY[0]/2,-DimX[0]/2,-DimY[0]/2,color1,color1,color2,color2,false);

	glGenBuffers(1,&VBObufferBackground);
	glBindBuffer(GL_ARRAY_BUFFER,VBObufferBackground);
	glBufferData(GL_ARRAY_BUFFER,4*2*2*sizeof(GLfloat),NULL,GL_STATIC_DRAW);
	UpdateTexData();

	Font=new OpenUtility::CFontLoader("../content/verdana.ttf",50);
	Title=new OpenUtility::C3DText(Font,0.0,0.28,0.46,1.0,0.5);
	Title->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignLeft,OpenUtility::CFontLoader::IFontEngine::EVAlignBaseligne);
	Title->UpdateText(Screen->GetName());

	ForceRender=true;
	State=ESDisplayMenu;
}

void CScreenBox::UnInit()
{
	delete Title;
	delete Font;
	delete Background;
	glDeleteBuffers(1,&VBObufferBackground);
	delete Shaders;
}

void CScreenBox::UpdateTexData()
{
	GLfloat data[4*2*2]={TexX[0]+TexW[0],TexY[0],0,0,TexX[0],TexY[0],1,0,TexX[0]+TexW[0],TexY[0]+TexH[0],0,1,TexX[0],TexY[0]+TexH[0],1,1};

	glBindBuffer(GL_ARRAY_BUFFER,VBObufferBackground);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(data),data);
}

bool CScreenBox::PreRender()
{
	const unsigned long long durationFullScreen=800;
	const unsigned long long durationTouch=100;
	unsigned long long timeUnit=pClass->GetTimeUnit();

	switch(State)
	{
	case ESStartAnimFullScreen:
		if (Animation.AnimStartTime>pClass->GetTimeUnit()) break;
		{
			Animation.Duration=durationFullScreen*((rand()/double(RAND_MAX)-0.5)/3.0+1.0);
			State=ESAnimFullScreen;
			float p[]={RotX[1],RotY[1],PosX[1],PosY[1],PosZ[1],DimX[1],DimY[1],0.0f,Id%3<1 ? -180.0f : Id%3>1 ? 180.0f : 180,FrontX,FrontY,FrontZ,DimW,DimH};
			float v[]={0.0f,0.0f,Id/3>0 ? -1.0f : 1.0f,(rand()/double(RAND_MAX)*2.0-1)*1.0f+1.0f,0.0f,0.0f,0.0f,-50.0f+(Id%3)*50,60.0f-(Id/3)*120,0.0f,0.0f,0.0f,0.0f,0.0f};

			Animation.AnimCoord.FlushPoints();
			Animation.AnimCoord.AddPointsVectors(p,v,2);
		}
		UdateAnimCoord(timeUnit);
		ForceRender=true;
		break;

	case ESAnimFullScreen:
		if (UdateAnimCoord(timeUnit))
		{
			State=ESDisplayFullScreen;
			pClass->OnEndAnim(this);
		}
		ForceRender=true;
	case ESDisplayFullScreen:
		break;

	case ESStartAnimTouch:
		if (Animation.AnimStartTime>pClass->GetTimeUnit()) break;
		{
			const OpenUtility::XYZd &camPos=pClass->GetCameraPos().GetPos();
			OpenUtility::XYZd vec(PosX[0],PosY[0],PosZ[0]);
			vec-=camPos;
			OpenUtility::v3dNormalize(vec);
			vec*=5;

			Animation.Duration=durationTouch;
			State=ESAnimTouch;
			float p[]={RotX[1],RotY[1],PosX[1],PosY[1],PosZ[1],DimX[1],DimY[1],0.0f,0.0f,PosX[0]+vec.x,PosY[0]+vec.y,PosZ[0]+vec.z,DimX[0],DimY[0]};
			float v[]={0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
			Animation.AnimCoord.FlushPoints();
			Animation.AnimCoord.AddPointsVectors(p,v,2);
		}
		UdateAnimCoord(timeUnit);
		ForceRender=true;
		break;

	case ESAnimTouch:
		if (UdateAnimCoord(timeUnit)) State=ESDisplayTouch;
		ForceRender=true;
	case ESDisplayTouch:
		break;

	case ESStartAnimUntouch:
		if (Animation.AnimStartTime>pClass->GetTimeUnit()) break;
		{
			Animation.Duration=durationTouch;
			State=ESAnimUntouch;
			float p[]={RotX[1],RotY[1],PosX[1],PosY[1],PosZ[1],DimX[1],DimY[1],0.0f,0.0f,PosX[0],PosY[0],PosZ[0],DimX[0],DimY[0]};
			float v[]={0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
			Animation.AnimCoord.FlushPoints();
			Animation.AnimCoord.AddPointsVectors(p,v,2);
		}
		UdateAnimCoord(timeUnit);
		ForceRender=true;
		break;

	case ESAnimUntouch:
		if (UdateAnimCoord(timeUnit)) State=ESDisplayMenu;
		ForceRender=true;
		break;

	default:
		RotX[1]=RotX[0];
		RotY[1]=RotY[0];
		PosX[1]=PosX[0];
		PosY[1]=PosY[0];
		PosZ[1]=PosZ[0];
		DimX[1]=DimX[0];
		DimY[1]=DimY[0];
		break;
	}

	if ((TexX[0]!=TexX[1]) || (TexY[0]!=TexY[1]) || (TexW[0]!=TexW[1]) || (TexH[0]!=TexH[1]))
	{
		TexX[0]=TexX[1];
		TexY[0]=TexY[1];
		TexW[0]=TexW[1];
		TexH[0]=TexH[1];

		UpdateTexData();
		ForceRender=true;
	}

	if (ForceRender)
	{
		ForceRender=false;

		MVPmatrix=ProjectionMatrix;
		MVPmatrix*=ModelViewMatrix;
		MVPmatrix*=OpenUtility::CMat4x4<float>().SetTranslate(PosX[1],PosY[1],PosZ[1]);
		MVPmatrix*=OpenUtility::CMat4x4<float>().SetRotate(RotX[1],1,0,0);
		MVPmatrix*=OpenUtility::CMat4x4<float>().SetRotate(RotY[1],0,1,0);

		MVPmatrixLogo=MVPmatrix;
		MVPmatrixLogo*=OpenUtility::CMat4x4<float>().SetTranslate(-2,1,0.01);

		MVPmatrixTitle=MVPmatrix;
		MVPmatrixTitle*=OpenUtility::CMat4x4<float>().SetTranslate(0,1,0.01);
		Title->SetDefaultShaderMatrix(MVPmatrixTitle);

		return(true);
	}
	return(false);
}

bool CScreenBox::UdateAnimCoord(unsigned long long timeUnit)
{
	double timeRatio=std::min(1.0,(timeUnit-Animation.AnimStartTime)/double(Animation.Duration));
	float p[5];
	Animation.AnimCoord.GetPoint(timeRatio,p);
	RotX[1]=p[0];
	RotY[1]=p[1];
	PosX[1]=p[2];
	PosY[1]=p[3];
	PosZ[1]=p[4];
	if (DimX[1]!=p[5] || DimY[1]!=p[6])
	{
		DimX[1]=p[5];
		DimY[1]=p[6];
		OpenUtility::RGBAd color1(0.8,0.8,0.8,1.0),color2(0.6,0.6,0.6,1.0);
		Background->UpdateCoord(DimX[1]/2,DimY[1]/2,-DimX[1]/2,-DimY[1]/2,color1,color1,color2,color2);
	}

	return(timeRatio==1.0);
}

void CScreenBox::Render()
{
	const double borderRatio=0.005;

	if (Background)
	{
		Shaders->RenderingShader.UseProgram();
		glUniform1f(Shaders->RenderingShader["u_Alpha"],1.0);
		glUniform1f(Shaders->RenderingShader["u_BorderX"],borderRatio);
		glUniform1f(Shaders->RenderingShader["u_BorderY"],borderRatio*DimX[1]/DimY[1]);
		glUniformMatrix4fv(Shaders->RenderingShader["u_MVPmatrix"],1,GL_FALSE,MVPmatrix.GetMatrix());
		glUniformMatrix4fv(Shaders->RenderingShader["u_Nmatrix"],1,GL_FALSE,NormalMatrix.GetMatrix());
		Background->AttachAttribToData(Shaders->RenderingShader["vPos"],Shaders->RenderingShader["vNorm"],Shaders->RenderingShader["vColor"]);
		glBindBuffer(GL_ARRAY_BUFFER,VBObufferBackground);
		glVertexAttribPointer(Shaders->RenderingShader["vTex"],2,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),(void*)0);
		glVertexAttribPointer(Shaders->RenderingShader["vId"],2,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),(void*)(2*sizeof(GLfloat)));
		glEnableVertexAttribArray(Shaders->RenderingShader["vTex"]);
		glEnableVertexAttribArray(Shaders->RenderingShader["vId"]);
		if (BackTex)
		{
			glActiveTexture(GL_TEXTURE0);
			glUniform1i(Shaders->RenderingShader["u_texId"],0);
			glBindTexture(GL_TEXTURE_2D,BackTex->GetId());
		}
		Background->Draw();
	}

	const OpenUtility::ITextureQuad *logo=Screen->GetLogo();
	pClass->GetGlobalShader().UseProgram();
	glUniform4f(pClass->GetGlobalShader()["u_GlobalColor"],1.0,1.0,1.0,1.0);
	glUniformMatrix4fv(pClass->GetGlobalShader()["u_MVPmatrix"],1,GL_FALSE,MVPmatrixLogo.GetMatrix());
	logo->AttachAttribToData(pClass->GetGlobalShader()["vPos"],pClass->GetGlobalShader()["vNorm"],pClass->GetGlobalShader()["vTexCoord"]);
	logo->Draw();

	Title->Draw();
}

void CScreenBox::SetMatrix(OpenUtility::CMat4x4<float> &p,OpenUtility::CMat4x4<float> &mv,OpenUtility::CMat4x4<float> &n)
{
	ProjectionMatrix=p;
	ModelViewMatrix=mv;
	NormalMatrix=n;
	MVPmatrix=p;
	MVPmatrix*=mv;
}

bool CScreenBox::TestImpact(double x,double y)
{
	double w2=DimX[0]/2,y2=DimY[0]/2;

	return(x>=PosX[0]-w2 && x<=PosX[0]+w2 && y<=PosY[0]+y2 && y>=PosY[0]-y2);
}

void CScreenBox::EventTouch()
{
	if (State==ESDisplayMenu)
	{
		State=ESStartAnimTouch;
		Animation.AnimStartTime=pClass->GetTimeUnit();
	}
}

void CScreenBox::EventUntouch()
{
	if ((State>=ESStartAnimTouch) && (State<=ESDisplayTouch))
	{
		State=ESStartAnimUntouch;
		Animation.AnimStartTime=pClass->GetTimeUnit();
	}
}

void CScreenBox::EventClick(unsigned long long timeUnit)
{
	if ((State==ESDisplayMenu) || ((State>=ESStartAnimTouch) && (State<=ESDisplayTouch)))
	{
		State=ESStartAnimFullScreen;
		if (timeUnit) Animation.AnimStartTime=timeUnit;
		else Animation.AnimStartTime=pClass->GetTimeUnit();
	}
}