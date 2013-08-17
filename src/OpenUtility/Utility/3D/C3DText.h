#ifndef _C3DText_h
	#define _C3DText_h

#include "CFontEngine.h"
#include "../Stream/Stream.h"
#include "CShaderProgram.h"
#include "3D.h"
#include "../../Template/CMat4x4.h"

namespace OpenUtility
{

class C3DText : public CFontLoader::IFontEngine
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
		GLfloat texcoord[2];
	};

public:
	C3DText(const CFontLoader *loader,double lineHeight=1.0,bool useDefaultShader=true);
	C3DText(const CFontLoader *loader,const double r,const double g,const double b,const double a,double lineHeight=1.0);
	C3DText(const C3DText &obj);
	~C3DText();
	void UseDefaultShader(bool useDefShader);
	inline void SetDefaultShaderMatrix(const CMat4x4<float> &matrix) {DefMVPmatrix=matrix;}
	inline void SetDefaultShaderColor(const double r,const double g,const double b,const double a) {DefColor.Set(r,g,b,a);}
	inline void SetDefaultShaderColor(const RGBAd &color) {DefColor=color;}
	void SetText(const char *text,EHAlign hAlign=EHAlignLeft,EVAlign vAlign=EVAlignBaseligne);
	void UpdateText(const char *text);
	void SetAlignement(EHAlign hAlign=EHAlignLeft,EVAlign vAlign=EVAlignBaseligne);
	double GetWidth();
	double GetHeight();
    void AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vTex);
    void AttachAttribToData(GLuint vPos,GLuint vTex);
	void Draw();

private:
	C3DText& operator=(const C3DText &obj);	//TODO: temporaire
    void CommonInit();
	void SetVertex(SVertex &vertex,double posX,double posY,double texX,double texY);
	void DefaultAttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vTex);

private:
	static SShaders *DefaultShader;
	static unsigned int DefShaderInUse;
	bool IsDefaultShaderInUse;
	CMat4x4<float> DefMVPmatrix;
	RGBAd DefColor;
    CStream Text;
    EHAlign CurrentHAlign;
    EVAlign CurrentVAlign;
	double ReqLineHeight;
    SVertex *VertexTab;
	GLubyte *ElementTab;
    GLuint VBObuffer;
	GLuint VBIBuffer;
    int TotalX,MinY,MaxY;
};

}

#endif
