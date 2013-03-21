//coding: utf-8
#include "Mouse.h"
#include <stddef.h>

static const char MouseVertex[]="\
attribute vec4 vPos;\
attribute vec4 vColor;\
\
uniform mat4 u_MVPmatrix;\
\
varying vec4 v_Color;\
\
void main()\
{\
	v_Color = vColor;\
	gl_Position = u_MVPmatrix * vPos;\
}\
";

static const char MouseFragment[]="\
precision mediump float;\n\
\
varying vec4 v_Color;\
\
void main()\
{\
	gl_FragColor = v_Color;\
}\
";

CMouse::CMouse(unsigned int width,unsigned int height,double hideDelay) :
	Width(width),
	Height(height),
	HideDelay(hideDelay),
	LastMove(0)
{
	glGenBuffers(1,&VBObuffer);
	InitObject();
}

CMouse::CMouse(const CMouse &obj)
{
	glGenBuffers(1,&VBObuffer);
	CopyObject(obj);
}

CMouse& CMouse::operator=(const CMouse &obj)
{
	CopyObject(obj);
	return(*this);
}

void CMouse::InitObject()
{
	SVertex VertexArray[6];

	unsigned int i=0;
	SetVertex(VertexArray[i++],0,0,1,0.5,0.5,1);
	SetVertex(VertexArray[i++],5,12,1,0,0,1);
	SetVertex(VertexArray[i++],6,6,1,0.5,0.5,1);
	SetVertex(VertexArray[i++],0,0,1,0.5,0.5,1);
	SetVertex(VertexArray[i++],6,6,1,0.5,0.5,1);
	SetVertex(VertexArray[i++],12,5,1,0,0,1);

	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(VertexArray),VertexArray,GL_STATIC_DRAW);

	Pmatrix.SetOrtho(0,Width-1,Height-1,0,1,-1);

	if (!Shaders.ShaderVertex.LoadStream(MouseVertex))
		std::cout << "-----------------------------------\nCMouse: Erreur vertex shader :\n" << Shaders.ShaderVertex.GetLog() << std::endl << "--------------------------" << std::endl;
	if (!Shaders.ShaderFragment.LoadStream(MouseFragment))
		std::cout << "-----------------------------------\nCMouse: Erreur fragment shader :\n" << Shaders.ShaderFragment.GetLog() << std::endl << "--------------------------" << std::endl;
	if (!Shaders.RenderingShader.LinkProgram())
		std::cout << "-----------------------------------\nCMouse: Erreur shader program :\n" << Shaders.RenderingShader.GetLog() << std::endl << "--------------------------" << std::endl;
}

void CMouse::CopyObject(const CMouse &obj)
{
	Width=obj.Width;
	Height=obj.Height;
	HideDelay=obj.HideDelay;
	LastMove=obj.LastMove;
	InitObject();
}

CMouse::~CMouse()
{
	glDeleteBuffers(1,&VBObuffer);
}

void CMouse::SetVertex(SVertex &vertex,double posX,double posY,double colorR,double colorG,double colorB,double colorA)
{
	vertex.position[0]=posX;
	vertex.position[1]=posY;
	vertex.position[2]=0;
	vertex.color[0]=colorR;
	vertex.color[1]=colorG;
	vertex.color[2]=colorB;
	vertex.color[3]=colorA;
}

void CMouse::SetLabel(const char *label)
{
}

void CMouse::SetPixelPosition(unsigned int x,unsigned int y)
{
	MVPmatrix=Pmatrix*OpenUtility::CMat4x4<float>().SetTranslate(x,y,0);
}

void CMouse::SetRelativePosition(double x,double y)
{
	SetPixelPosition(x*(Width-1),y*(Height-1));
}

void CMouse::Draw()
{
	Shaders.RenderingShader.UseProgram();

	GLint vPos=Shaders.RenderingShader["vPos"];
	GLint vColor=Shaders.RenderingShader["vColor"];

	glUniformMatrix4fv(Shaders.RenderingShader["u_MVPmatrix"],1,GL_FALSE,MVPmatrix.GetMatrix());
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glVertexAttribPointer(vPos,3,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,position));
	glVertexAttribPointer(vColor,4,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,color));
	glEnableVertexAttribArray(vPos);
	glEnableVertexAttribArray(vColor);

	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES,0,6);
	glEnable(GL_DEPTH_TEST);
}
