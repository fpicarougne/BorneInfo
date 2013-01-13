#ifndef _CTextureMultiQuad_h
    #define	_CTextureMultiQuad_h

#include "CTexture.h"
#include "../../Template/CVector.h"

namespace OpenUtility
{

class CTextureMultiQuad
{
public:
	struct SQuad
	{
		SQuad(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int maxw,double maxh) : x(x), y(y), w(w), h(h), maxW(maxw), maxH(maxh) {}
		unsigned int x,y;
		unsigned int w,h;
		double maxW,maxH;
	};

	class CQuad
	{
	friend class CTextureMultiQuad;

	private:
		CQuad(CTextureMultiQuad *multiQuad,unsigned int id,unsigned int w,unsigned int h) : MultiQuad(multiQuad), Id(id), W(w), H(h) {}
		CQuad(const CQuad &obj) : MultiQuad(obj.MultiQuad), Id(obj.Id), W(obj.W), H(obj.H) {}
		CQuad& operator=(const CQuad &obj) {MultiQuad=obj.MultiQuad;Id=obj.Id;W=obj.W;H=obj.H;return(*this);}

	public:
		inline double GetW() const {return(W);}
		inline double GetH() const {return(H);}
		inline void AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vTex) {MultiQuad->AttachAttribToData(vPos,vNorm,vTex);}
		inline void Draw() {MultiQuad->Draw(Id);}

	private:
		CTextureMultiQuad *MultiQuad;
		unsigned int Id;
		unsigned int W,H;
	};

private:
	struct SVertex
	{
		GLfloat position[3];
		GLfloat normal[3];
		GLfloat texcoord[2];
	};

public:
	CTextureMultiQuad(const char *srcImg,CVector<SQuad> &quads);
	CTextureMultiQuad(const CTexture *tex,CVector<SQuad> &quads);
	CTextureMultiQuad(const CTextureMultiQuad &obj);
	virtual ~CTextureMultiQuad();
	CTextureMultiQuad& operator=(const CTextureMultiQuad &obj);
	inline const CTexture* GetTexture() const {return(Texture);}
	void AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vTex);
	void Draw(unsigned int i);
	inline unsigned int GetSize() const {return(Quads.GetSize());}
	inline CQuad* operator[](unsigned int i) {return(Quads[i]);}

private:
	void GenQuads(CVector<SQuad> &quads);
	void CopyQuads(const CTextureMultiQuad &obj);
	void SetVertex(SVertex &vertex,double posX,double posY,double texX,double texY);

private:
	bool HasAllocatedTex;
	const CTexture *Texture;
	GLuint VBObuffer;
	SVertex *VertexTab;
	CVector<CQuad> Quads;
};

}

#endif

