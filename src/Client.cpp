//coding: utf-8
#include "Client.h"
#include <iostream>
#include <Template/CMat4x4.h>
#include <stddef.h>

Client::Client() :
	nbIndexes(0),
	Shaders(NULL),
	TexQuad(NULL),
	TexMultiQuad(NULL),
	MouseHasMove(false),
	Font30(NULL),
	FpsText(NULL),
	TaskBar(NULL)
{
	MutexMouse=OpenUtility::InitMutex();
}

Client::~Client()
{
	OpenUtility::DestroyMutex(MutexMouse);
	delete(Shaders);
	delete(Font30);
	delete(FpsText);
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
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
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
		double width=30;

		double ratioScreen=GetHeight()/double(GetWidth());

		Frustum.pNear=0.1;
		Frustum.pFar=100.0;
//		Frustum.pRight=tan(fovy*DTOR/2.0)*Frustum.pNear;
		Frustum.pRight=Frustum.pNear*width/2/camZ;
		Frustum.pLeft=-Frustum.pRight;
		Frustum.pTop=Frustum.pRight*ratioScreen;
		Frustum.pBottom=Frustum.pLeft*ratioScreen;
//		CameraPos.Set(OpenUtility::XYZd(0.0,0.0,width/2.0/tan(fovy*DTOR/2.0)),OpenUtility::XYZd(0.0,0.0,-1.0),OpenUtility::XYZd(0.0,1.0,0.0));
		CameraPos.Set(OpenUtility::XYZd(0.0,0.0,camZ),OpenUtility::XYZd(0.0,0.0,-1.0),OpenUtility::XYZd(0.0,1.0,0.0));

		Pmatrix.SetFrustum(Frustum.pLeft,Frustum.pRight,Frustum.pBottom,Frustum.pTop,Frustum.pNear,Frustum.pFar);
		MVmatrix.SetLookAt(	CameraPos.GetPos().x,CameraPos.GetPos().y,CameraPos.GetPos().z,
							CameraPos.GetPos().x+CameraPos.GetdView().x,CameraPos.GetPos().y+CameraPos.GetdView().y,CameraPos.GetPos().z+CameraPos.GetdView().z,
							CameraPos.GetdNorm().x,CameraPos.GetdNorm().y,CameraPos.GetdNorm().z);
		Nmatrix=MVmatrix;
		MVPmatrix=Pmatrix*MVmatrix;
		glViewport(0,0,GetWidth(),GetHeight());

		GL_CHECK();

		// Content Initialisation
		DisplayFps=true;
		Font30=new OpenUtility::CFontLoader("../content/verdana.ttf",30);
		FpsText=new OpenUtility::C3DText(Font30,1,0,0,1,0.35);
		FpsText->SetAlignement(OpenUtility::CFontLoader::IFontEngine::EHAlignRight,OpenUtility::CFontLoader::IFontEngine::EVAlignBaseligne);
		OpenUtility::CMat4x4<float> FpsMatrix(MVPmatrix);
		FpsMatrix*=OpenUtility::CMat4x4<float>().SetTranslate(CameraPos.GetPos().z*Frustum.pRight/Frustum.pNear-0.5,CameraPos.GetPos().z*Frustum.pTop/Frustum.pNear-1,0);
		FpsText->SetDefaultShaderMatrix(FpsMatrix);

		TaskBar=new CTaskBar(this);
		TaskBar->SetMatrix(Pmatrix,MVmatrix,Nmatrix);
		TaskBar->Init();

		TabScreen.Add(new CScreenContact());
		for (unsigned int i=0;i<TabScreen.GetSize();i++)
		{
//			TabScreen[i]->SetMatrix(Pmatrix,MVmatrix,Nmatrix);
//			TabScreen[i]->Init();
		}

		GL_CHECK();

/*		// Data to visualize
		GL_CHECK();
		TexQuad=new OpenUtility::CTextureQuad("../content/icones3.png",20,20);
		OpenUtility::CVector<OpenUtility::CTextureMultiQuad::SQuad> vect;
		vect.Add(new OpenUtility::CTextureMultiQuad::SQuad(0,0,100,100,5,5));
		vect.Add(new OpenUtility::CTextureMultiQuad::SQuad(200,0,100,100,3,2));
		TexMultiQuad=new OpenUtility::CTextureMultiQuad("../content/icones3.png",vect);
		vect.DeleteAll();
//		TexQuad=new OpenUtility::CTextureQuad(Font40->GetFontTexture(),0.15,0.15);
*/
	}
	catch(OpenUtility::CShaderProgram::Exception e)
	{
		std::cout << e.what() << std::endl;
	}
}

void Client::Uninit()
{
	TabMice.DeleteAll();
	TabScreen.DeleteAll();
	if (TaskBar)
	{
		TaskBar->UnInit();
		delete TaskBar;
		TaskBar=NULL;
	}

/*	delete TexQuad;
	glDeleteBuffers(1,&VBObuffer);
	glDeleteBuffers(1,&VBOtex);
	glDeleteBuffers(1,&VBOindex);*/
}

bool Client::PreRender(unsigned long long timeUnit)
{
	bool MustRender=false;

	if (UpdateSpace)
	{
		UpdateSpace=false;
		MustRender=true;
	}

	if (TaskBar->PreRender(timeUnit))
	{
		MustRender=true;
	}
//	for (unsigned int i=0;i<TabScreen.GetSize();i++)
//		if (TabScreen[i]->PreRender(timeUnit)) MustRender=true;

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shaders->RenderingShader.UseProgram();

		TaskBar->Render();
//		for (unsigned int i=0;i<TabScreen.GetSize();i++)
//			TabScreen[i]->Render();

		// Mouse Render
		for (unsigned int i=0;i<TabMice.GetSize();i++)
			if (TabMice[i]) TabMice[i]->Draw();
		GL_CHECK();

/*		glUniformMatrix4fv(Shaders->RenderingShader["u_Nmatrix"],1,GL_FALSE,Nmatrix.GetMatrix());
		glUniformMatrix4fv(Shaders->RenderingShader["u_MVPmatrix"],1,GL_FALSE,MVPmatrix.GetMatrix());
		GL_CHECK();

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(Shaders->RenderingShader["u_texId"],0);

//		TexQuad->AttachAttribToData(Shaders->RenderingShader["vPos"],Shaders->RenderingShader["vNorm"],Shaders->RenderingShader["vTexCoord"]);
//		TexQuad->Draw();

//		TexMultiQuad->AttachAttribToData(Shaders->RenderingShader["vPos"],Shaders->RenderingShader["vNorm"],Shaders->RenderingShader["vTexCoord"]);
//		TexMultiQuad->Draw(1);

		GL_CHECK();*/

		// Display fps
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

void Client::OnMouseMove(unsigned int id,double x,double y)
{
	if ((TabMice.GetSize()>id) && TabMice[id])
	{
		MouseHasMove=true;
		TabMice[id]->SetRelativePosition(x,y);
	}
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

void Client::OnMouseButtonDown(unsigned int id,int b,double x,double y)
{
	std::cout << "Mouse (id #" << id << ") btn #" << b << " pressed : x=" << x << " ; y=" << y << std::endl;
}

void Client::OnMouseButtonUp(unsigned int id,int b,double x,double y)
{
	std::cout << "Mouse (id #" << id << ") btn #" << b << " unpressed : x=" << x << " ; y=" << y << std::endl;
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
