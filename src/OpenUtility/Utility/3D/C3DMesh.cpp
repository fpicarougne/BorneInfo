#include "C3DMesh.h"

OpenUtility::C3DMesh::SShaders *OpenUtility::C3DMesh::DefaultShader=NULL;
unsigned int OpenUtility::C3DMesh::DefShaderInUse=0;

static const char MeshVertex[]="\
attribute vec4 vPos;\n\
attribute vec3 vNorm;\n\
attribute vec2 vTex;\n\
\n\
uniform mat4 u_MVPmatrix;\n\
uniform mat4 u_MVmatrix;\n\
uniform mat4 u_Nmatrix;\n\
\n\
varying vec3 v_Position;\n\
varying vec3 v_Normal;\n\
varying vec2 v_Tex;\n\
\n\
void main()\n\
{\n\
	v_Tex = vTex;\n\
	v_Normal = normalize(mat3(u_Nmatrix) * vNorm);\n\
	v_Position = vec3(u_MVmatrix * vPos);\n\
	gl_Position = u_MVPmatrix * vPos;\n\
}\n\
";

static const char MeshFragment[]="\
precision mediump float;\n\
\n\
varying vec3 v_Position;\n\
varying vec3 v_Normal;\n\
varying vec2 v_Tex;\n\
uniform vec4 u_LightPos;\n\
\n\
uniform sampler2D u_texId;\n\
\n\
void main()\n\
{\n\
	float distance = length(vec3(u_LightPos) - v_Position);\n\
	vec3 lightVector = normalize(vec3(u_LightPos) - v_Position);\n\
	float diffuse = max(dot(v_Normal, lightVector), 0.1);\n\
	distance=1.0;\n\
	diffuse = max(diffuse * (1.0 / (1.0 + (0.1 * distance * distance))),0.1);\n\
	vec4 texColor = texture2D(u_texId, v_Tex);\n\
	gl_FragColor = vec4(vec3(texColor)*diffuse,1.0);\n\
}\n\
";

//#############################################################################################

#include "Mesh/Cube.h"
#include "Mesh/Dodecahedron.h"
#include "Mesh/Football.h"
#include "Mesh/Icosahedron.h"
#include "Mesh/Octahedron.h"
#include "Mesh/Teapot.h"
#include "Mesh/Tetrahedron.h"
//#include "Mesh/Bunny.h"
//#include "Mesh/Dragon.h"
//#include "Mesh/Buddah.h"

OpenUtility::C3DCube::C3DCube(bool useDefaultShader) : C3DMesh(cube,sizeof(cube)/sizeof(float)/8,useDefaultShader) {}
OpenUtility::C3DDodecahedron::C3DDodecahedron(bool useDefaultShader) : C3DMesh(dodecahedron,sizeof(dodecahedron)/sizeof(float)/8,useDefaultShader) {}
OpenUtility::C3DFootball::C3DFootball(bool useDefaultShader) : C3DMesh(football,sizeof(football)/sizeof(float)/8,useDefaultShader) {}
OpenUtility::C3DIcosahedron::C3DIcosahedron(bool useDefaultShader) : C3DMesh(icosahedron,sizeof(icosahedron)/sizeof(float)/8,useDefaultShader) {}
OpenUtility::C3DOctahedron::C3DOctahedron(bool useDefaultShader) : C3DMesh(octahedron,sizeof(octahedron)/sizeof(float)/8,useDefaultShader) {}
OpenUtility::C3DTeapot::C3DTeapot(bool useDefaultShader) : C3DMesh(teapot,sizeof(teapot)/sizeof(float)/8,useDefaultShader) {}
OpenUtility::C3DTetrahedron::C3DTetrahedron(bool useDefaultShader) : C3DMesh(tetrahedron,sizeof(tetrahedron)/sizeof(float)/8,useDefaultShader) {}
//OpenUtility::C3DBunny::C3DBunny(bool useDefaultShader) : C3DMesh(bunny,sizeof(bunny)/sizeof(float)/8,useDefaultShader) {}
//OpenUtility::C3DDragon::C3DDragon(bool useDefaultShader) : C3DMesh(dragon,sizeof(dragon)/sizeof(float)/8,useDefaultShader) {}
//OpenUtility::C3DBuddah::C3DBuddah(bool useDefaultShader) : C3DMesh(buddah,sizeof(buddah)/sizeof(float)/8,useDefaultShader) {}

//#############################################################################################

OpenUtility::C3DMesh::C3DMesh(const float *tab,const unsigned int nbVertex,bool useDefaultShader) :
	IsDefaultShaderInUse(false),
	Texture(NULL),
	NbVertex(nbVertex)
{
	UseDefaultShader(useDefaultShader);
	glGenBuffers(1,&VBObuffer);
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferData(GL_ARRAY_BUFFER,nbVertex*8*sizeof(float),tab,GL_DYNAMIC_DRAW);
}

/*OpenUtility::C3DMesh::C3DMesh(const float *textureTab,const float *normalTab,const float *vertexTab,const unsigned int nbVertex,bool useDefaultShader) :
	IsDefaultShaderInUse(false),
	NbVertex(nbVertex)
{
	UseDefaultShader(useDefaultShader);
	glGenBuffers(1,&VBObuffer);
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(VertexArray),VertexArray,GL_DYNAMIC_DRAW);
}*/

OpenUtility::C3DMesh::~C3DMesh()
{
	UseDefaultShader(false);
	glDeleteBuffers(1,&VBObuffer);
}

void OpenUtility::C3DMesh::UseDefaultShader(bool useDefShader)
{
	if (useDefShader)
	{
		if (!DefaultShader)
		{
			DefaultShader=new SShaders;
			if (!DefaultShader->ShaderVertex.LoadStream(MeshVertex))
				std::cout << "-----------------------------------\nC3DMesh: Erreur vertex shader :\n" << DefaultShader->ShaderVertex.GetLog() << std::endl << "--------------------------" << std::endl;
			if (!DefaultShader->ShaderFragment.LoadStream(MeshFragment))
				std::cout << "-----------------------------------\nC3DMesh: Erreur fragment shader :\n" << DefaultShader->ShaderFragment.GetLog() << std::endl << "--------------------------" << std::endl;
			if (!DefaultShader->RenderingShader.LinkProgram())
				std::cout << "-----------------------------------\nC3DMesh: Erreur shader program :\n" << DefaultShader->RenderingShader.GetLog() << std::endl << "--------------------------" << std::endl;
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

void OpenUtility::C3DMesh::AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vTex)
{
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glVertexAttribPointer(vPos,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(5*sizeof(float)));
	glVertexAttribPointer(vNorm,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(2*sizeof(float)));
	glVertexAttribPointer(vTex,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
	glEnableVertexAttribArray(vPos);
	glEnableVertexAttribArray(vNorm);
	glEnableVertexAttribArray(vTex);
	if (IsTextureIdLoaded || Texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(DefaultShader->RenderingShader["u_texId"],0);
		if (IsTextureIdLoaded) glBindTexture(GL_TEXTURE_2D,TextureId);
		else glBindTexture(GL_TEXTURE_2D,Texture->GetId());
	}
}

void OpenUtility::C3DMesh::Draw()
{
	if (IsDefaultShaderInUse && DefaultShader)
	{
		DefaultShader->RenderingShader.UseProgram();
		glUniform4f(DefaultShader->RenderingShader["u_LightPos"],LightPos.x,LightPos.y,LightPos.z,1);
		glUniformMatrix4fv(DefaultShader->RenderingShader["u_MVPmatrix"],1,GL_FALSE,DefMVPmatrix.GetMatrix());
		glUniformMatrix4fv(DefaultShader->RenderingShader["u_MVmatrix"],1,GL_FALSE,DefMVmatrix.GetMatrix());
		glUniformMatrix4fv(DefaultShader->RenderingShader["u_Nmatrix"],1,GL_FALSE,DefNmatrix.GetMatrix());
		AttachAttribToData(DefaultShader->RenderingShader["vPos"],DefaultShader->RenderingShader["vNorm"],DefaultShader->RenderingShader["vTex"]);
	}
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glDrawArrays(GL_TRIANGLES,0,NbVertex);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
