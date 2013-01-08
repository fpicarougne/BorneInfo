//coding: utf-8
#include "GlWindow.h"
#include <assert.h>
#include <pthread.h>
#include <time.h>
#include <ctype.h>
#include <Utility/FileSystem/FileHandling.h>
#include <sys/types.h>
#include <sys/time.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <sys/inotify.h>
#include <limits.h>
#include <stdlib.h>
#include "borne.h"

const char DirEventFile[]="/dev/input/by-path/";

// ----- GlWindow
// -------------------------------------

GlWindow::GlWindow() :
	_end(true)
	,idThreadEvent(0)
	,Display(0)
{
	// Control end of processes
	_mutexEnd=OpenUtility::InitMutex();
	_mutexMouse=OpenUtility::InitMutex();
	pipe(_fdEnd);
}

GlWindow::~GlWindow()
{
	CloseWindow();
	close(_fdEnd[IN]);
	close(_fdEnd[OUT]);
	ListPeriph.DeleteAll();
	OpenUtility::DestroyMutex(_mutexMouse);
	OpenUtility::DestroyMutex(_mutexEnd);
}

void GlWindow::CheckGl(const char *file,int line)
{
	GLenum err;

	if ((err=glGetError())!=GL_NO_ERROR)
	{
		OpenUtility::CStream errStr;
		switch(err)
		{
#ifdef GL_INVALID_ENUM
		case GL_INVALID_ENUM:errStr="Invalid enum";break;
#endif
#ifdef GL_INVALID_VALUE
		case GL_INVALID_VALUE:errStr="Invalid value";break;
#endif
#ifdef GL_INVALID_OPERATION
		case GL_INVALID_OPERATION:errStr="Invalid operation";break;
#endif
#ifdef GL_STACK_OVERFLOW
		case GL_STACK_OVERFLOW:errStr="Stack overflow";break;
#endif
#ifdef GL_STACK_UNDERFLOW
		case GL_STACK_UNDERFLOW:errStr="Stack underflow";break;
#endif
#ifdef GL_OUT_OF_MEMORY
		case GL_OUT_OF_MEMORY:errStr="Out of memory";break;
#endif
#ifdef GL_TABLE_TOO_LARGE
		case GL_TABLE_TOO_LARGE:errStr="Table size exceeded";break;
#endif
		default:errStr.Format("Unknow error 0x%08.8X",err);break;
		}
		std::cout << "OpenGl error" << std::endl << "\t" << file << " (line " << line << ")" << std::endl << "\t" << errStr << std::endl;
	}
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
		EGL_DEPTH_SIZE, 16,
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
	GL_CHECK();

	// initialize the EGL display connection
	result = eglInitialize(Display, NULL, NULL);
	assert(EGL_FALSE != result);
	GL_CHECK();

	// get an appropriate EGL frame buffer configuration
	result = eglChooseConfig(Display, attribute_list, &config, 1, &num_config);
	assert(EGL_FALSE != result);
	GL_CHECK();

	// get an appropriate EGL frame buffer configuration
	result = eglBindAPI(EGL_OPENGL_ES_API);
	assert(EGL_FALSE != result);
	GL_CHECK();

	// create an EGL rendering context
	Context = eglCreateContext(Display, config, EGL_NO_CONTEXT, context_attributes);
	assert(Context!=EGL_NO_CONTEXT);
	GL_CHECK();

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

	GL_CHECK();

	Surface = eglCreateWindowSurface( Display, config, &nativewindow, NULL );
	assert(Surface != EGL_NO_SURFACE);
	GL_CHECK();

	// connect the context to the surface
	result = eglMakeCurrent(Display, Surface, Surface, Context);
	assert(EGL_FALSE != result);
	GL_CHECK();

	return(true);

	glBindFramebuffer(GL_FRAMEBUFFER,0);
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

void GlWindow::EventListener()
{
	OpenUtility::DIRHANDLE dirH;

	if ((dirH=OpenUtility::InitDirList("/dev/input",OpenUtility::ONLY_FILE))!=NULL)
	{
		OpenUtility::CStream file;

		CloseEvents();
		OpenUtility::MutexLock(_mutexEnd);

		while (OpenUtility::GetNextDirFile(dirH,file))
		{
			OpenUtility::CStream url("/dev/input/");
			url+=file;
			AddPeripheral(url.GetStream());
		}
		OpenUtility::ReleaseDirList(dirH);

		unsigned int fdI,wd;

		fdI=inotify_init();
		wd=inotify_add_watch(fdI,DirEventFile,IN_CREATE);

		OpenUtility::MutexUnlock(_mutexEnd);

		struct pollfd *fds=NULL;
		OpenUtility::CListe<SPeripheral>::CListeIterator *tabPeriph=NULL;
		bool initPoll;
		unsigned int nb,i,rNb;
		struct inotify_event *event;
		char buffer[(sizeof(struct inotify_event)+(FILENAME_MAX/sizeof(struct inotify_event)+1)*sizeof(struct inotify_event)+1)*100];

		initPoll=true;
		while (!_request_end)
		{
			char c;

			if (initPoll)
			{
				OpenUtility::MutexLock(_mutexEnd);

				fds=(struct pollfd*)realloc(fds,(ListPeriph.GetSize()+2)*sizeof(struct pollfd));
				delete[] tabPeriph;
				tabPeriph=new OpenUtility::CListe<SPeripheral>::CListeIterator[ListPeriph.GetSize()];

				for (nb=0;nb<ListPeriph.GetSize();nb++)
				{
					fds[nb].fd=ListPeriph[nb]->GetFdEvent();
					fds[nb].events=POLLIN;
					ListPeriph.ElementAt(nb,&tabPeriph[nb]);
				}
				fds[nb].fd=fdI;
				fds[nb].events=POLLIN;
				fds[nb+1].fd=_fdEnd[IN];
				fds[nb+1].events=POLLIN;

				OpenUtility::MutexUnlock(_mutexEnd);

				initPoll=false;
			}

			// Wait for events
			if (poll(fds,nb+2,-1)<0)
				break;

			// Check end event
			if (fds[nb+1].revents & POLLIN) read(_fdEnd[IN],&c,1);
			for (i=0;i<nb;i++)
			{
				// Check remove peripherical
				if (fds[i].revents & (POLLERR|POLLHUP))
				{
					// Disconnection of the device
					OpenUtility::MutexLock(_mutexEnd);
					RemovePeripheral(tabPeriph[i]);
					OpenUtility::MutexUnlock(_mutexEnd);

					initPoll=true;
				}
				// Check event
				else if (fds[i].revents & POLLIN)
				{
					OpenUtility::MutexLock(_mutexEnd);
					if (ListPeriph.GetSize()) ReadEvent(ListPeriph[tabPeriph[i]]);
					OpenUtility::MutexUnlock(_mutexEnd);
				}
			}

			// Check add peripherical (necessarly after removing peripjerical due to recycling event files)
			if (fds[nb].revents & POLLIN)
			{
				rNb=read(fdI,buffer,sizeof(buffer));
				unsigned int ri=0;
				while (ri<rNb)
				{
					event=(struct inotify_event*)&buffer[ri];
					if (event->mask & IN_CREATE)
					{
						// New file in the input event directory
						if (event->len)
						{
							OpenUtility::MutexLock(_mutexEnd);
							OpenUtility::CStream url(DirEventFile);
							url+=event->name;
							AddPeripheral(url.GetStream());
							OpenUtility::MutexUnlock(_mutexEnd);
						}
						initPoll=true;
					}
					ri+=sizeof(struct inotify_event)+event->len;
				}
			}
		}
		inotify_rm_watch(fdI,wd);
		free(fds);
		delete[] tabPeriph;
	}
}

bool GlWindow::AddPeripheral(char *file)
{
	char model[]="/dev/input/event";
	char buffer[PATH_MAX+1];
	bool found;
	unsigned int i;

	// Get real file (follow links, ...)
	realpath(file,buffer);

	// Check if file correspond to pattern
	i=0;
	found=false;
	while ((!found) && (model[i]!='\0') && (buffer[i]!='\0') && (model[i]==buffer[i])) i++;
	if (model[i]!='\0') return(false);
	while (isdigit(buffer[i])) i++;
	if (buffer[i]!='\0') return(false);

	// Generate peripherical
	SPeripheral *periph;
	if ((periph=SPeripheral::GenPeripheral(buffer))==NULL) return(false);

	ListPeriph.Add(periph);
	OnPeripheralAdd(periph->GetId(),periph->GetName(),periph->GetType());

	return(true);
}

bool GlWindow::RemovePeripheral(OpenUtility::CListe<SPeripheral>::CListeIterator &pos)
{
	SPeripheral* periph=ListPeriph.Remove(pos);
	OnPeripheralRemove(periph->GetId(),periph->GetName());
	delete periph;

	return(true);
}

void GlWindow::ReadEvent(SPeripheral *periph)
{
	struct input_event ev[64];
	size_t rb;
	unsigned int i,nb,id;

	if ((rb=read(periph->GetFdEvent(),ev,sizeof(ev)))<sizeof(struct input_event)) return;

	id=periph->GetId();
	nb=int(rb/sizeof(struct input_event));
	for (i=0;i<nb;i++)
	{
		switch(ev[i].type)
		{
		case EV_SYN:
//std::cout << "EV_SYN: " << ev[i].code << std::endl;
			OpenUtility::CTable<SEventValue> *_SynEvt;
			if (periph->GetKeys() && periph->GetKeys()->_SynKey.GetSize())
			{
				_SynEvt=&periph->GetKeys()->_SynKey;
				for (unsigned int j=0;ListPeriph.GetSize() && (j<_SynEvt->GetSize());j++)
				{
					int val=_SynEvt->ElementAt(j).value;
					switch(_SynEvt->ElementAt(j).type)
					{
					// Key released
					case 0:
						if ((val>=BTN_MOUSE) && (val<BTN_JOYSTICK))
						{
							int x=0,y=0;
							if (periph->GetAxes())
							{
								if (TEST_BIT(AXE_X,periph->GetAxes()->AxeMap)) x=periph->GetAxes()->AxeValues[AXE_X].GetValue();
								if (TEST_BIT(AXE_Y,periph->GetAxes()->AxeMap)) y=periph->GetAxes()->AxeValues[AXE_Y].GetValue();
							}
							OpenUtility::MutexUnlock(_mutexEnd);
							OnMouseButtonUp(id,val-BTN_MOUSE,x,y);
							OpenUtility::MutexLock(_mutexEnd);
						}
						else
						{
							OpenUtility::MutexUnlock(_mutexEnd);
							if ((val>=BTN_MISC) && (val<BTN_MOUSE)) OnButtonUp(periph->GetId(),val-BTN_MISC);
							else if ((val>=BTN_JOYSTICK) && (val<BTN_GAMEPAD)) OnJoystickButtonUp(id,val-BTN_JOYSTICK);
							else if ((val>=BTN_GAMEPAD) && (val<BTN_DIGI)) OnGamepadButtonUp(id,val-BTN_GAMEPAD);
							else OnKeyUp(id,val);
							OpenUtility::MutexLock(_mutexEnd);
						}
						break;
					// Key pressed
					case 1:
						if ((val>=BTN_MOUSE) && (val<BTN_JOYSTICK))
						{
							double x=0,y=0;
							if (periph->GetAxes())
							{
								if (TEST_BIT(AXE_X,periph->GetAxes()->AxeMap)) x=periph->GetAxes()->AxeValues[AXE_X].GetValue();
								if (TEST_BIT(AXE_Y,periph->GetAxes()->AxeMap)) y=periph->GetAxes()->AxeValues[AXE_Y].GetValue();
							}
							OpenUtility::MutexUnlock(_mutexEnd);
							OnMouseButtonDown(id,val-BTN_MOUSE,x,y);
							OpenUtility::MutexLock(_mutexEnd);
						}
						else
						{
							OpenUtility::MutexUnlock(_mutexEnd);
							if ((val>=BTN_MISC) && (val<BTN_MOUSE)) OnButtonDown(id,val-BTN_MISC);
							else if ((val>=BTN_JOYSTICK) && (val<BTN_GAMEPAD)) OnJoystickButtonDown(id,val-BTN_JOYSTICK);
							else if ((val>=BTN_GAMEPAD) && (val<BTN_DIGI)) OnGamepadButtonDown(id,val-BTN_GAMEPAD);
							else OnKeyDown(id,val);
							OpenUtility::MutexLock(_mutexEnd);
						}
						break;
					// Key keeping pressed
					case 2:break;
					}
				}
				if (ListPeriph.GetSize()) _SynEvt->DeleteAll();
			}
			if (periph->GetAxes() && periph->GetAxes()->_SynAxe.GetSize())
			{
				long AxeMap[NLONGS(AXE_UNKNOWN+1)];
				_SynEvt=&periph->GetAxes()->_SynAxe;
				bzero(AxeMap,NLONGS(AXE_UNKNOWN+1)*sizeof(long));

				for (unsigned int j=0;j<_SynEvt->GetSize();j++)
				{
					int type=_SynEvt->ElementAt(j).type;
					int val=_SynEvt->ElementAt(j).value;
					EPeriphAxe axe=AXE_UNKNOWN;

					switch(_SynEvt->ElementAt(j).data)
					{
					case EETrel:
						axe=GetAxeBit(type,true);
						if (periph->GetAxes() && TEST_BIT(axe,periph->GetAxes()->AxeMap))
							periph->GetAxes()->AxeValues[axe].SetRelativeValue(val);
						break;

					case EETabs:
						axe=GetAxeBit(type,false);
						if (periph->GetAxes() && TEST_BIT(axe,periph->GetAxes()->AxeMap))
							periph->GetAxes()->AxeValues[axe].SetValue(val);
						break;

					default:
						break;
					}
					SET_BIT(axe,AxeMap);
				}
				_SynEvt->DeleteAll();

				switch(periph->GetType())
				{
				case EPTmouse:
					if (TEST_BIT(AXE_X,AxeMap) || TEST_BIT(AXE_Y,AxeMap))
					{
						double x=0,y=0;
						if (periph->GetAxes() && TEST_BIT(AXE_X,periph->GetAxes()->AxeMap)) x=periph->GetAxes()->AxeValues[AXE_X].GetValue();
						if (periph->GetAxes() && TEST_BIT(AXE_Y,periph->GetAxes()->AxeMap)) y=periph->GetAxes()->AxeValues[AXE_Y].GetValue();

						OpenUtility::MutexUnlock(_mutexEnd);
						OnMouseMove(id,x,y);
						OpenUtility::MutexLock(_mutexEnd);

						UNSET_BIT(AXE_X,AxeMap);
						UNSET_BIT(AXE_Y,AxeMap);
					}
					break;

				case EPT6axis:
					if (TEST_BIT(AXE_X,AxeMap) || TEST_BIT(AXE_Y,AxeMap) || TEST_BIT(AXE_Z,AxeMap) || TEST_BIT(AXE_RX,AxeMap) || TEST_BIT(AXE_RY,AxeMap) || TEST_BIT(AXE_RZ,AxeMap))
					{
						double x=0,y=0,z=0,rx=0,ry=0,rz=0;
						if (periph->GetAxes())
						{
							if (TEST_BIT(AXE_X,periph->GetAxes()->AxeMap)) x=periph->GetAxes()->AxeValues[AXE_X].GetValue();
							if (TEST_BIT(AXE_Y,periph->GetAxes()->AxeMap)) y=periph->GetAxes()->AxeValues[AXE_Y].GetValue();
							if (TEST_BIT(AXE_Z,periph->GetAxes()->AxeMap)) z=periph->GetAxes()->AxeValues[AXE_Z].GetValue();
							if (TEST_BIT(AXE_RX,periph->GetAxes()->AxeMap)) rx=periph->GetAxes()->AxeValues[AXE_RX].GetValue();
							if (TEST_BIT(AXE_RY,periph->GetAxes()->AxeMap)) ry=periph->GetAxes()->AxeValues[AXE_RY].GetValue();
							if (TEST_BIT(AXE_RZ,periph->GetAxes()->AxeMap)) rz=periph->GetAxes()->AxeValues[AXE_RZ].GetValue();
						}

						OpenUtility::MutexUnlock(_mutexEnd);
						On6axisChange(id,x,y,z,rx,ry,rz);
						OpenUtility::MutexLock(_mutexEnd);

						UNSET_BIT(AXE_X,AxeMap);
						UNSET_BIT(AXE_Y,AxeMap);
						UNSET_BIT(AXE_Z,AxeMap);
						UNSET_BIT(AXE_RX,AxeMap);
						UNSET_BIT(AXE_RY,AxeMap);
						UNSET_BIT(AXE_RZ,AxeMap);
					}
					break;

				default:
					break;
				}
				for (unsigned int j=0;j<AXE_UNKNOWN;j++)
				{
					if (TEST_BIT(j,AxeMap))
					{
						switch(j)
						{
						case AXE_WHEEL:
							UNSET_BIT(AXE_HWHEEL,AxeMap);
						case AXE_HWHEEL:
							{
								double x=0,y=0;
								if (periph->GetAxes())
								{
									if (TEST_BIT(AXE_WHEEL,periph->GetAxes()->AxeMap)) x=periph->GetAxes()->AxeValues[AXE_WHEEL].GetValue();
									if (TEST_BIT(AXE_HWHEEL,periph->GetAxes()->AxeMap)) y=periph->GetAxes()->AxeValues[AXE_HWHEEL].GetValue();

									OpenUtility::MutexUnlock(_mutexEnd);
									OnWheelChange(id,x,y);
									OpenUtility::MutexLock(_mutexEnd);
								}
							}
							break;

						case AXE_HAT0X:
						case AXE_HAT1X:
						case AXE_HAT2X:
						case AXE_HAT3X:
							UNSET_BIT(j+1,AxeMap);
						case AXE_HAT0Y:
						case AXE_HAT1Y:
						case AXE_HAT2Y:
						case AXE_HAT3Y:
							{
								double x=0,y=0;
								if (periph->GetAxes())
								{
									if (TEST_BIT((j-AXE_HAT0X)/2*2+AXE_HAT0X,periph->GetAxes()->AxeMap)) x=periph->GetAxes()->AxeValues[(j-AXE_HAT0X)/2*2+AXE_HAT0X].GetValue();
									if (TEST_BIT((j-AXE_HAT0X)/2*2+AXE_HAT0X+1,periph->GetAxes()->AxeMap)) y=periph->GetAxes()->AxeValues[(j-AXE_HAT0X)/2*2+AXE_HAT0X+1].GetValue();

									OpenUtility::MutexUnlock(_mutexEnd);
									OnHatChange(id,(j-AXE_HAT0X)/2,x,y);
									OpenUtility::MutexLock(_mutexEnd);
								}
							}
							break;

						case AXE_TILT_X:
							UNSET_BIT(AXE_TILT_Y,AxeMap);
						case AXE_TILT_Y:
							{
								double x=0,y=0;
								if (periph->GetAxes())
								{
									if (TEST_BIT(AXE_TILT_X,periph->GetAxes()->AxeMap)) x=periph->GetAxes()->AxeValues[AXE_TILT_X].GetValue();
									if (TEST_BIT(AXE_TILT_Y,periph->GetAxes()->AxeMap)) y=periph->GetAxes()->AxeValues[AXE_TILT_Y].GetValue();

									OpenUtility::MutexUnlock(_mutexEnd);
									OnTiltChange(id,x,y);
									OpenUtility::MutexLock(_mutexEnd);
								}
							}
							break;

						default:
							{
								double val=0;
								if (TEST_BIT(j,periph->GetAxes()->AxeMap)) val=periph->GetAxes()->AxeValues[j].GetValue();

								OpenUtility::MutexUnlock(_mutexEnd);
								OnAxeChange(id,GetAxeFromInt(j),val);
								OpenUtility::MutexLock(_mutexEnd);
							}
						}
					}
					if (periph->GetAxes() && TEST_BIT(j,periph->GetAxes()->AxeMap) && periph->GetAxes()->AxeValues[j].IsSetToZero)
						periph->GetAxes()->AxeValues[j].SetValue(0);
				}
			}
			break;

		case EV_KEY:
			if (!periph->HasKeys()) break;
			periph->SetNewEvent(ev[i].value,ev[i].code,EETkey);
			break;

		case EV_REL:
			if (!periph->HasAxes()) break;
			periph->SetNewEvent(ev[i].code,ev[i].value,EETrel);
			break;

		case EV_ABS:
//if (ev[i].value>=ABS_MT_SLOT && ev[i].value<=ABS_MT_DISTANCE)
//	printf("EV_ABS_MT: %x\n",ev[i].value);
			if (!periph->HasAxes()) break;
			periph->SetNewEvent(ev[i].code,ev[i].value,EETabs);
			break;

		case EV_MSC:
		case EV_SW:
		case EV_LED:
		case EV_SND:
		case EV_REP:
		case EV_FF:
		case EV_PWR:
			break;
		}
	}
}

void GlWindow::CloseEvents()
{
	OpenUtility::MutexLock(_mutexEnd);

	ListPeriph.DeleteAll();
	SPeripheral::InitPeripheral();
	write(_fdEnd[OUT],"",1);

	OpenUtility::MutexUnlock(_mutexEnd);
}

void GlWindow::MainLoop()
{
	Init();
	GL_CHECK();
	while (!_request_end)
	{
		PreRender();
		Render();

		glFlush();
		glFinish();

		GL_CHECK();
		eglSwapBuffers(Display, Surface);
	}
	Uninit();
	_CloseWindow();
}

void GlWindow::SetAxeLimit(unsigned int id,EPeriphAxe axe,int min,int max)
{
	SPeripheral *periph;

	OpenUtility::MutexLock(_mutexEnd);
	if ((periph=SPeripheral::GetPeripheral(id))!=NULL)
	{
		if (periph->GetAxes() && TEST_BIT(axe,periph->GetAxes()->AxeMap))
			periph->GetAxes()->AxeValues[axe].SetMargin(min,max);
	}
	OpenUtility::MutexUnlock(_mutexEnd);
}

void GlWindow::SetAxeRemap(unsigned int id,EPeriphAxe axe,int min,int max)
{
	SPeripheral *periph;

	OpenUtility::MutexLock(_mutexEnd);
	if ((periph=SPeripheral::GetPeripheral(id))!=NULL)
	{
		if (periph->GetAxes() && TEST_BIT(axe,periph->GetAxes()->AxeMap))
			periph->GetAxes()->AxeValues[axe].SetRemap(min,max);
	}
	OpenUtility::MutexUnlock(_mutexEnd);
}

void GlWindow::UnsetAxeRemap(unsigned int id,EPeriphAxe axe)
{
	SPeripheral *periph;

	OpenUtility::MutexLock(_mutexEnd);
	if ((periph=SPeripheral::GetPeripheral(id))!=NULL)
	{
		if (periph->GetAxes() && TEST_BIT(axe,periph->GetAxes()->AxeMap))
			periph->GetAxes()->AxeValues[axe].UnsetRemap();
	}
	OpenUtility::MutexUnlock(_mutexEnd);
}

void GlWindow::SetMousePos(unsigned int id,int x,int y)
{
	SPeripheral *periph;

	OpenUtility::MutexLock(_mutexEnd);
	if ((periph=SPeripheral::GetPeripheral(id))!=NULL)
	{
		if (periph->GetAxes())
		{
			if (TEST_BIT(AXE_X,periph->GetAxes()->AxeMap))
				periph->GetAxes()->AxeValues[AXE_X].SetValue(x);
			if (TEST_BIT(AXE_Y,periph->GetAxes()->AxeMap))
				periph->GetAxes()->AxeValues[AXE_Y].SetValue(y);
		}
	}
	OpenUtility::MutexUnlock(_mutexEnd);
}

void GlWindow::Set6AxisPos(unsigned int id,int x,int y,int z,int rx,int ry,int rz)
{
	SPeripheral *periph;

	OpenUtility::MutexLock(_mutexEnd);
	if ((periph=SPeripheral::GetPeripheral(id))!=NULL)
	{
		if (periph->GetAxes())
		{
			if (TEST_BIT(AXE_X,periph->GetAxes()->AxeMap))
				periph->GetAxes()->AxeValues[AXE_X].SetValue(x);
			if (TEST_BIT(AXE_Y,periph->GetAxes()->AxeMap))
				periph->GetAxes()->AxeValues[AXE_Y].SetValue(y);
			if (TEST_BIT(AXE_Z,periph->GetAxes()->AxeMap))
				periph->GetAxes()->AxeValues[AXE_Z].SetValue(z);
			if (TEST_BIT(AXE_RX,periph->GetAxes()->AxeMap))
				periph->GetAxes()->AxeValues[AXE_RX].SetValue(rx);
			if (TEST_BIT(AXE_RY,periph->GetAxes()->AxeMap))
				periph->GetAxes()->AxeValues[AXE_RY].SetValue(ry);
			if (TEST_BIT(AXE_RZ,periph->GetAxes()->AxeMap))
				periph->GetAxes()->AxeValues[AXE_RZ].SetValue(ry);
		}
	}
	OpenUtility::MutexUnlock(_mutexEnd);
}

void GlWindow::SetMouseLimitToScreen(unsigned int id,bool enable)
{
	OpenUtility::MutexLock(_mutexEnd);
	SetAxeLimit(id,AXE_X,0,ScrWidth);
	SetAxeLimit(id,AXE_Y,0,ScrHeight);
	OpenUtility::MutexUnlock(_mutexEnd);
}

const char* GlWindow::GetPeripheralTypeName(EPeriphType type)
{
	switch(type)
	{
	case EPTkeyboard:return("Keyboard");break;
	case EPTmouse:return("Mouse");break;
	case EPTjoystick:return("Joystick/Gamepad");break;
	case EPT6axis:return("6 axis");break;
	default:return("Unknown");
	}
}

GlWindow::EPeriphAxe GlWindow::GetAxeBit(int bit,bool realAxe)
{
	GlWindow::EPeriphAxe val=AXE_UNKNOWN;

	if (realAxe)
	{
		switch(bit)
		{
		case REL_X:val=AXE_X;break;
		case REL_Y:val=AXE_Y;break;
		case REL_Z:val=AXE_Z;break;
		case REL_RX:val=AXE_RX;break;
		case REL_RY:val=AXE_RY;break;
		case REL_RZ:val=AXE_RZ;break;
		case REL_HWHEEL:val=AXE_HWHEEL;break;
		case REL_DIAL:val=AXE_DIAL;break;
		case REL_WHEEL:val=AXE_WHEEL;break;
		case REL_MISC:val=AXE_MISC;break;
		}
	}
	else
	{
		switch(bit)
		{
		case ABS_X:val=AXE_X;break;
		case ABS_Y:val=AXE_Y;break;
		case ABS_Z:val=AXE_Z;break;
		case ABS_RX:val=AXE_RX;break;
		case ABS_RY:val=AXE_RY;break;
		case ABS_RZ:val=AXE_RZ;break;
		case ABS_THROTTLE:val=AXE_THROTTLE;break;
		case ABS_RUDDER:val=AXE_RUDDER;break;
		case ABS_WHEEL:val=AXE_WHEEL;break;
		case ABS_GAS:val=AXE_GAS;break;
		case ABS_BRAKE:val=AXE_BRAKE;break;
		case ABS_HAT0X:val=AXE_HAT0X;break;
		case ABS_HAT0Y:val=AXE_HAT0Y;break;
		case ABS_HAT1X:val=AXE_HAT1X;break;
		case ABS_HAT1Y:val=AXE_HAT1Y;break;
		case ABS_HAT2X:val=AXE_HAT2X;break;
		case ABS_HAT2Y:val=AXE_HAT2Y;break;
		case ABS_HAT3X:val=AXE_HAT3X;break;
		case ABS_HAT3Y:val=AXE_HAT3Y;break;
		case ABS_PRESSURE:val=AXE_PRESSURE;break;
		case ABS_DISTANCE:val=AXE_DISTANCE;break;
		case ABS_TILT_X:val=AXE_TILT_X;break;
		case ABS_TILT_Y:val=AXE_TILT_Y;break;
		case ABS_TOOL_WIDTH:val=AXE_TOOL_WIDTH;break;
		case ABS_VOLUME:val=AXE_VOLUME;break;
		case ABS_MISC:val=AXE_MISC;break;
		}
	}
	return(val);
}

GlWindow::EPeriphAxe GlWindow::GetAxeFromInt(int val)
{
	switch(val)
	{
	case AXE_X:return(AXE_X);
	case AXE_Y:return(AXE_Y);
	case AXE_Z:return(AXE_Z);
	case AXE_RX:return(AXE_RX);
	case AXE_RY:return(AXE_RY);
	case AXE_RZ:return(AXE_RZ);
	case AXE_WHEEL:return(AXE_WHEEL);
	case AXE_HWHEEL:return(AXE_HWHEEL);
	case AXE_DIAL:return(AXE_DIAL);
	case AXE_THROTTLE:return(AXE_THROTTLE);
	case AXE_RUDDER:return(AXE_RUDDER);
	case AXE_GAS:return(AXE_GAS);
	case AXE_BRAKE:return(AXE_BRAKE);
	case AXE_HAT0X:return(AXE_HAT0X);
	case AXE_HAT0Y:return(AXE_HAT0Y);
	case AXE_HAT1X:return(AXE_HAT1X);
	case AXE_HAT1Y:return(AXE_HAT1Y);
	case AXE_HAT2X:return(AXE_HAT2X);
	case AXE_HAT2Y:return(AXE_HAT2Y);
	case AXE_HAT3X:return(AXE_HAT3X);
	case AXE_HAT3Y:return(AXE_HAT3Y);
	case AXE_PRESSURE:return(AXE_PRESSURE);
	case AXE_DISTANCE:return(AXE_DISTANCE);
	case AXE_TILT_X:return(AXE_TILT_X);
	case AXE_TILT_Y:return(AXE_TILT_Y);
	case AXE_TOOL_WIDTH:return(AXE_TOOL_WIDTH);
	case AXE_VOLUME:return(AXE_VOLUME);
	case AXE_MISC:return(AXE_MISC);
	default:return(AXE_UNKNOWN);
	}
}

const char* GlWindow::GetAxeName(GlWindow::EPeriphAxe axe)
{
	switch(axe)
	{
	case AXE_X:return("X");
	case AXE_Y:return("Y");
	case AXE_Z:return("Z");
	case AXE_RX:return("RX");
	case AXE_RY:return("RY");
	case AXE_RZ:return("RZ");
	case AXE_WHEEL:return("WHEEL");
	case AXE_HWHEEL:return("HWHEEL");
	case AXE_DIAL:return("DIAL");
	case AXE_THROTTLE:return("THROTTLE");
	case AXE_RUDDER:return("RUDDER");
	case AXE_GAS:return("GAS");
	case AXE_BRAKE:return("BRAKE");
	case AXE_HAT0X:return("HAT0X");
	case AXE_HAT0Y:return("HAT0Y");
	case AXE_HAT1X:return("HAT1X");
	case AXE_HAT1Y:return("HAT1Y");
	case AXE_HAT2X:return("HAT2X");
	case AXE_HAT2Y:return("HAT2Y");
	case AXE_HAT3X:return("HAT3X");
	case AXE_HAT3Y:return("HAT3Y");
	case AXE_PRESSURE:return("PRESSURE");
	case AXE_DISTANCE:return("DISTANCE");
	case AXE_TILT_X:return("TILT_X");
	case AXE_TILT_Y:return("TILT_Y");
	case AXE_TOOL_WIDTH:return("TOOL_WIDTH");
	case AXE_VOLUME:return("VOLUME");
	case AXE_MISC:return("MISC");
	default:return("UNKNOWN");
	}
}

// ----- SPeripheral
// -------------------------------------

OpenUtility::CTable<unsigned int> GlWindow::SPeripheral::_IdFreePeriph(5);
OpenUtility::CTable<GlWindow::SPeripheral*> GlWindow::SPeripheral::_TabPeriph(5);
OpenUtility::CListe<OpenUtility::CStream> GlWindow::SPeripheral::_ListPeriphFile;

void GlWindow::SPeripheral::InitPeripheral()
{
	_IdFreePeriph.DeleteAll();
	_TabPeriph.DeleteAll();
	_ListPeriphFile.DeleteAll();
}

GlWindow::SPeripheral* GlWindow::SPeripheral::GenPeripheral(const char *file)
{
	char name[256]="Unknown";
	bool found;
	unsigned int i;

	// Check if we have not already load the peripherical
	found=false;
	i=0;
	while (!found && (i<_ListPeriphFile.GetSize()))
	{
		if ((*_ListPeriphFile[i])==file) found=true;
		i++;
	}
	if (found) return(NULL);

	// Get informations about the peripherical
	int fd=-1;
	if ((fd=open(file,O_RDONLY))<0) return(NULL);

	if (ioctl(fd,EVIOCGNAME(sizeof(name)),name)<0)
	{
		close(fd);
		return(NULL);
	}

	OpenUtility::CListe<OpenUtility::CStream>::CListeIterator pos;
	_ListPeriphFile.Add(new OpenUtility::CStream(file),&pos);
	SPeripheral *periph=new SPeripheral(fd,name,pos);
	if (_IdFreePeriph.GetSize())
	{
		periph->id=_IdFreePeriph[_IdFreePeriph.GetSize()-1];
		_TabPeriph[periph->id]=periph;
		_IdFreePeriph.Delete(1);
	}
	else
	{
		periph->id=_TabPeriph.GetSize();
		_TabPeriph.Add(periph);
	}

	return(periph);
}

GlWindow::SPeripheral* GlWindow::SPeripheral::GetPeripheral(unsigned int id)
{
	if (_TabPeriph.GetSize()<id) return(_TabPeriph[id]);
	return(NULL);
}

GlWindow::SPeripheral::SPeripheral(int fd,const char *name,OpenUtility::CListe<OpenUtility::CStream>::CListeIterator &pos) :
	FilePos(pos)
	,fdEvent(fd)
	,Type(EPTunknown)
	,sKey(NULL)
	,sAxe(NULL)
{
	long bitmask[NLONGS(EV_CNT)];
	long bit;

	strcpy(Name,name);

	memset(bitmask,0,sizeof(bitmask));
	if (ioctl(fd,EVIOCGBIT(0,sizeof(bitmask)),bitmask)<0) return;

	for (bit=0;bit<KEY_CNT;bit++)
	{
		if (TEST_BIT(bit,bitmask))
		{
			switch (bit)
			{
			case EV_KEY:
				{
					long keymask[NLONGS(KEY_CNT)];
					long bitK;
					int codes[2];
					int keybNb;

					keybNb=0;
					sKey=new SKey();
					if (ioctl(fd,EVIOCGBIT(EV_KEY,sizeof(keymask)),keymask)<0) break;
					for (bitK=0;bitK<KEY_CNT;bitK++)
					{
						if (TEST_BIT(bitK,keymask))
						{
							// Try to detect peripheral type
							if (Type==EPTunknown)
							{
								if ((bitK>=KEY_1) && (bitK<=KEY_CAPSLOCK))
								{
									keybNb++;
									if (keybNb>5) Type=EPTkeyboard;
								}
								else if (bitK==BTN_MOUSE) Type=EPTmouse;
								else if ((bitK==BTN_GAMEPAD) || (bitK==BTN_JOYSTICK)) Type=EPTjoystick;
							}

							// Try to get corresponding keycode
							codes[0]=bitK;
							if (ioctl(fd,EVIOCGKEYCODE,codes)<0) sKey->KeyMap[bitK]=bitK;
							else sKey->KeyMap[codes[0]]=codes[1];
						}
					}
				}
				break;

			case EV_REL:
				{
					long bitK;
					long axemask[NLONGS(REL_CNT)];

					if (!sAxe) sAxe=new SAxe();
					if (ioctl(fd,EVIOCGBIT(EV_REL,sizeof(axemask)),axemask)<0) break;
					for (bitK=0;bitK<REL_CNT;bitK++)
					{
						if (TEST_BIT(bitK,axemask))
						{
							EPeriphAxe axe;
							if (AXE_UNKNOWN!=(axe=GetAxeBit(bitK,true)))
							{
								SET_BIT(axe,sAxe->AxeMap);
							}
						}
					}
					if ((Type==EPTunknown) || (Type==EPTmouse))
					{
						if (TEST_BIT(AXE_X,sAxe->AxeMap) &&
							TEST_BIT(AXE_Y,sAxe->AxeMap) &&
							TEST_BIT(AXE_Z,sAxe->AxeMap) &&
							TEST_BIT(AXE_RX,sAxe->AxeMap) &&
							TEST_BIT(AXE_RY,sAxe->AxeMap) &&
							TEST_BIT(AXE_RZ,sAxe->AxeMap))
						{
							for (int axe=AXE_X;axe<=AXE_RZ;axe++)
								sAxe->AxeValues[axe].IsSetToZero=true;
							Type=EPT6axis;
						}
					}
				}
				break;

			case EV_ABS:
				{
					long bitK;
					long axemask[NLONGS(ABS_CNT)];
					struct input_absinfo abs_feat;

					if (!sAxe) sAxe=new SAxe();
					if (ioctl(fd,EVIOCGBIT(EV_ABS,sizeof(axemask)),axemask)<0) break;
					for (bitK=0;bitK<ABS_CNT;bitK++)
					{
						if (TEST_BIT(bitK,axemask))
						{
							EPeriphAxe axe;
							if (AXE_UNKNOWN!=(axe=GetAxeBit(bitK,false)))
							{
								SET_BIT(axe,sAxe->AxeMap);
								if (ioctl(fd,EVIOCGABS(bitK),&abs_feat)>=0)
								{
									sAxe->AxeValues[axe].SetMargin(abs_feat.minimum,abs_feat.maximum);
									sAxe->AxeValues[axe].SetValue(abs_feat.value);
								}
							}
						}
					}
					if (Type==EPTunknown)
					{
						if (TEST_BIT(AXE_X,sAxe->AxeMap) &&
							TEST_BIT(AXE_Y,sAxe->AxeMap) &&
							TEST_BIT(AXE_Z,sAxe->AxeMap) &&
							TEST_BIT(AXE_RX,sAxe->AxeMap) &&
							TEST_BIT(AXE_RY,sAxe->AxeMap) &&
							TEST_BIT(AXE_RZ,sAxe->AxeMap)) Type=EPT6axis;
					}
					break;
				}
			}
		}
	}
}

void GlWindow::SPeripheral::SetNewEvent(int type,int value,EEventType etype)
{
	SEventValue evt;

	switch(etype)
	{
	case EETkey:
		if (HasKeys())
		{
			evt.type=type;
			evt.value=value;
			sKey->_SynKey.Add(evt);
		}
		break;

	case EETrel:
	case EETabs:
		if (HasAxes())
		{
			evt.type=type;
			evt.value=value;
			evt.data=etype;
			sAxe->_SynAxe.Add(evt);
		}
		break;
	}
}

GlWindow::SPeripheral::~SPeripheral()
{
	close(fdEvent);
	_ListPeriphFile.Delete(FilePos);
	_IdFreePeriph.Add(id);
	_TabPeriph[id]=NULL;
	delete sKey;
	delete sAxe;
}

void GlWindow::SAxeParam::CheckValue()
{
	if (min!=max)
	{
		if (value<min) value=min;
		else if (value>max) value=max;
	}
}

double GlWindow::SAxeParam::GetValue()
{
	if (remap) return((value-min)/(max-min)*(maxR-minR)+minR);
	return(value);
}
