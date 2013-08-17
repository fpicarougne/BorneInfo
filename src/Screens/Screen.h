//coding: utf-8
#ifndef _Screen_h
	#define _Screen_h

#include <Utility/Stream/Stream.h>
#include <Utility/3D/CTextureQuad.h>

class CScreen
{
public:
	CScreen(const char *name,OpenUtility::ITextureQuad *image=NULL);
	CScreen(const CScreen &obj);
	virtual ~CScreen();
	CScreen& operator=(const CScreen &obj);
	inline const OpenUtility::CStream& GetName() const {return(Name);}
	inline const OpenUtility::ITextureQuad* GetLogo() const {return(Image);}
	void DrawScreen();

protected:
	OpenUtility::CStream Name;
	OpenUtility::ITextureQuad *Image;
};

#endif
