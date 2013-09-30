#ifndef _CQuad_h
	#define _CQuad_h

#ifdef GLES2
	#include <GLES2/gl2.h>
#else
	#include <GL/gl.h>
#endif
#include <stddef.h>
#include "CShaderProgram.h"
#include "3D.h"
#include "../../Template/CMat4x4.h"

namespace OpenUtility
{

class CQuad
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
		GLfloat color[4];
	};

public:
	CQuad(double x1,double y1,double x2,double y2,double r=1.0,double g=1.0,double b=1.0,double a=1.0,bool useDefaultShader=true);
	CQuad(double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &color,bool useDefaultShader=true);
	CQuad(double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &tl,const OpenUtility::RGBAd &tr,const OpenUtility::RGBAd &bl,const OpenUtility::RGBAd &br,bool useDefaultShader=true);
	~CQuad();
	void UseDefaultShader(bool useDefShader);
	inline void SetDefaultShaderMatrix(const CMat4x4<float> &MVPmatrix,const CMat4x4<float> &Nmatrix) {DefMVPmatrix=MVPmatrix;DefNmatrix=Nmatrix;}
	inline void SetAlpha(const float alpha) {Alpha=alpha;}
	void UpdateCoord(double x1,double y1,double x2,double y2,double r=1.0,double g=1.0,double b=1.0,double a=1.0);
	inline void UpdateCoord(double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &color) {UpdateCoord(x1,y1,x2,y2,color,color,color,color);}
	void UpdateCoord(double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &tl,const OpenUtility::RGBAd &tr,const OpenUtility::RGBAd &bl,const OpenUtility::RGBAd &br);
	void AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vColor);
	void Draw();

private:
	void Update(SVertex VertexArray[4],double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &tl,const OpenUtility::RGBAd &tr,const OpenUtility::RGBAd &bl,const OpenUtility::RGBAd &br);
	void SetVertex(SVertex &vertex,double posX,double posY,double r=1.0,double g=1.0,double b=1.0,double a=1.0);

private:
	static SShaders *DefaultShader;
	static unsigned int DefShaderInUse;
	bool IsDefaultShaderInUse;
	CMat4x4<float> DefMVPmatrix,DefNmatrix;
	float Alpha;
	GLuint VBObuffer;
};

}

#endif
