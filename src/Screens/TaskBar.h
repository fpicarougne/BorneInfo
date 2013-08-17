//coding: utf-8
#ifndef _TaskBar_h
	#define _TaskBar_h

#include <Utility/3D/C3DText.h>
#include <Utility/3D/CTextureQuad.h>
#include <Template/CListe.h>
#include "../RenderingObject.h"

class CTaskBar : public IRenderingObject
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

	class CQuad
	{
	private:
		struct SVertex
		{
			GLfloat position[3];
			GLfloat normal[3];
			GLfloat color[4];
		};

	public:
		CQuad(double x1,double y1,double x2,double y2,double r=1.0,double g=1.0,double b=1.0,double a=1.0);
		CQuad(double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &tl,const OpenUtility::RGBAd &tr,const OpenUtility::RGBAd &bl,const OpenUtility::RGBAd &br);
		~CQuad();
		void UpdateCoord(double x1,double y1,double x2,double y2,double r=1.0,double g=1.0,double b=1.0,double a=1.0);
		void UpdateCoord(double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &tl,const OpenUtility::RGBAd &tr,const OpenUtility::RGBAd &bl,const OpenUtility::RGBAd &br);
		void AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vColor);
		void Draw();

	private:
		void Update(SVertex VertexArray[4],double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &tl,const OpenUtility::RGBAd &tr,const OpenUtility::RGBAd &bl,const OpenUtility::RGBAd &br);
		void SetVertex(SVertex &vertex,double posX,double posY,double r=1.0,double g=1.0,double b=1.0,double a=1.0);

	private:
		GLuint VBObuffer;
	};

	struct SAnim
	{
		unsigned long long StartTime;
		unsigned long long Duration;
		OpenUtility::CListe<SAnim>::CListeIterator pos;
	};

public:
	CTaskBar(IRenderingObjectComm *pClass);
	CTaskBar(const CTaskBar &obj);
	virtual ~CTaskBar();
	CTaskBar& operator=(const CTaskBar &obj);
	void Init();
	void UnInit();
	bool PreRender(unsigned long long timeUnit);
	void Render();
	void SetMatrix(OpenUtility::CMat4x4<float> &p,OpenUtility::CMat4x4<float> &mv,OpenUtility::CMat4x4<float> &n);
	bool UseGlobalShaderProgram() {return(false);}

private:
	const char* GetDay(int day);

private:
	SShaders *QuadShader;
	bool MatrixHasChanged;
	OpenUtility::CFontLoader *Font30,*Font50;
	OpenUtility::C3DText *Day,*Date,*HourMin,*Seconds;
	OpenUtility::CTextureQuad *Logo;
	CQuad *Background,*Bg;
	float BgAlpha;
	OpenUtility::CMat4x4<float> MVPmatrixDay,MVPmatrixDate,MVPmatrixHourMin,MVPmatrixSeconds,MVPmatrixLogo,MVPmatrixBackground,MVPmatrixBg;
	// Annimations
	OpenUtility::CListe<SAnim> ListAnnim;
};

#endif
