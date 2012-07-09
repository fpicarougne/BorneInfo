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
		CloseWindow();
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

void Client::OnKeyDown(char key)
{
	std::cout << "Press : " << key << std::endl;
}

void Client::OnKeyUp(char key)
{
	std::cout << "Release : " << key << std::endl;
}

void Client::OnMouseButtonDown(int id,int x,int y)
{
	std::cout << "Mouse btn #" << id " pressed : x=" << x << " ; y=" << y << std::endl;
}

void Client::OnMouseButtonUp(int id,int x,int y)
{
	std::cout << "Mouse btn #" << id " unpressed : x=" << x << " ; y=" << y << std::endl;
}

void Client::OnMouseMove(int x,int y)
{
}
