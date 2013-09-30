#ifndef _C3DMesh_h
	#define _C3DMesh_h

#ifdef GLES2
	#include <GLES2/gl2.h>
#else
	#include <GL/gl.h>
#endif
#include "CShaderProgram.h"
#include "CTexture.h"
#include "3D.h"
#include "../../Template/CMat4x4.h"

namespace OpenUtility
{

class C3DMesh
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

public:
	C3DMesh(const float *tab,const unsigned int size,bool useDefaultShader=true);
//	C3DMesh(const float *textureTab,const float *normalTab,const float *vertexTab,const unsigned int size,bool useDefaultShader=true);
	virtual ~C3DMesh();
	void UseDefaultShader(bool useDefShader);
	inline void SetDefaultShaderMatrix(const CMat4x4<float> &Pmatrix,const CMat4x4<float> &MVmatrix,const CMat4x4<float> &Nmatrix) {DefMVPmatrix=Pmatrix*MVmatrix;DefMVmatrix=MVmatrix;DefNmatrix=Nmatrix;}
	inline void SetLightPos(const XYZf &pos) {LightPos=pos;}
	inline void SetLightPos(float x,float y,float z) {LightPos.Set(x,y,z);}
	inline void SetTexture(const CTexture *tex=NULL) {Texture=tex;IsTextureIdLoaded=false;}
	inline void SetTexture(GLuint texId) {TextureId=texId;IsTextureIdLoaded=true;}
	void AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vColor);
	void Draw();

private:
	static SShaders *DefaultShader;
	static unsigned int DefShaderInUse;
	bool IsDefaultShaderInUse;
	CMat4x4<float> DefMVPmatrix,DefMVmatrix,DefNmatrix;
	XYZf LightPos;
	const CTexture *Texture;
	GLuint TextureId;
	bool IsTextureIdLoaded;
	GLuint VBObuffer;
	unsigned int NbVertex;
};

class C3DCube : public C3DMesh {public:C3DCube(bool useDefaultShader=true);};
class C3DDodecahedron : public C3DMesh {public:C3DDodecahedron(bool useDefaultShader=true);};
class C3DFootball : public C3DMesh {public:C3DFootball(bool useDefaultShader=true);};
class C3DIcosahedron : public C3DMesh {public:C3DIcosahedron(bool useDefaultShader=true);};
class C3DOctahedron : public C3DMesh {public:C3DOctahedron(bool useDefaultShader=true);};
class C3DTeapot : public C3DMesh {public:C3DTeapot(bool useDefaultShader=true);};
class C3DTetrahedron : public C3DMesh {public:C3DTetrahedron(bool useDefaultShader=true);};
//class C3DBunny : public C3DMesh {public:C3DBunny(bool useDefaultShader=true);};
//class C3DDragon : public C3DMesh {public:C3DDragon(bool useDefaultShader=true);};
//class C3DBuddah : public C3DMesh {public:C3DBuddah(bool useDefaultShader=true);};

}

#endif
