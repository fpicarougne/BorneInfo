#ifndef _CTextureQuad_h
	#define _CTextureQuad_h

#include "CTexture.h"

namespace OpenUtility
{

class CTextureQuad
{
private:
	struct SVertex
	{
		GLfloat position[3];
		GLfloat normal[3];
		GLfloat texcoord[2];
	};

public:
	CTextureQuad(const char *srcImg,double maxW=1.0,double maxH=1.0);
	CTextureQuad(const CTexture *tex,double maxW=1.0,double maxH=1.0);
	CTextureQuad(const CTextureQuad &obj);
	virtual ~CTextureQuad();
	CTextureQuad& operator=(const CTextureQuad &obj);
	inline const CTexture* GetTexture() const {return(Texture);}
	inline double GetW() const {return(w);}
	inline double GetH() const {return(h);}
	void AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vTex);
	void Draw();

private:
	void CalcSize(double maxW,double maxH);
	void SetVertex(SVertex &vertex,double posX,double posY,double texX,double texY);

private:
	bool HasAllocatedTex;
	const CTexture *Texture;
	double w,h;
	GLuint VBObuffer;
};

}

#endif
