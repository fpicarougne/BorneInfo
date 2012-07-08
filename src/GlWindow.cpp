//coding: utf-8
#include "GlWindow.h"
#include <assert.h>
#include <pthread.h>
#include <time.h>
#include <ctype.h>
#include <Utility/FileHandling.h>
#include <Utility/Stream.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/time.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include "borne.h"

#define check() assert(glGetError()==0)

GlWindow::GlWindow() :
	_end(true),
	idThreadEvent(0),
	Display(0)
{
	// Control end of processes
	_mutexEnd=OpenUtility::InitMutex();
	pipe(_fdEnd);
}

GlWindow::~GlWindow()
{
	CloseWindow();
	close(_fdEnd[IN]);
	close(_fdEnd[OUT]);
}

TH_CALL_BACK(GlWindow,EventListenerCB,pParam)
{
	GlWindow *pClass=(GlWindow*)pParam;
	pClass->EventListener();
	return(0);
}

bool GlWindow::CreateOpenGlContext()
{
	int32_t success = 0;
	EGLBoolean result;
	EGLint num_config;

	static EGL_DISPMANX_WINDOW_T nativewindow;

	DISPMANX_ELEMENT_HANDLE_T dispman_element;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T dispman_update;
	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;

	static const EGLint attribute_list[] =
	{
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE
	};

	static const EGLint context_attributes[] = 
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	EGLConfig config;

	// get an EGL display connection
	Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert(Display!=EGL_NO_DISPLAY);
	check();

	// initialize the EGL display connection
	result = eglInitialize(Display, NULL, NULL);
	assert(EGL_FALSE != result);
	check();

	// get an appropriate EGL frame buffer configuration
	result = eglChooseConfig(Display, attribute_list, &config, 1, &num_config);
	assert(EGL_FALSE != result);
	check();

	// get an appropriate EGL frame buffer configuration
	result = eglBindAPI(EGL_OPENGL_ES_API);
	assert(EGL_FALSE != result);
	check();

	// create an EGL rendering context
	Context = eglCreateContext(Display, config, EGL_NO_CONTEXT, context_attributes);
	assert(Context!=EGL_NO_CONTEXT);
	check();

	// create an EGL window surface
	success = graphics_get_display_size(0 /* LCD */, &ScrWidth, &ScrHeight);
	assert( success >= 0 );

	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.width = ScrWidth;
	dst_rect.height = ScrHeight;

	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width = ScrWidth << 16;
	src_rect.height = ScrHeight << 16;        

	dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
	dispman_update = vc_dispmanx_update_start( 0 );
	 
	dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,
		0/*layer*/, &dst_rect, 0/*src*/,
		&src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);

	nativewindow.element = dispman_element;
	nativewindow.width = ScrWidth;
	nativewindow.height = ScrHeight;
	vc_dispmanx_update_submit_sync( dispman_update );

	check();

	Surface = eglCreateWindowSurface( Display, config, &nativewindow, NULL );
	assert(Surface != EGL_NO_SURFACE);
	check();

	// connect the context to the surface
	result = eglMakeCurrent(Display, Surface, Surface, Context);
	assert(EGL_FALSE != result);
	check();

	return(true);

/*
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
*/
}

void GlWindow::DestroyOpenGlContext()
{
	// Release OpenGL resources
	if (Display)
	{
		eglMakeCurrent(Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroySurface(Display, Surface);
		eglDestroyContext(Display, Context);
		eglTerminate(Display);
		Display=0;
	}
}

void GlWindow::OpenWindow()
{
	OpenUtility::MutexLock(_mutexEnd);
	if (!_end)
	{
		OpenUtility::MutexUnlock(_mutexEnd);
		return;
	}
	_end=false;
	_request_end=false;

	CreateOpenGlContext();
	OpenUtility::MutexUnlock(_mutexEnd);

	OpenUtility::CreateThread(EventListenerCB,this,&idThreadEvent);
	MainLoop();
}

void GlWindow::CloseWindow()
{
	OpenUtility::MutexLock(_mutexEnd);
	if (_end)
	{
		OpenUtility::MutexUnlock(_mutexEnd);
		return;
	}
	_request_end=true;
	OpenUtility::MutexUnlock(_mutexEnd);

	CloseEvents();
}

void GlWindow::_CloseWindow()
{
	if (idThreadEvent)
	{
		OpenUtility::WaitJoinThread(idThreadEvent);
		idThreadEvent=0;
	}
	DestroyOpenGlContext();

	OpenUtility::MutexLock(_mutexEnd);
	_end=true;
	OpenUtility::MutexUnlock(_mutexEnd);
}

#define LONG_BITS (sizeof(long) * 8)
#define TestBit(bit, array) (array[(bit) / LONG_BITS]) & (1L << ((bit) % LONG_BITS))

void GlWindow::EventListener()
{
	OpenUtility::DIRHANDLE dirH;

	if ((dirH=OpenUtility::InitDirList("/dev/input",OpenUtility::ONLY_FILE))!=NULL)
	{
		OpenUtility::CStream file;
		char model[]="event";

		CloseEvents();
		OpenUtility::MutexLock(_mutexEnd);

		while (OpenUtility::GetNextDirFile(dirH,file))
		{
			bool find=false;
			unsigned int i;

			i=0;
			while ((!find) && (model[i]!='\0') && (file.GetStream()[i]!='\0') && (model[i]==file.GetStream()[i])) i++;
			if (model[i]=='\0')
			{
				while (isdigit(file.GetStream()[i])) i++;
				if (file.GetStream()[i]=='\0')
				{
					OpenUtility::CStream url("/dev/input/");
					char name[256]= "Unknown";
					int fd=-1;
					long bitmask[EV_CNT];
					long bit;

					url+=file;
					std::cout << url << std::endl;
					if ((fd=open(url.GetStream(),O_RDONLY))<0) break;
					std::cout << "open" << std::endl;
					if (ioctl(fd,EVIOCGNAME(sizeof(name)),name)<0)
					{
						std::cout << "failed name" << std::endl;
						close(fd);
						break;
					}
					std::cout << file << " : " << name << std::endl;
					memset(bitmask,0,sizeof(bitmask));
					if (ioctl(fd,EVIOCGBIT(0,sizeof(bitmask)),bitmask)<0)
					{
						std::cout << "failed bit" << std::endl;
						close(fd);
						break;
					}
					for (bit=0;bit<EV_MAX;bit++)
					{
						if (TestBit(bit,bitmask))
						{
							switch (bit)
							{
							case EV_SYN: std::cout << "\tSynch Events" << std::endl;break;
							case EV_KEY: std::cout << "\tKeys or Buttons" << std::endl;break;
							case EV_REL: std::cout << "\tRelative Axes" << std::endl;break;
							case EV_ABS: std::cout << "\tAbsolute Axes" << std::endl;break;
							case EV_MSC: std::cout << "\tMiscellaneous" << std::endl;break;
							case EV_LED: std::cout << "\tLEDs" << std::endl;break;
							case EV_SND: std::cout << "\tSounds" << std::endl;break;
							case EV_REP: std::cout << "\tRepeat" << std::endl;break;
							case EV_FF:
							case EV_FF_STATUS: std::cout << "\tForce Feedback" << std::endl;break;
							case EV_PWR: std::cout << "\tPower Management" << std::endl;break;

							default:
								std::cout << "\tUnknown" << std::endl;
							}
						}
					}

					// Store file descriptor to check events
					std::cout << "\nPush event fd " << fd << std::endl << std::endl;
					_fdEvents.push_back(fd);
				}
			}
		}
		OpenUtility::ReleaseDirList(dirH);

		OpenUtility::MutexUnlock(_mutexEnd);

		std::cout << std::endl << "start event listener" << std::endl;

		OpenUtility::MutexLock(_mutexEnd);

		struct pollfd fds[_fdEvents.size()+1];
		int nb,retval,i;
		std::vector<int>::iterator it;

		for (it=_fdEvents.begin(),nb=0;it!=_fdEvents.end();it++,nb++)
		{
			fds[nb].fd=*it;
			fds[nb].events=POLLIN;
		}
		fds[nb].fd=_fdEnd[IN];
		fds[nb].events=POLLIN;

		OpenUtility::MutexUnlock(_mutexEnd);

		while (!_request_end)
		{
			bool data;
			char c;

			retval=poll(fds,nb+1,-1);

			if (fds[nb].revents & POLLIN) read(_fdEnd[IN],&c,1);
			for (i=0,data=false;i<nb;i++)
			{
				if (fds[i].revents & POLLIN) ReadEvent(fds[i].fd);
			}
		}
	}
}

void GlWindow::ReadEvent(int fd)
{
	struct input_event ev[64];
	size_t rb;
	int i;

	if ((rb=read(fd,ev,sizeof(ev)))<sizeof(struct input_event)) return;

	for (i=0;i<int(rb/sizeof(struct input_event));i++)
	{
		switch(ev[i].type)
		{
		case EV_KEY:
std::cout<<"Key"<<std::endl;
			switch(ev[i].value)
			{
			// Key released
			case 0:OnKeyDown(ev[i].code);break;
			// Key pressed
			case 1:OnKeyUp(ev[i].code);break;
			// Key keeping pressed
			case 2:break;
			}
			break;

		case EV_REL:
std::cout<<"Relative"<<std::endl;
			break;

		case EV_ABS:
std::cout<<"Absolute"<<std::endl;
			break;

		case EV_MSC:
std::cout<<"Miscellious"<<std::endl;
			break;

		case EV_SW:
std::cout<<"Switch"<<std::endl;
			break;

		case EV_LED:
std::cout<<"Led"<<std::endl;
			break;

		case EV_SND:
std::cout<<"Sound"<<std::endl;
			break;

		case EV_REP:
std::cout<<"Repeat"<<std::endl;
			break;

		case EV_FF:
std::cout<<"Force feedback"<<std::endl;
			break;

		case EV_PWR:
std::cout<<"Power"<<std::endl;
			break;
		}
	}
}

void GlWindow::CloseEvents()
{
	std::vector<int>::iterator it;

	std::cout << "CloseEvents" << std::endl;
	OpenUtility::MutexLock(_mutexEnd);

	for (it=_fdEvents.begin();it!=_fdEvents.end();it++)
		close(*it);
	_fdEvents.clear();
	write(_fdEnd[OUT],"",1);

	OpenUtility::MutexUnlock(_mutexEnd);
}

void GlWindow::MainLoop()
{
	struct timespec waitDelay;

	waitDelay.tv_sec=0;
	waitDelay.tv_nsec=100000000;

	Init();
	while (!_request_end)
	{
		PreRender();
		Render();

		glFlush();
		glFinish();
		check();
		eglSwapBuffers(Display, Surface);
	}
	_CloseWindow();
}
