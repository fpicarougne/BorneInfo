#ifndef _C3DText_h
	#define _C3DText_h

#include "CFontEngine.h"

namespace OpenUtility
{

class C3DText : public CFontLoader::CFontEngine
{
public:
	C3DText(const CFontLoader *loader);
	C3DText(const C3DText &obj);
	~C3DText();
	C3DText& operator=(const C3DText &obj);
	void SetText(const char *text);
	void SetAlignement(EHAlign hAlign=EHAlignLeft,EVAlign vAlign=EVAlignBaseligne);
	void DrawText();
};

}

#endif
