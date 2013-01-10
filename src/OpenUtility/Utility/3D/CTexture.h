#ifndef _CTexture_h
	#define _CTexture_h

#ifdef GLES2
	#include <GLES2/gl2.h>
#else
	#include <GL/gl.h>
#endif
#include "../../Template/CVector.h"

namespace OpenUtility
{

class CTexture
{
public:
	enum EPicMode
	{
		EPModeA,
		EPModeG,
		EPModeGA,
		EPModeRGB,
		EPModeRGBA
	};

	class CTextureLoader
	{
	public:
		CTextureLoader() {}
		virtual ~CTextureLoader() {}
		virtual unsigned char* Load(const char *file,unsigned long &w,unsigned long &h,CTexture::EPicMode &channel)=0;
		virtual void DestroyData(unsigned char *data)=0;
		virtual bool IsCapable(const char *ext)=0;
		static bool IsCapable(const char *ext,const char *verif);

	protected:
		CTextureLoader(const CTextureLoader &obj) {}
		CTextureLoader& operator=(const CTextureLoader &obj) {return *this;}
	};

public:
	virtual ~CTexture();
	static void RegisterTextureLoader(CTextureLoader *texLoader);
	inline static CTexture* LoadTextureFile(const char *file,bool nonPowerOf2=false) {return(LoadTextureFile(file,0,0,nonPowerOf2));}
	static CTexture* LoadTextureFile(const char *file,unsigned long w,unsigned long h,bool nonPowerOf2=false);
	static CTexture* LoadTextureMemory(const unsigned char *buffer,unsigned long w,unsigned long h,EPicMode channel,bool nonPowerOf2=false);
	inline GLuint GetId() const {return(TextureId);}
	inline unsigned long GetW() const {return(w);}
	inline unsigned long GetH() const {return(h);}
	inline unsigned long GetWT() const {return(wT);}
	inline unsigned long GetHT() const {return(hT);}
	inline EPicMode GetMode() const {return(Mode);}

protected:
	CTexture();
	CTexture(const CTexture &obj) {}
	CTexture& operator=(const CTexture &obj) {return *this;}

private:
	static void InitTextureLoader();
	static CTextureLoader* GetLoader(const char *file);

private:
	static OpenUtility::CVector<CTextureLoader> Loaders;
	GLuint TextureId;
	unsigned long w,h;
	unsigned long wT,hT;
	EPicMode Mode;
};

}

#endif
