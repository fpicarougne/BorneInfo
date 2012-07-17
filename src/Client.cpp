//coding: utf-8
#include "Client.h"

Client::Client()
{
}

Client::~Client()
{
}

void Client::Start()
{
	OpenWindow();
}

void Client::Init()
{
	clock_gettime(CLOCK_MONOTONIC,&_debTime);

	// Set background color and clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Client::PreRender()
{
	struct timespec curTime;
	clock_gettime(CLOCK_MONOTONIC,&curTime);

	curTime=DiffTime(_debTime,curTime);
	if (curTime.tv_sec>10)
	{
//		CloseWindow();
	}
}

void Client::Render()
{
	glClearColor(0.3f, 0.4f, 0.5f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT);
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
	if (keyCode==KEY_Q)
		CloseWindow();
}

void Client::OnPeripheralAdd(unsigned int id,const char *name,EPeriphType type)
{
	std::cout << "Nouveau périphérique (id=" << id << " type=" << GlWindow::GetPeripheralTypeName(type) << ") : " << name << std::endl;
}

void Client::OnPeripheralRemove(unsigned int id,const char *name)
{
	std::cout << "Déconnexion de (id=" << id << ") : " << name << std::endl;
}

void Client::OnMouseMove(unsigned int id,double x,double y)
{
	std::cout << "Mouse (id #" << id << ") move : x=" << x << " ; y=" << y << std::endl;
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
	std::cout << "Axe (id #" << id << ") axe " << GlWindow::GetAxeName(axe) << " : val=" << val << std::endl;
}

void Client::OnKeyUp(unsigned int id,int keyCode)
{
	std::cout << "Release (id #" << id << ") : " << keyCode << std::endl;
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
