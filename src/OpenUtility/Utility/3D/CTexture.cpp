#include "CTexture.h"
#include <ctype.h>
#include "../Filesystem/FileHandling.h"

OpenUtility::CVector<CTexture::CTextureLoader> CTexture::Loaders;

void CTexture::RegisterTextureLoader(CTexture::CTextureLoader *texLoader)
{
	Loaders.Add(texLoader);
}

bool CTexture::CTextureLoader::IsCapable(const char *ext,const char *verif)
{
	while ((*ext!='\0') && (*verif!='\0') && (tolower(*ext)==tolower(*verif)))
	{
		ext++;
		verif++;
	}
	return((*ext=='\0') && (*verif=='\0'));
}

CTexture::CTexture() :
	TextureId(0)
{
}

CTexture::~CTexture()
{
	if (TextureId) glDeleteTextures(1,&TextureId);
}

CTexture* CTexture::LoadTexture(const char *file,unsigned long w,unsigned long h,bool nonPowerOf2)
{
	CTextureLoader *texLoader;

	if ((texLoader=GetLoader(file))!=NULL)
	{
		unsigned char *data;

		if ((data=texLoader->Load(file,w,h,nonPowerOf2))!=NULL)
		{
			unsigned long wT,hT;
			int i;
			CTexture *texObj;

			if (nonPowerOf2)
			{
				wT=w;
				hT=h;
			}
			else
			{
				i=1;
				while ((w>>i)!=0) i++;
				if (w==(1ul<<(i-1))) i--;
				wT=1<<i;
				i=1;
				while ((h>>i)!=0) i++;
				if (h==(1ul<<(i-1))) i--;
				hT=1<<i;
			}

			texObj=new CTexture();
			texObj->w=w;
			texObj->h=h;
			texObj->wT=wT;
			texObj->hT=hT;

			if (texObj->TextureId==0) glGenTextures(1,&texObj->TextureId);
			glBindTexture(GL_TEXTURE_2D,texObj->TextureId);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,wT,hT,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
			glTexSubImage2D(GL_TEXTURE_2D,0,0,0,w,h,GL_RGB,GL_UNSIGNED_BYTE,data);

			texLoader->DestroyData(data);
			return(texObj);
		}
	}
	return(NULL);
}

CTexture::CTextureLoader* CTexture::GetLoader(const char *file)
{
	OpenUtility::CStream dir,fileName,ext;

	InitTextureLoader();
	ParseFileName(file,dir,fileName,ext);
	for (unsigned int i=0;i<Loaders.GetSize();i++)
	{
		if (Loaders[i]->IsCapable(ext)) return(Loaders[i]);
	}
	return(NULL);
}
