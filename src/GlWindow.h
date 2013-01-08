//coding: utf-8
#ifndef _GlWindow_h
	#define _GlWindow_h

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <Utility/Parallelism/MultiThread.h>
#include <Utility/Stream/Stream.h>
#include <iostream>
#include <Template/CTable.h>
#include <Template/CListe.h>
#include <linux/input.h>

#ifndef LONG_BITS
	#define LONG_BITS (sizeof(long) * 8)
#endif
#ifndef NLONGS
	#define NLONGS(x) (((x) - 1) / LONG_BITS + 1)
#endif
#ifndef TEST_BIT
	#define TEST_BIT(bit, array) (array[(bit) / LONG_BITS]) & (1L << ((bit) % LONG_BITS))
#endif
#ifndef SET_BIT
	#define SET_BIT(bit, array) (array[(bit) / LONG_BITS]) |= (1L << ((bit) % LONG_BITS))
#endif
#ifndef UNSET_BIT
	#define UNSET_BIT(bit, array) (array[(bit) / LONG_BITS]) &= ~(1L << ((bit) % LONG_BITS))
#endif

#define GL_CHECK() GlWindow::CheckGl(__FILE__,__LINE__)

class GlWindow
{
public:
	enum EPeriphType
	{
		EPTkeyboard,
		EPTmouse,
		EPTjoystick,
		EPT6axis,
		EPTunknown
	};

	enum EPeriphAxe
	{
		AXE_X=0,
		AXE_Y,
		AXE_Z,
		AXE_RX,
		AXE_RY,
		AXE_RZ,
		AXE_WHEEL,
		AXE_HWHEEL,
		AXE_DIAL,
		AXE_THROTTLE,
		AXE_RUDDER,
		AXE_GAS,
		AXE_BRAKE,
		AXE_HAT0X,
		AXE_HAT0Y,
		AXE_HAT1X,
		AXE_HAT1Y,
		AXE_HAT2X,
		AXE_HAT2Y,
		AXE_HAT3X,
		AXE_HAT3Y,
		AXE_PRESSURE,
		AXE_DISTANCE,
		AXE_TILT_X,
		AXE_TILT_Y,
		AXE_TOOL_WIDTH,
		AXE_VOLUME,
		AXE_MISC,
		AXE_UNKNOWN
	};

private:
	enum EEventType
	{
		EETkey,
		EETrel,
		EETabs
	};

	struct SEventValue
	{
		int type;
		int value;
		int data;
	};

	struct SKey
	{
		SKey() {bzero(KeyMap,KEY_CNT*sizeof(int));}
		int KeyMap[KEY_CNT];
		OpenUtility::CTable<SEventValue> _SynKey;
	};

	class SAxeParam
	{
	public:
		SAxeParam(int _min=0,int _max=0) : IsSetToZero(false),min(_min),max(_max),remap(false),value(_min) {if (min>max){int t=min;min=max;max=t;}}
		inline double SetMargin(double _min=0,double _max=0) {min=_min;max=_max;if (min>max){int t=min;min=max;max=t;}return(GetValue());}
		inline double SetRemap(double _min=0,double _max=1) {remap=true;minR=_min;maxR=_max;if (minR>maxR){int t=minR;minR=maxR;maxR=t;}return(GetValue());}
		inline void UnsetRemap() {remap=false;}
		inline double SetValue(int _value) {value=_value;CheckValue();return(GetValue());}
		inline double SetRelativeValue(int _value) {value+=_value;CheckValue();return(GetValue());}
		double GetValue();

	private:
		void CheckValue();

	public:
		bool IsSetToZero;

	private:
		int min;
		int max;
		// for remap
		bool remap;
		double minR;
		double maxR;
		int value;
	};

	struct SAxe
	{
		SAxe() {bzero(AxeMap,NLONGS(AXE_UNKNOWN+1)*sizeof(long));}
		long AxeMap[NLONGS(AXE_UNKNOWN+1)];
		SAxeParam AxeValues[AXE_UNKNOWN+1];
		OpenUtility::CTable<SEventValue> _SynAxe;
	};

	class SPeripheral
	{
	public:
		static void InitPeripheral();
		static SPeripheral* GenPeripheral(const char *file);
		static SPeripheral* GetPeripheral(unsigned int id);
		~SPeripheral();
		inline unsigned int GetId() {return(id);}
		inline int GetFdEvent() {return(fdEvent);}
		inline const char* GetName() const {return(Name);}
		inline EPeriphType GetType() const {return(Type);}
		inline bool HasKeys() {return(sKey!=NULL);}
		inline bool HasAxes() {return(sAxe!=NULL);}
		inline SKey* GetKeys() {return(sKey);}
		inline SAxe* GetAxes() {return(sAxe);}
		void SetNewEvent(int type,int value,EEventType etype);

	private:
		SPeripheral(int fd,const char *name,OpenUtility::CListe<OpenUtility::CStream>::CListeIterator &pos);

	private:
		static OpenUtility::CTable<unsigned int> _IdFreePeriph;
		static OpenUtility::CTable<SPeripheral*> _TabPeriph;
		static OpenUtility::CListe<OpenUtility::CStream> _ListPeriphFile;
		OpenUtility::CListe<OpenUtility::CStream>::CListeIterator FilePos;
		int fdEvent;
		unsigned int id;
		char Name[256];
		EPeriphType Type;
		SKey *sKey;
		SAxe *sAxe;
	};

public:
	GlWindow();
	virtual ~GlWindow();
	static GlWindow::EPeriphAxe GetAxeBit(int bit,bool realAxe);
	static GlWindow::EPeriphAxe GetAxeFromInt(int val);
	static const char* GetAxeName(GlWindow::EPeriphAxe axe);
	static void CheckGl(const char *file,int line);

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
	virtual void Uninit() {}
	virtual void PreRender() {}
	virtual void Render() {}
	// Event catcher
	virtual void OnPeripheralAdd(unsigned int id,const char *name,GlWindow::EPeriphType type) {}
	virtual void OnPeripheralRemove(unsigned int id,const char *name) {}
	virtual void OnKeyDown(unsigned int id,int keyCode) {}
	virtual void OnKeyUp(unsigned int id,int keyCode) {}
	virtual void OnMouseMove(unsigned int id,double x,double y) {}
	virtual void On6axisChange(unsigned int id,double x,double y,double z,double rx,double ry,double rz) {}
	virtual void OnWheelChange(unsigned int id,double x,double y) {}
	virtual void OnHatChange(unsigned int id,int sub,double x,double y) {}
	virtual void OnTiltChange(unsigned int id,double x,double y) {}
	virtual void OnAxeChange(unsigned int id,EPeriphAxe axe,double val) {}
	virtual void OnButtonDown(unsigned int id,int b) {}
	virtual void OnButtonUp(unsigned int id,int b) {}
	virtual void OnMouseButtonDown(unsigned int id,int b,double x,double y) {}
	virtual void OnMouseButtonUp(unsigned int id,int b,double x,double y) {}
	virtual void OnJoystickButtonDown(unsigned int id,int b) {}
	virtual void OnJoystickButtonUp(unsigned int id,int b) {}
	virtual void OnGamepadButtonDown(unsigned int id,int b) {}
	virtual void OnGamepadButtonUp(unsigned int id,int b) {}
	// Peripheral management
	void SetAxeLimit(unsigned int id,EPeriphAxe axe,int min,int max);
	void SetAxeRemap(unsigned int id,EPeriphAxe axe,int min,int max);
	void UnsetAxeRemap(unsigned int id,EPeriphAxe axe);
	void SetMousePos(unsigned int id,int x,int y);
	void Set6AxisPos(unsigned int id,int x,int y,int z,int rx,int ry,int rz);
	void SetMouseLimitToScreen(unsigned int id,bool enable=true);
	// Tools
	static const char* GetPeripheralTypeName(EPeriphType type);

private:
	GlWindow(const GlWindow &obj) {}
	GlWindow* operator=(const GlWindow &obj) {return(NULL);}
	bool CreateOpenGlContext();
	void DestroyOpenGlContext();
	TH_CALL_BACK_PROTO(EventListenerCB,pParam);
	void EventListener();
	bool AddPeripheral(char *file);
	bool RemovePeripheral(OpenUtility::CListe<SPeripheral>::CListeIterator &pos);
	void ReadEvent(SPeripheral *periph);
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
	MUTEX _mutexMouse;
	OpenUtility::CListe<SPeripheral> ListPeriph;
};

#endif
