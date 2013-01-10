#ifndef _CTexturePngLoader_h
	#define _CTexturePngLoader_h

#include "CTexture.h"

namespace OpenUtility
{

class CTexturePngLoader : public CTexture::CTextureLoader
{
public:
	CTexturePngLoader() {}
	virtual ~CTexturePngLoader() {}
	unsigned char* Load(const char *file,unsigned long &w,unsigned long &h,CTexture::EPicMode &channel);
	void DestroyData(unsigned char *data);
	bool IsCapable(const char *ext);

private:
	CTexturePngLoader(const CTexturePngLoader &obj) {}
	CTexturePngLoader& operator=(const CTexturePngLoader &obj) {return *this;}
	bool IsCapable(const char *ext,const char *verif);
};

}

#endif
