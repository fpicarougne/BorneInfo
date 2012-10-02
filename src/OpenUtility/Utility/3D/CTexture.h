#ifndef _CTexture_h
	#define _CTexture_h

#ifdef GLES2
	#include <GLES2/gl2.h>
#else
	#include <GL/gl.h>
#endif
#include "../../Template/CVector.h"

class CTexture
{
public:
	class CTextureLoader
	{
	public:
		CTextureLoader() {}
		virtual ~CTextureLoader() {}
		virtual unsigned char* Load(const char *file,unsigned long &w,unsigned long &h,bool nonPowerOf2=false)=0;
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
	inline static CTexture* LoadTexture(const char *file,bool nonPowerOf2=false) {return(LoadTexture(file,0,0,nonPowerOf2));}
	static CTexture* LoadTexture(const char *file,unsigned long w,unsigned long h,bool nonPowerOf2=false);
	inline GLuint GetId() {return(TextureId);}
	inline unsigned long GetW() {return(w);}
	inline unsigned long GetH() {return(h);}
	inline unsigned long GetWT() {return(wT);}
	inline unsigned long GetHT() {return(hT);}

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
};

#endif
