#include "CTexture.h"
#include <ctype.h>
#include "../FileSystem/FileHandling.h"

OpenUtility::CVector<OpenUtility::CTexture::CTextureLoader> OpenUtility::CTexture::Loaders;

void OpenUtility::CTexture::RegisterTextureLoader(CTexture::CTextureLoader *texLoader)
{
	Loaders.Add(texLoader);
}

bool OpenUtility::CTexture::CTextureLoader::IsCapable(const char *ext,const char *verif)
{
	while ((*ext!='\0') && (*verif!='\0') && (tolower(*ext)==tolower(*verif)))
	{
		ext++;
		verif++;
	}
	return((*ext=='\0') && (*verif=='\0'));
}

OpenUtility::CTexture::CTexture() :
	TextureId(0)
{
}

OpenUtility::CTexture::~CTexture()
{
	if (TextureId) glDeleteTextures(1,&TextureId);
}

OpenUtility::CTexture* OpenUtility::CTexture::LoadTextureFile(const char *file,unsigned long w,unsigned long h,bool nonPowerOf2)
{
	CTextureLoader *texLoader;

	if ((texLoader=GetLoader(file))!=NULL)
	{
		unsigned char *data;
		EPicMode channel;

		if ((data=texLoader->Load(file,w,h,channel))!=NULL)
		{
			CTexture *texObj=LoadTextureMemory(data,w,h,channel,nonPowerOf2);
			texLoader->DestroyData(data);
			return(texObj);
		}
	}
	return(NULL);
}

OpenUtility::CTexture* OpenUtility::CTexture::LoadTextureMemory(const unsigned char *buffer,unsigned long w,unsigned long h,EPicMode channel,bool nonPowerOf2)
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
	texObj->Mode=channel;

	GLenum glMode;
	switch (channel)
	{
	case EPModeA:glMode=GL_ALPHA;break;
	case EPModeG:glMode=GL_LUMINANCE;break;
	case EPModeGA:glMode=GL_LUMINANCE_ALPHA;break;
	case EPModeRGB:glMode=GL_RGB;break;
	case EPModeRGBA:glMode=GL_RGBA;break;
	}
std::cout<<std::endl<<std::endl<< "w="<<w<<" h="<<h<< " wT="<<wT<<" hT="<<hT<<std::endl;

	if (texObj->TextureId==0) glGenTextures(1,&texObj->TextureId);

	glBindTexture(GL_TEXTURE_2D,texObj->TextureId);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,wT,hT,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,w,h,glMode,GL_UNSIGNED_BYTE,buffer);

	return(texObj);
}

OpenUtility::CTexture::CTextureLoader* OpenUtility::CTexture::GetLoader(const char *file)
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
