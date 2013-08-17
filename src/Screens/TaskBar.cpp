//coding: utf-8
#include "TaskBar.h"

static const char QuadVertex[]="\
attribute vec4 vPos;\n\
attribute vec3 vNorm;\n\
attribute vec4 vColor;\n\
\n\
uniform mat4 u_MVPmatrix;\n\
uniform mat4 u_Nmatrix;\n\
\n\
varying vec4 v_Color;\n\
varying vec3 v_Normal;\n\
\n\
void main()\n\
{\n\
	v_Color = vColor;\n\
	v_Normal = mat3(u_Nmatrix) * vNorm;\n\
	gl_Position = u_MVPmatrix * vPos;\n\
}\n\
";

static const char QuadFragment[]="\
precision mediump float;\n\
\n\
uniform float u_Alpha;\n\
\n\
varying vec4 v_Color;\n\
varying vec3 v_Normal;\n\
\n\
void main()\n\
{\n\
	gl_FragColor = vec4(v_Color.rgb,v_Color.a*u_Alpha);\n\
}\n\
";

//#############################################################################################

CTaskBar::CQuad::CQuad(double x1,double y1,double x2,double y2,double r,double g,double b,double a)
{
	SVertex VertexArray[4];

	OpenUtility::RGBAd color(r,g,b,a);
	Update(VertexArray,x1,y1,x2,y2,color,color,color,color);
	glGenBuffers(1,&VBObuffer);
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(VertexArray),VertexArray,GL_DYNAMIC_DRAW);
}

CTaskBar::CQuad::CQuad(double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &tl,const OpenUtility::RGBAd &tr,const OpenUtility::RGBAd &bl,const OpenUtility::RGBAd &br)
{
	SVertex VertexArray[4];

	Update(VertexArray,x1,y1,x2,y2,tl,tr,bl,br);
	glGenBuffers(1,&VBObuffer);
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(VertexArray),VertexArray,GL_DYNAMIC_DRAW);
}

CTaskBar::CQuad::~CQuad()
{
	glDeleteBuffers(1,&VBObuffer);
}

void CTaskBar::CQuad::UpdateCoord(double x1,double y1,double x2,double y2,double r,double g,double b,double a)
{
	OpenUtility::RGBAd color(r,g,b,a);
	UpdateCoord(x1,y1,x2,y2,color,color,color,color);
}

void CTaskBar::CQuad::UpdateCoord(double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &tl,const OpenUtility::RGBAd &tr,const OpenUtility::RGBAd &bl,const OpenUtility::RGBAd &br)
{
	SVertex VertexArray[4];

	Update(VertexArray,x1,y1,x2,y2,tl,tr,bl,br);
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(VertexArray),VertexArray);
}

void CTaskBar::CQuad::Update(SVertex VertexArray[4],double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &tl,const OpenUtility::RGBAd &tr,const OpenUtility::RGBAd &bl,const OpenUtility::RGBAd &br)
{
	double t;

	if (x1>x2)
	{
		t=x1;
		x1=x2;
		x2=t;
	}
	if (y1>y2)
	{
		t=y1;
		y1=y2;
		y2=t;
	}

	SetVertex(VertexArray[0],x1,y1,bl.r,bl.g,bl.b,bl.a);
	SetVertex(VertexArray[1],x2,y1,br.r,br.g,br.b,br.a);
	SetVertex(VertexArray[2],x1,y2,tl.r,tl.g,tl.b,tl.a);
	SetVertex(VertexArray[3],x2,y2,tr.r,tr.g,tr.b,tr.a);
}

void CTaskBar::CQuad::SetVertex(SVertex &vertex,double posX,double posY,double r,double g,double b,double a)
{
	vertex.position[0]=posX;
	vertex.position[1]=posY;
	vertex.position[2]=0;
	vertex.normal[0]=0;
	vertex.normal[1]=0;
	vertex.normal[2]=1.0;
	vertex.color[0]=r;
	vertex.color[1]=g;
	vertex.color[2]=b;
	vertex.color[3]=a;
}

void CTaskBar::CQuad::AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vColor)
{
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glVertexAttribPointer(vPos,3,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,position));
	glVertexAttribPointer(vNorm,3,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,normal));
	glVertexAttribPointer(vColor,4,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,color));
	glEnableVertexAttribArray(vPos);
	glEnableVertexAttribArray(vNorm);
	glEnableVertexAttribArray(vColor);
}

void CTaskBar::CQuad::Draw()
{
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
}

//#############################################################################################

CTaskBar::CTaskBar(IRenderingObjectComm *pClass) :
	IRenderingObject(pClass),
	QuadShader(NULL),
	Font30(NULL),
	Font50(NULL),
	Day(NULL),
	Date(NULL),
	HourMin(NULL),
	Seconds(NULL),
	Logo(NULL),
	Background(NULL),
	Bg(NULL)
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
	QuadShader=new SShaders;
	if (!QuadShader->ShaderVertex.LoadStream(QuadVertex))
		std::cout << "-----------------------------------\nCTaskBar: Erreur vertex shader :\n" << QuadShader->ShaderVertex.GetLog() << std::endl << "--------------------------" << std::endl;
	if (!QuadShader->ShaderFragment.LoadStream(QuadFragment))
		std::cout << "-----------------------------------\nCTaskBar: Erreur fragment shader :\n" << QuadShader->ShaderFragment.GetLog() << std::endl << "--------------------------" << std::endl;
	if (!QuadShader->RenderingShader.LinkProgram())
		std::cout << "-----------------------------------\nCTaskBar: Erreur shader program :\n" << QuadShader->RenderingShader.GetLog() << std::endl << "--------------------------" << std::endl;

	Font30=new OpenUtility::CFontLoader("../content/verdana.ttf",30);
	Font50=new OpenUtility::CFontLoader("../content/verdana.ttf",50);
	Day=new OpenUtility::C3DText(Font30,0.0,0.28,0.46,1.0,0.35);
	Date=new OpenUtility::C3DText(Font30,0.0,0.28,0.46,1.0,0.35);
	HourMin=new OpenUtility::C3DText(Font50,0.0,0.28,0.46,1.0);
	Seconds=new OpenUtility::C3DText(Font50,0.0,0.28,0.46,1.0,0.75);
	Logo=new OpenUtility::CTextureQuad("../content/polytech.png",15,1.7);
	Background=new CQuad(-0.5,-0.5,0.5,0.5);
	Bg=new CQuad(-0.5,-0.5,0.5,0.5);

	Day->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignRight,OpenUtility::CFontLoader::IFontEngine::EVAlignBaseligne);
	Date->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignRight,OpenUtility::CFontLoader::IFontEngine::EVAlignBaseligne);
	HourMin->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignRight,OpenUtility::CFontLoader::IFontEngine::EVAlignBaseligne);
	Seconds->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignRight,OpenUtility::CFontLoader::IFontEngine::EVAlignBaseligne);
	MatrixHasChanged=true;
}

void CTaskBar::UnInit()
{
	delete Bg;
	delete Background;
	delete Logo;
	delete Day;
	delete Date;
	delete HourMin;
	delete Seconds;
	delete Font30;
	delete Font50;
	delete QuadShader;
}

bool CTaskBar::PreRender(unsigned long long timeUnit)
{
	struct tm timeTM;
	bool hasChanged;
	static struct tm lastTM={0,0,0,0,0,0,0,0,0};
	OpenUtility::CStream strTime;
	time_t timeStamp;

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

	const unsigned long long animTime=1000;
	const unsigned long long animTime2Deb=4500;
	const unsigned long long animTime2End=5000;
	if (MatrixHasChanged || timeUnit<animTime+1000 || (timeUnit>animTime2Deb && timeUnit<animTime2End))
	{
		const OpenUtility::SFrustum &frustum=pClass->GetFrustum();
		double dist=OpenUtility::v3dModule(pClass->GetCameraPos().GetPos());
		double l,r,t,b;
		double taskBarHeight=1.8;

		r=dist*frustum.pRight/frustum.pNear;
		l=dist*frustum.pLeft/frustum.pNear;
		t=dist*frustum.pTop/frustum.pNear;
		b=dist*frustum.pBottom/frustum.pNear;

		MVPmatrixSeconds=MVPmatrixLogo=MVPmatrixBackground=MVPmatrixBg=ProjectionMatrix*ModelViewMatrix;
		if (timeUnit>animTime2Deb && timeUnit<animTime2End)
		{
			double factor=0.8;
			BgAlpha=std::max(0.0,((animTime2End-animTime2Deb)*factor+animTime2Deb-timeUnit)/double(animTime2End-animTime2Deb)*factor);
			MVPmatrixSeconds*=OpenUtility::CMat4x4<float>().SetTranslate(0,-taskBarHeight*(1-(animTime2End-timeUnit)/double(animTime2End-animTime2Deb)),0);
			OpenUtility::RGBAd color(0.0,0.28,0.46,BgAlpha);
			Day->SetDefaultShaderColor(color);
			Date->SetDefaultShaderColor(color);
			HourMin->SetDefaultShaderColor(color);
			Seconds->SetDefaultShaderColor(color);
		}
		MVPmatrixSeconds*=OpenUtility::CMat4x4<float>().SetTranslate(r-0.5,b,0);
		if (timeUnit<animTime) MVPmatrixSeconds*=OpenUtility::CMat4x4<float>().SetRotate(-120+120*timeUnit/double(animTime),1,0,0);
		MVPmatrixDay=MVPmatrixDate=MVPmatrixHourMin=MVPmatrixSeconds;
		MVPmatrixDay*=OpenUtility::CMat4x4<float>().SetTranslate(-4.3,0.4+0.5,0);
		MVPmatrixDate*=OpenUtility::CMat4x4<float>().SetTranslate(-4.3,0.05+0.5,0);
		MVPmatrixHourMin*=OpenUtility::CMat4x4<float>().SetTranslate(-1.1,0+0.5,0);
		MVPmatrixSeconds*=OpenUtility::CMat4x4<float>().SetTranslate(0,0.18+0.5,0);

		Day->SetDefaultShaderMatrix(MVPmatrixDay);
		Date->SetDefaultShaderMatrix(MVPmatrixDate);
		HourMin->SetDefaultShaderMatrix(MVPmatrixHourMin);
		Seconds->SetDefaultShaderMatrix(MVPmatrixSeconds);

		if (timeUnit>animTime2Deb && timeUnit<animTime2End)
		{
			MVPmatrixLogo*=OpenUtility::CMat4x4<float>().SetTranslate(0,-taskBarHeight*(1-(animTime2End-timeUnit)/double(animTime2End-animTime2Deb)),0);
		}
		MVPmatrixLogo*=OpenUtility::CMat4x4<float>().SetTranslate(0,b,0);
		if (timeUnit<animTime) MVPmatrixLogo*=OpenUtility::CMat4x4<float>().SetRotate(-120+120*timeUnit/double(animTime),1,0,0);
		MVPmatrixLogo*=OpenUtility::CMat4x4<float>().SetTranslate(l+Logo->GetW()/2+0.7,taskBarHeight/2,0);

		Background->UpdateCoord(l,0,r,taskBarHeight,OpenUtility::RGBAd(1.0,1.0,1.0,1.0),OpenUtility::RGBAd(1.0,1.0,1.0,1.0),OpenUtility::RGBAd(0.7,0.7,0.7,1.0),OpenUtility::RGBAd(0.7,0.7,0.7,1.0));
		if (timeUnit>animTime2Deb && timeUnit<animTime2End)
		{
			MVPmatrixBackground*=OpenUtility::CMat4x4<float>().SetTranslate(0,-taskBarHeight*(1-(animTime2End-timeUnit)/double(animTime2End-animTime2Deb)),0);
		}
		else BgAlpha=1;
		MVPmatrixBackground*=OpenUtility::CMat4x4<float>().SetTranslate(0,b,0);
		if (timeUnit<animTime) MVPmatrixBackground*=OpenUtility::CMat4x4<float>().SetRotate(-120+120*timeUnit/double(animTime),1,0,0);

		if (timeUnit<animTime) Bg->UpdateCoord(l*10,b*10,r*10,t*10,OpenUtility::RGBAd(1.0,1.0,1.0,timeUnit/double(animTime)),OpenUtility::RGBAd(1.0,1.0,1.0,timeUnit/double(animTime)),OpenUtility::RGBAd(0.68,0.68,0.68,timeUnit/double(animTime)),OpenUtility::RGBAd(0.68,0.68,0.68,timeUnit/double(animTime)));
		else Bg->UpdateCoord(l*frustum.pFar,b*frustum.pFar,r*frustum.pFar,t*frustum.pFar,OpenUtility::RGBAd(1.0,1.0,1.0,1.0),OpenUtility::RGBAd(1.0,1.0,1.0,1.0),OpenUtility::RGBAd(0.68,0.68,0.68,1.0),OpenUtility::RGBAd(0.68,0.68,0.68,1.0));
		MVPmatrixBg*=OpenUtility::CMat4x4<float>().SetTranslate(0,0,-(frustum.pFar-dist));

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

		QuadShader->RenderingShader.UseProgram();

		glUniform1f(QuadShader->RenderingShader["u_Alpha"],1.0);
		glUniformMatrix4fv(QuadShader->RenderingShader["u_MVPmatrix"],1,GL_FALSE,MVPmatrixBg.GetMatrix());
		glUniformMatrix4fv(QuadShader->RenderingShader["u_Nmatrix"],1,GL_FALSE,ModelViewMatrix.GetMatrix());
		Bg->AttachAttribToData(QuadShader->RenderingShader["vPos"],QuadShader->RenderingShader["vNorm"],QuadShader->RenderingShader["vColor"]);
		Bg->Draw();

		glUniform1f(QuadShader->RenderingShader["u_Alpha"],BgAlpha);
		glUniformMatrix4fv(QuadShader->RenderingShader["u_MVPmatrix"],1,GL_FALSE,MVPmatrixBackground.GetMatrix());
		glUniformMatrix4fv(QuadShader->RenderingShader["u_Nmatrix"],1,GL_FALSE,ModelViewMatrix.GetMatrix());
		Background->AttachAttribToData(QuadShader->RenderingShader["vPos"],QuadShader->RenderingShader["vNorm"],QuadShader->RenderingShader["vColor"]);
		Background->Draw();

		pClass->GetGlobalShader().UseProgram();
		glUniform4f(pClass->GetGlobalShader()["u_GlobalColor"],1.0,1.0,1.0,BgAlpha);
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
