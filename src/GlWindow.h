//coding: utf-8
#ifndef _GlWindow_h
	#define _GlWindow_h

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <Utility/MultiThread.h>
#include <iostream>
#include <Template/Table.h>

class GlWindow
{
private:
	enum EKeyEventType
	{
		EKEpress=0,
		EKEunpress=1,
		EKEkeep=2
	};

	struct SEventKey
	{
		SEventKey(int _type,int _code) : type(_type),code(_code) {}
		int type;
		int code;
	};

	template<class T,int N> class SEventAxe
	{
	public:
		SEventAxe() {Init();}
		void Init();
		inline bool hasChanged() {return(_changed);}
		void SetAxeChange(int axe,T val);
		void SetAxe(T _axe[N],bool relative=true);

	private:
		T Axe[N];
		bool bAxe[N];
		bool _changed;
	};

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
	CTable<int> _fdEvents;
	CTable<SEventKey> _SynKey;
	CTable<SEventKey> _SynMouseBt;
	SEventAxe<int,10> _SynMouse,
	int MouseAxes[10];
};

#endif
