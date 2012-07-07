//coding: utf-8
#ifndef _Client_h
	#define _Client_h

#include "GlWindow.h"
#include <time.h>

class Client : protected GlWindow
{
public:
	Client();
	virtual ~Client();
	void Start();

protected:
	void Init();
	void PreRender();
	void Render();

private:
	timespec DiffTime(timespec start,timespec end);

private:
	struct timespec _debTime;
};

#endif
