#include "CTextureQuad.h"
#include <stddef.h>

OpenUtility::CTextureQuad::CTextureQuad(const char *srcImg,double maxW,double maxH)
{
	Texture=CTexture::LoadTextureFile(srcImg);
	HasAllocatedTex=true;
	CalcSize(maxW,maxH);
}

OpenUtility::CTextureQuad::CTextureQuad(const CTexture *tex,double maxW,double maxH) :
	HasAllocatedTex(false),
	Texture(tex)
{
	CalcSize(maxW,maxH);
}

OpenUtility::CTextureQuad::CTextureQuad(const CTextureQuad &obj) :
	HasAllocatedTex(false),
	Texture(obj.Texture),
	w(obj.w),
	h(obj.h)
{
}

OpenUtility::CTextureQuad& OpenUtility::CTextureQuad::operator=(const CTextureQuad &obj)
{
	HasAllocatedTex=false;
	Texture=obj.Texture;
	w=obj.w;
	h=obj.h;
	return(*this);
}

OpenUtility::CTextureQuad::~CTextureQuad()
{
	if (HasAllocatedTex)
		delete Texture;
	glDeleteBuffers(1,&VBObuffer);
}

void OpenUtility::CTextureQuad::CalcSize(double maxW,double maxH)
{
	if (Texture)
	{
		double ratioT=double(Texture->GetW())/Texture->GetH();

		if (maxW/maxH>ratioT)
		{
			h=maxH;
			w=h*ratioT;
		}
		else
		{
			w=maxW;
			h=w/ratioT;
		}
	}
	else
	{
		w=maxW;
		h=maxH;
	}

	SVertex VertexArray[4];
	double w2=w/2;
	double h2=h/2;
	double tx=double(Texture->GetW())/Texture->GetWT();
	double ty=double(Texture->GetH())/Texture->GetHT();

	SetVertex(VertexArray[0],-w2,-h2,0,0);
	SetVertex(VertexArray[1],w2,-h2,tx,0);
	SetVertex(VertexArray[2],-w2,h2,0,ty);
	SetVertex(VertexArray[3],w2,h2,tx,ty);

	glGenBuffers(1,&VBObuffer);
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(VertexArray),VertexArray,GL_STATIC_DRAW);
}

void OpenUtility::CTextureQuad::SetVertex(SVertex &vertex,double posX,double posY,double texX,double texY)
{
	vertex.position[0]=posX;
	vertex.position[1]=posY;
	vertex.position[2]=0;
	vertex.normal[0]=0;
	vertex.normal[1]=0;
	vertex.normal[2]=1.0;
	vertex.texcoord[0]=texX;
	vertex.texcoord[1]=texY;
}

void OpenUtility::CTextureQuad::AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vTex)
{
	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glVertexAttribPointer(vPos,3,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,position));
	glVertexAttribPointer(vNorm,3,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,normal));
	glVertexAttribPointer(vTex,2,GL_FLOAT,GL_FALSE,sizeof(SVertex),(void*)offsetof(SVertex,texcoord));
	glEnableVertexAttribArray(vPos);
	glEnableVertexAttribArray(vNorm);
	glEnableVertexAttribArray(vTex);
	glBindTexture(GL_TEXTURE_2D,Texture->GetId());
}

void OpenUtility::CTextureQuad::Draw()
{
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
}
