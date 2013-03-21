//coding: utf-8
#ifndef _Mouse_h
	#define _Mouse_h

#include <GLES2/gl2.h>
#include <Template/CMat4x4.h>
#include <Utility/3D/CShaderProgram.h>

class CMouse
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
		GLfloat color[4];
	};

public:
	CMouse(unsigned int width,unsigned int height,double hideDelay=0);
	CMouse(const CMouse &obj);
	virtual ~CMouse();
	CMouse& operator=(const CMouse &obj);
	void SetLabel(const char *label);
	void SetPixelPosition(unsigned int x,unsigned int y);
	void SetRelativePosition(double x,double y);
	void Draw();

private:
	void InitObject();
	void CopyObject(const CMouse &obj);
	void SetVertex(SVertex &vertex,double posX,double posY,double colorR,double colorG,double colorB,double colorA);

private:
	SShaders Shaders;
	GLuint VBObuffer;
	unsigned int Width,Height;
	unsigned int HideDelay,LastMove;
	OpenUtility::CMat4x4<float> Pmatrix,MVPmatrix;
};

#endif
