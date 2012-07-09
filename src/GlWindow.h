//coding: utf-8
#ifndef _GlWindow_h
	#define _GlWindow_h

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <Utility/MultiThread.h>
#include <iostream>
#include <vector>

class GlWindow
{
public:
	GlWindow();
	virtual ~GlWindow();

protected:
	// Window information
	inline EGLDisplay GetDisplay() {return(Display);}
	inline EGLSurface GetSurface() {return(Surface);}
	inline EGLContext GetContext() {return(Display);}
	inline uint32_t GetWidth() {return(ScrWidth);}
	inline uint32_t GetHeight() {return(ScrHeight);}
	// Window managing
	void OpenWindow();
	void CloseWindow();
	// OpenGL rendering
	virtual void Init() {}
	virtual void PreRender() {}
	virtual void Render() {}
	// Event catcher
	virtual void OnKeyDown(int keyCode) {}
	virtual void OnKeyUp(int keyCode) {}
	virtual void OnMouseButtonDown(int id,int x,int y) {}
	virtual void OnMouseButtonUp(int id,int x,int y) {}
	virtual void OnMouseMove(int x,int y) {}

private:
	GlWindow(const GlWindow &obj) {}
	GlWindow* operator=(const GlWindow &obj) {return(NULL);}
	bool CreateOpenGlContext();
	void DestroyOpenGlContext();
	TH_CALL_BACK_PROTO(EventListenerCB,pParam);
	void EventListener();
	void ReadEvent(int fd);
	void CloseEvents();
	void MainLoop();
	void _CloseWindow();

private:
	bool _end;
	bool _request_end;
	MUTEX _mutexEnd;
	THREAD_ID idThreadEvent;
	int _fdEnd[2];
	// Screen info
	uint32_t ScrWidth,ScrHeight;
	// OpenGL stuff
	EGLDisplay Display;
	EGLSurface Surface;
	EGLContext Context;
	// Event stuff
	std::vector<int> _fdEvents;
	int MouseX,MouseY;
};

#endif
