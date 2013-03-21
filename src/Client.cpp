//coding: utf-8
#include "Client.h"
#include <iostream>
#include <Template/CMat4x4.h>
#include <stddef.h>

Client::Client() :
	nbIndexes(0),
	Shaders(NULL),
	Font40(NULL),
	TexQuad(NULL),
	TexMultiQuad(NULL),
	_3dText(NULL)
{
	MutexMouse=OpenUtility::InitMutex();
}

Client::~Client()
{
	OpenUtility::DestroyMutex(MutexMouse);
	delete(Shaders);
}

void Client::Start()
{
	OpenWindow();
}

void Client::Init()
{
	try
	{
		clock_gettime(CLOCK_MONOTONIC,&_debTime);

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

		// Data to visualize
		GL_CHECK();
		Font40=new OpenUtility::CFontLoader("../content/verdana.ttf",70);
		TexQuad=new OpenUtility::CTextureQuad("../content/icones3.png",20,20);
		OpenUtility::CVector<OpenUtility::CTextureMultiQuad::SQuad> vect;
		vect.Add(new OpenUtility::CTextureMultiQuad::SQuad(0,0,100,100,5,5));
		vect.Add(new OpenUtility::CTextureMultiQuad::SQuad(200,0,100,100,3,2));
		TexMultiQuad=new OpenUtility::CTextureMultiQuad("../content/icones3.png",vect);
		vect.DeleteAll();
//		TexQuad=new OpenUtility::CTextureQuad(Font40->GetFontTexture(),0.15,0.15);
		_3dText=new OpenUtility::C3DText(Font40);
		_3dText->SetAlignement(OpenUtility::CFontLoader::CFontEngine::EHAlignCenter,OpenUtility::CFontLoader::CFontEngine::EVAlignBaseligne);
//		_3dText->SetText("Bonjour, il est 14:12",OpenUtility::CFontLoader::CFontEngine::EHAlignCenter,OpenUtility::CFontLoader::CFontEngine::EVAlignBaseligne);
		GL_CHECK();

		// Matrix operations
		OpenUtility::CMat4x4<float> MVmatrix,Pmatrix;
		float factor=1;

//		MVmatrix*=OpenUtility::CMat4x4<float>().SetLookAt(0,2,3,0,0,0,0,1,0);
		MVmatrix*=(OpenUtility::CMat4x4<float>()).SetLookAt(0,0,1.2,0,0,0,0,1,0);
		Pmatrix.SetFrustum(-factor,factor,-factor*GetHeight()/float(GetWidth()),factor*GetHeight()/float(GetWidth()),0.1f,1000);

		Nmatrix=MVmatrix;
		MVPmatrix=Pmatrix*MVmatrix;

		glViewport(0,0,GetWidth(),GetHeight());
		GL_CHECK();
	}
	catch(OpenUtility::CShaderProgram::Exception e)
	{
		std::cout << e.what() << std::endl;
	}
}

void Client::Uninit()
{
	TabMice.DeleteAll();
	delete TexQuad;
	delete _3dText;
	delete Font40;
	glDeleteBuffers(1,&VBObuffer);
	glDeleteBuffers(1,&VBOtex);
	glDeleteBuffers(1,&VBOindex);
}

void Client::PreRender()
{
	struct timespec curTime;
	struct tm *timeTM;
	time_t timeStamp;
	static time_t savTimeStamp=0;

	timeStamp=time(NULL);
	if (savTimeStamp!=timeStamp)
	{
		savTimeStamp=timeStamp;
		timeTM=localtime(&timeStamp);
		OpenUtility::CStream strTime;

		strTime.Format("%02d:%02d:%02d",timeTM->tm_hour,timeTM->tm_min,timeTM->tm_sec);
		_3dText->UpdateText(strTime);
	}

	clock_gettime(CLOCK_MONOTONIC,&curTime);

	curTime=DiffTime(_debTime,curTime);
	if (curTime.tv_sec>10)
	{
//		CloseWindow();
	}

	OpenUtility::MutexLock(MutexMouse);
	for (unsigned int i=0;i<TabIdMice.GetSize();i++)
	{
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
}

void Client::Render()
{
	try
	{
		glClearColor(0.0f, 0.4f, 0.5f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shaders->RenderingShader.UseProgram();

		glUniformMatrix4fv(Shaders->RenderingShader["u_Nmatrix"],1,GL_FALSE,Nmatrix.GetMatrix());
		glUniformMatrix4fv(Shaders->RenderingShader["u_MVPmatrix"],1,GL_FALSE,MVPmatrix.GetMatrix());
		GL_CHECK();

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(Shaders->RenderingShader["u_texId"],0);

//		TexQuad->AttachAttribToData(Shaders->RenderingShader["vPos"],Shaders->RenderingShader["vNorm"],Shaders->RenderingShader["vTexCoord"]);
//		TexQuad->Draw();

//		TexMultiQuad->AttachAttribToData(Shaders->RenderingShader["vPos"],Shaders->RenderingShader["vNorm"],Shaders->RenderingShader["vTexCoord"]);
//		TexMultiQuad->Draw(1);

		_3dText->AttachAttribToData(Shaders->RenderingShader["vPos"],Shaders->RenderingShader["vNorm"],Shaders->RenderingShader["vTexCoord"]);
		_3dText->Draw();
		GL_CHECK();

		for (unsigned int i=0;i<TabMice.GetSize();i++)
			if (TabMice[i]) TabMice[i]->Draw();
		GL_CHECK();
	}
	catch(OpenUtility::CShaderProgram::Exception &e)
	{
		std::cout << e << std::endl;
	}
}

timespec Client::DiffTime(timespec start,timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
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
	}
}

void Client::OnPeripheralAdd(unsigned int id,const char *name,EPeriphType type)
{
	std::cout << "Nouveau périphérique (id=" << id << " type=" << GlWindow::GetPeripheralTypeName(type) << ") : " << name << std::endl;

	if (type==EPTmouse || type==EPTunknown)
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
//	std::cout << "Mouse (id #" << id << ") move : x=" << x << " ; y=" << y << std::endl;
	if ((TabMice.GetSize()>id) && TabMice[id]) TabMice[id]->SetRelativePosition(x,y);
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
		if ((TabMice.GetSize()>id) && TabMice[id]) TabMice[id]->SetRelativePosition(x,y);
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
