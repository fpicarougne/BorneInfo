//coding: utf-8
#ifndef _Screen_h
	#define _Screen_h

#include <Utility/Stream/Stream.h>
#include <Utility/3D/CTextureQuad.h>
#include <Utility/3D/C3DText.h>
#include <Utility/3D/CQuad.h>
#include <Utility/Math/CBezierCurve.h>
#include <Template/CVec4.h>
#include "../RenderingObject.h"

class IScreen : public IRenderingObject
{
public:
	IScreen(IRenderingObjectComm *obj,const char *name,OpenUtility::ITextureQuad *image=NULL);
	IScreen(const IScreen &obj);
	virtual ~IScreen();
	IScreen& operator=(const IScreen &obj);
	inline const OpenUtility::CStream& GetName() const {return(Name);}
	inline const OpenUtility::ITextureQuad* GetLogo() const {return(Image);}

protected:
	OpenUtility::CStream Name;
	OpenUtility::ITextureQuad *Image;
};

class CScreenBox : public IRenderingObject
{
private:
	struct SShaders
	{
		SShaders() :
			ShaderVertex(OpenUtility::CShaderFile::EVertexShader,"vertex shader"),
			ShaderFragment(OpenUtility::CShaderFile::EFragmentShader,"fragment shader")
		{
			RenderingShader.AddShaderFile(&ShaderVertex);
			RenderingShader.AddShaderFile(&ShaderFragment);
		}
		OpenUtility::CShaderProgram RenderingShader;
		OpenUtility::CShaderFile ShaderVertex,ShaderFragment;
	};

	enum EState
	{
		ESDisplayMenu,
		ESStartAnimTouch,
		ESAnimTouch,
		ESDisplayTouch,
		ESEndAnimTouch,
		ESStartAnimUntouch,
		ESAnimUntouch,
		ESStartAnimFullScreen,
		ESAnimFullScreen,
		ESDisplayFullScreen
	};

	struct SAnim
	{
		SAnim(unsigned int dim) : AnimCoord(dim) {}
		unsigned long long AnimStartTime;
		OpenUtility::CBezierCurve AnimCoord;
		unsigned long long Duration;
	};

public:
	CScreenBox(IRenderingObjectComm *pClass,unsigned int id,IScreen *obj,const OpenUtility::CTexture *backTex=NULL,double w=1.0,double h=1.0,double posX=0,double posY=0,double texX=0.0,double texY=0.0,double texW=1.0,double texH=1.0);
	~CScreenBox();
	void Init();
	void UnInit();
	bool PreRender();
	void Render();
	void SetMatrix(OpenUtility::CMat4x4<float> &p,OpenUtility::CMat4x4<float> &mv,OpenUtility::CMat4x4<float> &n);
	inline void SetFrontParams(double x,double y,double z,double w=0,double h=0) {FrontX=x;FrontY=y;FrontZ=z;DimW=w ? w : DimX[0];DimH=h ? h : DimY[0];}
	inline void UpdateBackTexture(const OpenUtility::CTexture *backTex) {BackTex=backTex;}
	inline void SetTexParams(double x,double y,double w,double h) {TexX[1]=x;TexY[1]=y;TexW[1]=w;TexH[1]=h;}
	bool UseGlobalShaderProgram() {return(false);}
	bool TestImpact(double x,double y);
	void EventTouch();
	void EventUntouch();
    void EventClick(unsigned long long timeUnit=0);
	inline IScreen* GetScreenClient() {return(Screen);}

private:
	void UpdateTexData();
	bool UdateAnimCoord(unsigned long long timeUnit);

private:
	unsigned int Id;
    double PosX[2],PosY[2],PosZ[2],RotX[2],RotY[2],DimX[2],DimY[2];
    double TexX[2],TexY[2],TexW[2],TexH[2];
	IScreen *Screen;
	SShaders *Shaders;
	const OpenUtility::CTexture *BackTex;
	OpenUtility::CMat4x4<float> MVPmatrix,MVPmatrixLogo,MVPmatrixTitle;
	OpenUtility::CQuad *Background;
    OpenUtility::CVec4<float> VBackground[2];
	GLuint VBObufferBackground;
	OpenUtility::CFontLoader *Font;
	OpenUtility::C3DText *Title;
	bool ForceRender;
	EState State;
	SAnim Animation;
	double FrontX,FrontY,FrontZ,DimW,DimH;
};

#endif
