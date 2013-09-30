//coding: utf-8
#ifndef _Client_h
	#define _Client_h

#include "GlWindow.h"
#include "OpenUtility/Utility/3D/CTextureMultiQuad.h"
#include <time.h>
#include <Utility/3D/CShaderProgram.h>
#include <Utility/3D/CTexture.h>
#include <Utility/3D/CTextureQuad.h>
#include <Utility/3D/CTextureMultiQuad.h>
#include <Utility/3D/C3DText.h>
#include <Utility/3D/CQuad.h>
#include <Template/CVector.h>
#include "Mouse.h"
#include "Screens/TaskBar.h"
#include "Screens/Screen.h"

class Client : protected GlWindow, public IRenderingObjectComm
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

	struct SVertex
	{
		GLfloat position[3];
		GLfloat normal[3];
	};

	struct STexture
	{
		GLfloat coord[2];
	};

	class SFBO
	{
	public:
		SFBO(unsigned int w,unsigned int h);
		~SFBO();
		OpenUtility::CTexture* const GetTexture() const {return(Image);}
		void Activate();
		void Unactivate();

	private:
		GLuint idFBO;
		OpenUtility::CTexture *Image;
		GLuint idDepthRB;
	};

public:
	Client();
	virtual ~Client();
	void Start();
	inline unsigned long long GetTimeUnit() {return(GlWindow::GetTimeUnit());}
	inline const OpenUtility::SFrustum& GetFrustum() const {return(Frustum);}
	inline const OpenUtility::SPivotd& GetCameraPos() const {return(CameraPos);}
	inline const OpenUtility::CShaderProgram& GetGlobalShader() const {return(Shaders->RenderingShader);}
	void OnEndAnim(IRenderingObject *obj);

protected:
	void Init();
	void UnInit();
	bool PreRender();
	void Render();
	void OnPeripheralAdd(unsigned int id,const char *name,EPeriphType type);
	void OnPeripheralRemove(unsigned int id,const char *name);
	void OnKeyDown(unsigned int id,int keyCode);
	void OnKeyUp(unsigned int id,int keyCode);
	void OnMouseMove(unsigned int id,double x,double y);
	void On6axisChange(unsigned int id,double x,double y,double z,double rx,double ry,double rz);
	void OnWheelChange(unsigned int id,double x,double y);
	void OnHatChange(unsigned int id,int sub,double x,double y);
	void OnTiltChange(unsigned int id,double x,double y);
	void OnAxeChange(unsigned int id,GlWindow::EPeriphAxe axe,double val);
	void OnMouseButtonDown(unsigned int id,int b,double x,double y);
	void OnMouseButtonUp(unsigned int id,int b,double x,double y);
    void OnTouchDown(unsigned int id,int b,double x,double y);
	void OnTouchUp(unsigned int id,int b,double x,double y);
	void OnButtonDown(unsigned int id,int b);
	void OnButtonUp(unsigned int id,int b);
	void OnJoystickButtonDown(unsigned int id,int b);
	void OnJoystickButtonUp(unsigned int id,int b);
	void OnGamepadButtonDown(unsigned int id,int b);
	void OnGamepadButtonUp(unsigned int id,int b);

private:
	void TransformCoord(double &x,double &y);

private:
	GLuint VBObuffer;
	GLuint VBOindex;
	GLuint VBOtex;
	unsigned int nbIndexes;
	SShaders *Shaders;
	OpenUtility::CMat4x4<float> MVPmatrix,Nmatrix;
	OpenUtility::CTextureQuad *TexQuad;
	OpenUtility::CTextureMultiQuad *TexMultiQuad;
	MUTEX MutexMouse;
	bool MouseHasMove;
	OpenUtility::CTable<int> TabIdMice;
	OpenUtility::CVector<CMouse> TabMice;
	OpenUtility::SFrustum Frustum;
	OpenUtility::SPivotd CameraPos;
	OpenUtility::CMat4x4<float> MVmatrix,Pmatrix;
	OpenUtility::CQuad *Background;
	OpenUtility::CFontLoader *Font30;
	OpenUtility::C3DText *FpsText;
	CTaskBar *TaskBar;
	OpenUtility::CVector<IScreen> TabScreen;
	OpenUtility::CVector<CScreenBox> TabScreenBox;
	OpenUtility::CTexture *NullTex;
	double WidthAtZero,ScreenW,ScreenH;
	bool UpdateSpace;
	bool DisplayFps;
	SFBO *OfflineRB;
	CScreenBox *CurTouchScreen;
	CScreenBox *CurDisplayScreen;
	bool OfflineDisplay;
};

#endif
