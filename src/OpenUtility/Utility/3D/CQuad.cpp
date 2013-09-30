#include "CQuad.h"

OpenUtility::CQuad::SShaders *OpenUtility::CQuad::DefaultShader=NULL;
unsigned int OpenUtility::CQuad::DefShaderInUse=0;

static const char QuadVertex[]="\
attribute vec4 vPos;\n\
attribute vec3 vNorm;\n\
attribute vec4 vColor;\n\
\n\
uniform mat4 u_MVPmatrix;\n\
uniform mat4 u_Nmatrix;\n\
\n\
varying vec4 v_Color;\n\
varying vec3 v_Normal;\n\
\n\
void main()\n\
{\n\
	v_Color = vColor;\n\
	v_Normal = mat3(u_Nmatrix) * vNorm;\n\
	gl_Position = u_MVPmatrix * vPos;\n\
}\n\
";

static const char QuadFragment[]="\
precision mediump float;\n\
\n\
uniform float u_Alpha;\n\
\n\
varying vec4 v_Color;\n\
varying vec3 v_Normal;\n\
\n\
void main()\n\
{\n\
	gl_FragColor = vec4(v_Color.rgb,v_Color.a*u_Alpha);\n\
}\n\
";

//#############################################################################################

OpenUtility::CQuad::CQuad(double x1,double y1,double x2,double y2,double r,double g,double b,double a,bool useDefaultShader) :
	IsDefaultShaderInUse(false),
	Alpha(1.0)
{
	SVertex VertexArray[4];

	UseDefaultShader(useDefaultShader);
	OpenUtility::RGBAd color(r,g,b,a);
	Update(VertexArray,x1,y1,x2,y2,color,color,color,color);
	glGenBuffers(1,&VBObuffer);
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(VertexArray),VertexArray,GL_DYNAMIC_DRAW);
}

OpenUtility::CQuad::CQuad(double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &color,bool useDefaultShader) :
	IsDefaultShaderInUse(false),
	Alpha(1.0)
{
	SVertex VertexArray[4];

	UseDefaultShader(useDefaultShader);
	Update(VertexArray,x1,y1,x2,y2,color,color,color,color);
	glGenBuffers(1,&VBObuffer);
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(VertexArray),VertexArray,GL_DYNAMIC_DRAW);
}

OpenUtility::CQuad::CQuad(double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &tl,const OpenUtility::RGBAd &tr,const OpenUtility::RGBAd &bl,const OpenUtility::RGBAd &br,bool useDefaultShader) :
	IsDefaultShaderInUse(false),
	Alpha(1.0)
{
	SVertex VertexArray[4];

	UseDefaultShader(useDefaultShader);
	Update(VertexArray,x1,y1,x2,y2,tl,tr,bl,br);
	glGenBuffers(1,&VBObuffer);
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(VertexArray),VertexArray,GL_DYNAMIC_DRAW);
}

OpenUtility::CQuad::~CQuad()
{
	UseDefaultShader(false);
	glDeleteBuffers(1,&VBObuffer);
}

void OpenUtility::CQuad::UseDefaultShader(bool useDefShader)
{
	if (useDefShader)
	{
		if (!DefaultShader)
		{
			DefaultShader=new SShaders;
			if (!DefaultShader->ShaderVertex.LoadStream(QuadVertex))
				std::cout << "-----------------------------------\nCTaskBar: Erreur vertex shader :\n" << DefaultShader->ShaderVertex.GetLog() << std::endl << "--------------------------" << std::endl;
			if (!DefaultShader->ShaderFragment.LoadStream(QuadFragment))
				std::cout << "-----------------------------------\nCTaskBar: Erreur fragment shader :\n" << DefaultShader->ShaderFragment.GetLog() << std::endl << "--------------------------" << std::endl;
			if (!DefaultShader->RenderingShader.LinkProgram())
				std::cout << "-----------------------------------\nCTaskBar: Erreur shader program :\n" << DefaultShader->RenderingShader.GetLog() << std::endl << "--------------------------" << std::endl;
		}
		if (!IsDefaultShaderInUse) DefShaderInUse++;
	}
	else
	{
		if (IsDefaultShaderInUse)
		{
			DefShaderInUse--;
			if (DefShaderInUse==0)
			{
				delete DefaultShader;
				DefaultShader=NULL;
			}
		}
	}
	IsDefaultShaderInUse=useDefShader;
}

void OpenUtility::CQuad::UpdateCoord(double x1,double y1,double x2,double y2,double r,double g,double b,double a)
{
	OpenUtility::RGBAd color(r,g,b,a);
	UpdateCoord(x1,y1,x2,y2,color,color,color,color);
}

void OpenUtility::CQuad::UpdateCoord(double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &tl,const OpenUtility::RGBAd &tr,const OpenUtility::RGBAd &bl,const OpenUtility::RGBAd &br)
{
	SVertex VertexArray[4];

	Update(VertexArray,x1,y1,x2,y2,tl,tr,bl,br);
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(VertexArray),VertexArray);
}

void OpenUtility::CQuad::Update(SVertex VertexArray[4],double x1,double y1,double x2,double y2,const OpenUtility::RGBAd &tl,const OpenUtility::RGBAd &tr,const OpenUtility::RGBAd &bl,const OpenUtility::RGBAd &br)
{
	double t;

	if (x1>x2)
	{
		t=x1;
		x1=x2;
		x2=t;
	}
	if (y1>y2)
	{
		t=y1;
		y1=y2;
		y2=t;
	}

	SetVertex(VertexArray[0],x1,y1,bl.r,bl.g,bl.b,bl.a);
	SetVertex(VertexArray[1],x2,y1,br.r,br.g,br.b,br.a);
	SetVertex(VertexArray[2],x1,y2,tl.r,tl.g,tl.b,tl.a);
	SetVertex(VertexArray[3],x2,y2,tr.r,tr.g,tr.b,tr.a);
}

void OpenUtility::CQuad::SetVertex(SVertex &vertex,double posX,double posY,double r,double g,double b,double a)
{
	vertex.position[0]=posX;
	vertex.position[1]=posY;
	vertex.position[2]=0;
	vertex.normal[0]=0;
	vertex.normal[1]=0;
	vertex.normal[2]=1.0;
	vertex.color[0]=r;
	vertex.color[1]=g;
	vertex.color[2]=b;
	vertex.color[3]=a;
}

void OpenUtility::CQuad::AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vColor)
{
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glVertexAttribPointer(vPos,3,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,position));
	glVertexAttribPointer(vNorm,3,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,normal));
	glVertexAttribPointer(vColor,4,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,color));
	glEnableVertexAttribArray(vPos);
	glEnableVertexAttribArray(vNorm);
	glEnableVertexAttribArray(vColor);
}

void OpenUtility::CQuad::Draw()
{
	if (IsDefaultShaderInUse && DefaultShader)
	{
		DefaultShader->RenderingShader.UseProgram();
		glUniform1f(DefaultShader->RenderingShader["u_Alpha"],Alpha);
		glUniformMatrix4fv(DefaultShader->RenderingShader["u_MVPmatrix"],1,GL_FALSE,DefMVPmatrix.GetMatrix());
		glUniformMatrix4fv(DefaultShader->RenderingShader["u_Nmatrix"],1,GL_FALSE,DefNmatrix.GetMatrix());
		AttachAttribToData(DefaultShader->RenderingShader["vPos"],DefaultShader->RenderingShader["vNorm"],DefaultShader->RenderingShader["vColor"]);
	}
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
