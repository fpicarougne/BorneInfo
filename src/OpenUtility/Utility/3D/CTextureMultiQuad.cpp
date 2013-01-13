#include "CTextureMultiQuad.h"
#include <stddef.h>
#include <string.h>

OpenUtility::CTextureMultiQuad::CTextureMultiQuad(const char *srcImg,CVector<SQuad> &quads) :
	VertexTab(NULL)
{
	Texture=CTexture::LoadTextureFile(srcImg);
	HasAllocatedTex=true;
	glGenBuffers(1,&VBObuffer);
	GenQuads(quads);
}

OpenUtility::CTextureMultiQuad::CTextureMultiQuad(const CTexture *tex,CVector<SQuad> &quads) :
	HasAllocatedTex(false),
	Texture(tex),
	VertexTab(NULL)
{
	glGenBuffers(1,&VBObuffer);
	GenQuads(quads);
}

OpenUtility::CTextureMultiQuad::CTextureMultiQuad(const CTextureMultiQuad &obj) :
	HasAllocatedTex(false),
	Texture(obj.Texture)
{
	std::cout<<"(CTextureMultiQuad) copy !"<<std::endl;
	glGenBuffers(1,&VBObuffer);
	CopyQuads(obj);
}

OpenUtility::CTextureMultiQuad::~CTextureMultiQuad()
{
	if (HasAllocatedTex)
		delete Texture;
	delete[] VertexTab;
	Quads.DeleteAll();
	glDeleteBuffers(1,&VBObuffer);
}

OpenUtility::CTextureMultiQuad& OpenUtility::CTextureMultiQuad::operator=(const CTextureMultiQuad &obj)
{
	std::cout<<"(CTextureMultiQuad) operator= !"<<std::endl;
	HasAllocatedTex=false;
	Texture=obj.Texture;
	delete[] VertexTab;
	Quads.DeleteAll();
	CopyQuads(obj);

	return(*this);
}

void OpenUtility::CTextureMultiQuad::AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vTex)
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

void OpenUtility::CTextureMultiQuad::Draw(unsigned int i)
{
	if ((i<Quads.GetSize()) && Quads[i]!=NULL)
		glDrawArrays(GL_TRIANGLE_STRIP,i*4,4);
}

void OpenUtility::CTextureMultiQuad::GenQuads(CVector<SQuad> &quads)
{
	if (!Texture) return;
	SQuad *quad;
	double ratioT,w,h;
	double w2,h2,tx,ty,tw,th;

	VertexTab=new SVertex[4*quads.GetSize()];
	for (unsigned int i=0;i<quads.GetSize();i++)
	{
		quad=quads[i];
		if ((quad->x+quad->w>Texture->GetW()) || (quad->y+quad->h>Texture->GetH()))
		{
			Quads.Add(NULL);
			continue;
		}

		ratioT=double(quad->w)/quad->h;
		if (quad->maxW/quad->maxH>ratioT)
		{
			h=quad->maxH;
			w=h*ratioT;
		}
		else
		{
			w=quad->maxW;
			h=w/ratioT;
		}
		Quads.Add(new CQuad(this,i,w,h));

		w2=w/2;
		h2=h/2;
		tx=double(quad->x)/Texture->GetWT();
		ty=double(quad->y)/Texture->GetHT();
		tw=double(quad->w)/Texture->GetWT();
		th=double(quad->h)/Texture->GetHT();

		SetVertex(VertexTab[i*4],-w2,-h2,tx,ty);
		SetVertex(VertexTab[i*4+1],w2,-h2,tx+tw,ty);
		SetVertex(VertexTab[i*4+2],-w2,h2,tx,ty+th);
		SetVertex(VertexTab[i*4+3],w2,h2,tx+tw,ty+th);
	}

	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(SVertex)*4*quads.GetSize(),VertexTab,GL_STATIC_DRAW);
}

void OpenUtility::CTextureMultiQuad::CopyQuads(const CTextureMultiQuad &obj)
{
	CQuad *quad;

	VertexTab=new SVertex[4*obj.Quads.GetSize()];
	memcpy(VertexTab,obj.VertexTab,sizeof(SVertex)*4*obj.Quads.GetSize());
	for (unsigned int i=0;i<obj.Quads.GetSize();i++)
	{
		quad=obj.Quads[i];
		if (quad==NULL) Quads.Add(NULL);
		else Quads.Add(new CQuad(*quad));
	}

	glBindBuffer(GL_ARRAY_BUFFER,VBObuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(SVertex)*4*obj.Quads.GetSize(),VertexTab,GL_STATIC_DRAW);
}

void OpenUtility::CTextureMultiQuad::SetVertex(SVertex &vertex,double posX,double posY,double texX,double texY)
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
