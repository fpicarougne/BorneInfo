//coding: utf-8
#include "Client.h"
#include <iostream>
#include <Template/CMat4x4.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "Screens/ScreenContact.h"
#include "Screens/ScreenEvent.h"
#include "Screens/ScreenMap.h"
#include "Screens/ScreenOrganigram.h"
#include "Screens/ScreenSchedule.h"
#include "Screens/ScreenTransport.h"

//#######################################################################################################################

Client::SFBO::SFBO(unsigned int w,unsigned int h)
{
	Image=OpenUtility::CTexture::InitTexture(w,h,OpenUtility::CTexture::EPModeRGBA);

	glGenRenderbuffers(1,&idDepthRB);
	glBindRenderbuffer(GL_RENDERBUFFER,idDepthRB);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,Image->GetWT(),Image->GetHT());

	glGenFramebuffers(1,&idFBO);
	glBindFramebuffer(GL_FRAMEBUFFER,idFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,Image->GetId(),0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,idDepthRB);

	GLenum iResult=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (iResult!=GL_FRAMEBUFFER_COMPLETE) std::cout << "Framebuffer incomplete" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

Client::SFBO::~SFBO()
{
	glDeleteFramebuffers(1,&idFBO);
	delete(Image);
	glDeleteRenderbuffers(1,&idDepthRB);
}

void Client::SFBO::Activate()
{
	glBindFramebuffer(GL_FRAMEBUFFER,idFBO);
	glViewport(0,0,Image->GetW(),Image->GetH());
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Client::SFBO::Unactivate()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

//#######################################################################################################################

Client::Client() :
	nbIndexes(0),
	Shaders(NULL),
	TexQuad(NULL),
	TexMultiQuad(NULL),
	MouseHasMove(false),
	Background(NULL),
	Font30(NULL),
	FpsText(NULL),
	TaskBar(NULL),
	OfflineRB(NULL),
	CurTouchScreen(NULL),
	CurDisplayScreen(NULL)
{
	srand(time(NULL));
	MutexMouse=OpenUtility::InitMutex();
}

Client::~Client()
{
	OpenUtility::DestroyMutex(MutexMouse);
	delete(Shaders);
	delete(Font30);
	delete(FpsText);
	TabScreenBox.DeleteAll();
	TabScreen.DeleteAll();
	delete TaskBar;
}

void Client::Start()
{
	OpenWindow();
}

void Client::Init()
{
	try
	{
		Shaders=new SShaders;
		if (!Shaders->ShaderVertex.LoadFile("../shader/rendering.vert"))
			std::cout << "-----------------------------------\nErreur vertex shader :\n" << Shaders->ShaderVertex.GetLog() << std::endl << "--------------------------" << std::endl;
		if (!Shaders->ShaderFragment.LoadFile("../shader/rendering.frag"))
			std::cout << "-----------------------------------\nErreur fragment shader :\n" << Shaders->ShaderFragment.GetLog() << std::endl << "--------------------------" << std::endl;
		if (!Shaders->RenderingShader.LinkProgram())
			std::cout << "-----------------------------------\nErreur shader program :\n" << Shaders->RenderingShader.GetLog() << std::endl << "--------------------------" << std::endl;

		// Set background color and clear buffers
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Initialisation of the opengl state
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

		// Matrix initialization
//		double fovy=50;
		double camZ=50;
		WidthAtZero=30;

		double ratioScreen=GetHeight()/double(GetWidth());

		Frustum.pNear=0.1;
		Frustum.pFar=100.0;
//		Frustum.pRight=tan(fovy*DTOR/2.0)*Frustum.pNear;
		Frustum.pRight=Frustum.pNear*WidthAtZero/2/camZ;
		Frustum.pLeft=-Frustum.pRight;
		Frustum.pTop=Frustum.pRight*ratioScreen;
		Frustum.pBottom=Frustum.pLeft*ratioScreen;
//		CameraPos.Set(OpenUtility::XYZd(0.0,0.0,WidthAtZero/2.0/tan(fovy*DTOR/2.0)),OpenUtility::XYZd(0.0,0.0,-1.0),OpenUtility::XYZd(0.0,1.0,0.0));
		CameraPos.Set(OpenUtility::XYZd(0.0,0.0,camZ),OpenUtility::XYZd(0.0,0.0,-1.0),OpenUtility::XYZd(0.0,1.0,0.0));

		Pmatrix.SetFrustum(Frustum.pLeft,Frustum.pRight,Frustum.pBottom,Frustum.pTop,Frustum.pNear,Frustum.pFar);
		MVmatrix.SetLookAt(	CameraPos.GetPos().x,CameraPos.GetPos().y,CameraPos.GetPos().z,
							CameraPos.GetPos().x+CameraPos.GetdView().x,CameraPos.GetPos().y+CameraPos.GetdView().y,CameraPos.GetPos().z+CameraPos.GetdView().z,
							CameraPos.GetdNorm().x,CameraPos.GetdNorm().y,CameraPos.GetdNorm().z);
		Nmatrix=MVmatrix;
		MVPmatrix=Pmatrix*MVmatrix;
		glViewport(0,0,GetWidth(),GetHeight());

		//### Content Initialisation
		///////////////////////////////////////////////////////////////////////////////////////////
		// Background
		double ratio=Frustum.pFar/Frustum.pNear;
		OpenUtility::RGBAd color1(1.0,1.0,1.0,1.0),color2(0.68,0.68,0.68,1.0);
//		OpenUtility::RGBAd color1(0.4,0.4,0.4,1.0),color2(0.1,0.1,0.1,1.0);
		Background=new OpenUtility::CQuad(Frustum.pLeft*ratio,Frustum.pBottom*ratio,Frustum.pRight*ratio,Frustum.pTop*ratio,color1,color1,color2,color2);
		OpenUtility::CMat4x4<float> MVPmatrixBg(Pmatrix);
		MVPmatrixBg*=OpenUtility::CMat4x4<float>().SetTranslate(0,0,-Frustum.pFar);
		Background->SetDefaultShaderMatrix(MVPmatrixBg,OpenUtility::CMat4x4<float>());

		///////////////////////////////////////////////////////////////////////////////////////////
		// Fps
		DisplayFps=true;
		Font30=new OpenUtility::CFontLoader("../content/verdana.ttf",30);
		FpsText=new OpenUtility::C3DText(Font30,1,0,0,1,0.35);
		FpsText->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignRight,OpenUtility::CFontLoader::IFontEngine::EVAlignBaseligne);
		OpenUtility::CMat4x4<float> FpsMatrix(MVPmatrix);
		FpsMatrix*=OpenUtility::CMat4x4<float>().SetTranslate(CameraPos.GetPos().z*Frustum.pRight/Frustum.pNear-0.5,CameraPos.GetPos().z*Frustum.pTop/Frustum.pNear-1,0);
		FpsText->SetDefaultShaderMatrix(FpsMatrix);

		///////////////////////////////////////////////////////////////////////////////////////////
		// Task bar
		const double taskbarH=1.8;
		TaskBar=new CTaskBar(this,taskbarH);
		TaskBar->SetMatrix(Pmatrix,MVmatrix,Nmatrix);
		TaskBar->Init();

		///////////////////////////////////////////////////////////////////////////////////////////
		// Offline rendering
//		OfflineRB=new SFBO(GetWidth(),GetHeight());
		OfflineRB=new SFBO(512*GetWidth()/double(GetHeight()),512);
		CurDisplayScreen=NULL;
		CurTouchScreen=NULL;
		OfflineDisplay=false;
		NullTex=OpenUtility::CTexture::InitTexture(1,1,OpenUtility::CTexture::EPModeRGBA);

		///////////////////////////////////////////////////////////////////////////////////////////
		// Screens
		OpenUtility::CVector<OpenUtility::CTextureMultiQuad::SQuad> vect;
		for (unsigned int i=0;i<6;i++)
			vect.Add(new OpenUtility::CTextureMultiQuad::SQuad(150*i,0,150,122,2));
		TexMultiQuad=new OpenUtility::CTextureMultiQuad("../content/screen_icon.png",vect);
		vect.DeleteAll();

		TabScreen.Add(new CScreenContact(this,(*TexMultiQuad)[0]));
		TabScreen.Add(new CScreenEvent(this,(*TexMultiQuad)[5]));
		TabScreen.Add(new CScreenMap(this,(*TexMultiQuad)[3]));
		TabScreen.Add(new CScreenOrganigram(this,(*TexMultiQuad)[2]));
		TabScreen.Add(new CScreenSchedule(this,(*TexMultiQuad)[1]));
		TabScreen.Add(new CScreenTransport(this,(*TexMultiQuad)[4]));

		double texW,texH;
		ScreenW=WidthAtZero/4.0;
		ScreenH=ScreenW*ratioScreen;
		texW=OfflineRB->GetTexture()->GetW()/double(OfflineRB->GetTexture()->GetWT());
		texH=OfflineRB->GetTexture()->GetH()/double(OfflineRB->GetTexture()->GetHT());
//		texH=(WidthAtZero*ratioScreen-taskbarH)/(WidthAtZero*ratioScreen);
		for (unsigned int i=0;i<TabScreen.GetSize();i++)
		{
			TabScreen[i]->SetMatrix(Pmatrix,MVmatrix,Nmatrix);
			TabScreen[i]->Init();
			CScreenBox *screenbox=new CScreenBox(this,i,TabScreen[i],NullTex/*Font30->GetFontTexture()*/,ScreenW,ScreenH,ScreenW*1.25*(double(i%3)-1),ScreenH*0.75*(2*double(i/3)-1),0,0,texW,texH);
			screenbox->Init();
			screenbox->SetMatrix(Pmatrix,MVmatrix,Nmatrix);
			TabScreenBox.Add(screenbox);
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		GL_CHECK();
	}
	catch(OpenUtility::CShaderProgram::Exception e)
	{
		std::cout << e.what() << std::endl;
	}
}

void Client::UnInit()
{
	unsigned int i;

	TabMice.DeleteAll();
	for (i=0;i<TabScreenBox.GetSize();i++)
		TabScreenBox[i]->UnInit();
	TabScreenBox.DeleteAll();
	for (i=0;i<TabScreen.GetSize();i++)
		TabScreen[i]->UnInit();
	TabScreen.DeleteAll();
	delete NullTex;
	delete TexMultiQuad;
	if (TaskBar)
	{
		TaskBar->UnInit();
		delete TaskBar;
		TaskBar=NULL;
	}
	delete Background;
	delete OfflineRB;
}

bool Client::PreRender()
{
	bool MustRender=false;
	unsigned long long timeUnit=GetTimeUnit();

	if (UpdateSpace)
	{
		UpdateSpace=false;
		MustRender=true;
	}

	// Update background
	static bool firstInit=true;
	if (firstInit)
	{
		double alphaBg=1;
		double duration=500;

		if (timeUnit<duration)
			alphaBg=1-(duration-timeUnit)/duration;
		else firstInit=false;
		Background->SetAlpha(alphaBg);
		MustRender=true;
	}

	if (TaskBar->PreRender())
	{
		MustRender=true;
	}
	for (unsigned int i=0;i<TabScreenBox.GetSize();i++)
		if (TabScreenBox[i]->PreRender()) MustRender=true;
	if (CurDisplayScreen)
	{
		if (CurDisplayScreen->GetScreenClient()->PreRender()) MustRender=true;
	}

	// Mouse Operations
	OpenUtility::MutexLock(MutexMouse);
	if (MouseHasMove)
	{
		MouseHasMove=false;
		MustRender=true;
	}
	for (unsigned int i=0;i<TabIdMice.GetSize();i++)
	{
		MustRender=true;

		int id=TabIdMice[i];
		unsigned int _id;
		if (id>0)
		{
			_id=id;
			while (TabMice.GetSize()<_id+1) TabMice.Add(NULL);
			if (TabMice[_id]) delete TabMice[_id];
			(TabMice[_id]=new CMouse(GetWidth(),GetHeight()))->SetPixelPosition(0,0);
		}
		else
		{
			_id=-id;
			if ((TabMice.GetSize()>_id) && TabMice[_id])
			{
				delete TabMice[_id];
				TabMice[_id]=NULL;
			}
		}
	}
	TabIdMice.DeleteAll();
	OpenUtility::MutexUnlock(MutexMouse);

	return(MustRender);
}

void Client::Render()
{
	static struct timespec tTime,curTime,prevTime={0,0};
	OpenUtility::CStream strTime;

	try
	{
		if (OfflineRB && OfflineDisplay && CurDisplayScreen)
		{
			OfflineRB->Activate();
			CurDisplayScreen->GetScreenClient()->Render();
			OfflineRB->Unactivate();
			glViewport(0,0,GetWidth(),GetHeight());
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		if (OfflineDisplay || !CurDisplayScreen)
		{
			Background->Draw();

			Shaders->RenderingShader.UseProgram();
			for (unsigned int i=0;i<TabScreenBox.GetSize();i++)
				TabScreenBox[i]->Render();
		}
		else
		{
			Shaders->RenderingShader.UseProgram();
			CurDisplayScreen->GetScreenClient()->Render();
		}

		Shaders->RenderingShader.UseProgram();
		TaskBar->Render();

		// Mouse Render
		for (unsigned int i=0;i<TabMice.GetSize();i++)
			if (TabMice[i]) TabMice[i]->Draw();

		// Display fps
		glDisable(GL_DEPTH_TEST);
		clock_gettime(CLOCK_MONOTONIC,&curTime);
		if (DisplayFps && (prevTime.tv_sec!=0 || prevTime.tv_nsec!=0))
		{
			tTime=DiffTime(prevTime,curTime);
			double refTime=tTime.tv_nsec;
			if (tTime.tv_sec!=0)refTime+=1000000000*tTime.tv_sec;
			strTime.Format("%.1f fps",1000000000.0/refTime);
			FpsText->UpdateText(strTime.GetStream());
			FpsText->Draw();
		}
		prevTime=curTime;
	}
	catch(OpenUtility::CShaderProgram::Exception &e)
	{
		std::cout << e << std::endl;
	}
}

void Client::OnEndAnim(IRenderingObject *obj)
{
	if (CurDisplayScreen==obj)
	{
		OfflineDisplay=false;
	}
}

void Client::TransformCoord(double &x,double &y)
{
	x=(x-0.5)*WidthAtZero;
	y=(1-y-0.5)*WidthAtZero*GetHeight()/double(GetWidth());
}

void Client::OnKeyDown(unsigned int id,int keyCode)
{
	std::cout << "Press (id #" << id << ") : " << keyCode << std::endl;
}

void Client::OnKeyUp(unsigned int id,int keyCode)
{
	std::cout << "Release (id #" << id << ") : " << keyCode << std::endl;
	switch (keyCode)
	{
	case KEY_Q:
		CloseWindow();
		break;

	case KEY_F:
		DisplayFps=!DisplayFps;
		break;

	case KEY_SPACE:
		UpdateSpace=true;
		break;
	}
}

void Client::OnPeripheralAdd(unsigned int id,const char *name,EPeriphType type)
{
	std::cout << "Nouveau périphérique (id=" << id << " type=" << GlWindow::GetPeripheralTypeName(type) << ") : " << name << std::endl;

	if (type==EPTmouse || type==EPTtouchscreen)
	{
		if (!HasLimit(id,AXE_X)) SetAxeLimit(id,AXE_X,0,GetWidth());
		if (!HasLimit(id,AXE_Y)) SetAxeLimit(id,AXE_Y,0,GetHeight());
		SetAxeRemap(id,AXE_X,0,1);
		SetAxeRemap(id,AXE_Y,0,1);

		OpenUtility::MutexLock(MutexMouse);
		TabIdMice.Push(int(id));
		OpenUtility::MutexUnlock(MutexMouse);
	}
}

void Client::OnPeripheralRemove(unsigned int id,const char *name)
{
	std::cout << "Déconnexion de (id=" << id << ") : " << name << std::endl;

	OpenUtility::MutexLock(MutexMouse);
	TabIdMice.Push(-int(id));
	OpenUtility::MutexUnlock(MutexMouse);
}

void Client::On6axisChange(unsigned int id,double x,double y,double z,double rx,double ry,double rz)
{
	std::cout << "6axis (id #" << id << ") move : x=" << x << " ; y=" << y << " ; z=" << z << " ; rx=" << rx << " ; ry=" << ry << " ; rz=" << rz << std::endl;
}

void Client::OnWheelChange(unsigned int id,double x,double y)
{
	std::cout << "Wheel (id #" << id << ") : x=" << x << " ; y=" << y << std::endl;
}

void Client::OnHatChange(unsigned int id,int sub,double x,double y)
{
	std::cout << "Hat (id #" << id << ") ID #" << sub << " : x=" << x << " ; y=" << y << std::endl;
}

void Client::OnTiltChange(unsigned int id,double x,double y)
{
	std::cout << "Tilt (id #" << id << ") : x=" << x << " ; y=" << y << std::endl;
}

void Client::OnAxeChange(unsigned int id,GlWindow::EPeriphAxe axe,double val)
{
//	std::cout << "Axe (id #" << id << ") axe " << GlWindow::GetAxeName(axe) << " : val=" << val << std::endl;
	if (axe==AXE_X || axe==AXE_Y)
	{
		double x,y;
		if (axe==AXE_X)
		{
			x=val;
			y=GetAxeValue(id,AXE_Y);
		}
		else
		{
			x=GetAxeValue(id,AXE_X);
			y=val;
		}
		if ((TabMice.GetSize()>id) && TabMice[id])
		{
			MouseHasMove=true;
			TabMice[id]->SetRelativePosition(x,y);
		}
	}
}

void Client::OnMouseMove(unsigned int id,double x,double y)
{
	OnMouseButtonDown(id,0,x,y);
	if ((TabMice.GetSize()>id) && TabMice[id])
	{
		MouseHasMove=true;
		TabMice[id]->SetRelativePosition(x,y);
	}

	if (CurTouchScreen)
	{
		bool found=false;

		TransformCoord(x,y);
		for (unsigned int i=0;!found && i<TabScreenBox.GetSize();i++)
		{
			if ((found=TabScreenBox[i]->TestImpact(x,y))==true)
			{
				if (CurTouchScreen)
				{
					if (TabScreenBox[i]!=CurTouchScreen)
					{
						CurTouchScreen->EventUntouch();
						CurTouchScreen=TabScreenBox[i];
						CurTouchScreen->EventTouch();
					}
				}
				else
				{
					CurTouchScreen=TabScreenBox[i];
					CurTouchScreen->EventTouch();
				}
			}
		}
		if (!found && CurTouchScreen)
		{
			CurTouchScreen->EventUntouch();
			CurTouchScreen=NULL;
		}
	}
}

void Client::OnMouseButtonDown(unsigned int id,int b,double x,double y)
{
//	std::cout << "Mouse (id #" << id << ") btn #" << b << " pressed : x=" << x << " ; y=" << y << std::endl;
	TransformCoord(x,y);

	if (!CurDisplayScreen)
	{
		bool found=false;

		for (unsigned int i=0;!found && i<TabScreenBox.GetSize();i++)
		{
			if ((found=TabScreenBox[i]->TestImpact(x,y))==true)
			{
				if (CurTouchScreen && (TabScreenBox[i]!=CurTouchScreen)) CurTouchScreen->EventUntouch();
				CurTouchScreen=TabScreenBox[i];
			}
		}
		if (CurTouchScreen)
		{
			if (found) CurTouchScreen->EventTouch();
			else
			{
				CurTouchScreen->EventUntouch();
				CurTouchScreen=NULL;
			}
		}
	}
}

void Client::OnMouseButtonUp(unsigned int id,int b,double x,double y)
{
//	std::cout << "Mouse (id #" << id << ") btn #" << b << " unpressed : x=" << x << " ; y=" << y << std::endl;
	TransformCoord(x,y);

	if (!CurDisplayScreen)
	{
		bool found=false;
		for (unsigned int i=0;!found && i<TabScreenBox.GetSize();i++)
		{
			if ((found=TabScreenBox[i]->TestImpact(x,y))==true)
			{
				TabScreenBox[i]->SetFrontParams(0,0,CameraPos.GetPos().z-Frustum.pNear*ScreenW/2.0/Frustum.pRight);
				TabScreenBox[i]->UpdateBackTexture(OfflineRB->GetTexture());
				OfflineDisplay=true;
				CurDisplayScreen=TabScreenBox[i];
				CurDisplayScreen->EventClick();
				if (CurTouchScreen && (CurTouchScreen!=TabScreenBox[i])) CurTouchScreen->EventUntouch();
				CurTouchScreen=NULL;
			}
		}
		if (!found && CurTouchScreen)
		{
			CurTouchScreen->EventUntouch();
			CurTouchScreen=NULL;
		}
		if (!found)
		{
			double z=CameraPos.GetPos().z-Frustum.pNear*ScreenW*3.0/2.0/Frustum.pRight;
			for (unsigned int i=0;i<TabScreenBox.GetSize();i++)
			{
				double w,h;
				const OpenUtility::CTexture *tex=(*TexMultiQuad)[0]->GetTexture();

				w=ScreenW;
				h=ScreenW*3.0/double(GetWidth())*GetHeight()/2.0;
				TabScreenBox[i]->SetFrontParams((double(i%3)-1.0)*w,(double(i/3)-0.5)*h,z,w,h);
				TabScreenBox[i]->UpdateBackTexture(tex);

				w=tex->GetW()/double(tex->GetWT())/3.0;
				h=tex->GetH()/double(tex->GetHT())/2.0;
				TabScreenBox[i]->SetTexParams((i%3)*w,(i/3)*h,w,h);
				TabScreenBox[i]->EventClick(GetTimeUnit()+rand()/double(RAND_MAX)*200);
				TaskBar->Hide();
			}
		}
	}
}

void Client::OnTouchDown(unsigned int id,int b,double x,double y)
{
//	std::cout << "Touch (id #" << id << ") btn #" << b << " pressed : x=" << x << " ; y=" << y << std::endl;
}

void Client::OnTouchUp(unsigned int id,int b,double x,double y)
{
//	std::cout << "Touch (id #" << id << ") btn #" << b << " unpressed : x=" << x << " ; y=" << y << std::endl;
	OnMouseButtonUp(id,b,x,y);
}

void Client::OnButtonDown(unsigned int id,int b)
{
	std::cout << "Button (id #" << id << ") btn #" << b << " pressed" << std::endl;
}

void Client::OnButtonUp(unsigned int id,int b)
{
	std::cout << "Button (id #" << id << ") btn #" << b << " unpressed" << std::endl;
}

void Client::OnJoystickButtonDown(unsigned int id,int b)
{
	std::cout << "Joystick (id #" << id << ") btn #" << b << " pressed" << std::endl;
}

void Client::OnJoystickButtonUp(unsigned int id,int b)
{
	std::cout << "Joystick (id #" << id << ") btn #" << b << " unpressed" << std::endl;
}

void Client::OnGamepadButtonDown(unsigned int id,int b)
{
	std::cout << "Gamepad (id #" << id << ") btn #" << b << " pressed" << std::endl;
}

void Client::OnGamepadButtonUp(unsigned int id,int b)
{
	std::cout << "Gamepad (id #" << id << ") btn #" << b << " unpressed" << std::endl;
}
