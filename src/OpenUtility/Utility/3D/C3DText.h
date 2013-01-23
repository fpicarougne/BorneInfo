#ifndef _C3DText_h
	#define _C3DText_h

#include "CFontEngine.h"
#include "../Stream/Stream.h"

namespace OpenUtility
{

class C3DText : public CFontLoader::CFontEngine
{
private:
	struct SVertex
	{
		GLfloat position[3];
		GLfloat normal[3];
		GLfloat texcoord[2];
	};

public:
	C3DText(const CFontLoader *loader);
	C3DText(const C3DText &obj);
	~C3DText();
	C3DText& operator=(const C3DText &obj);
	void SetText(const char *text,EHAlign hAlign=EHAlignLeft,EVAlign vAlign=EVAlignBaseligne);
	void UpdateText(const char *text);
	void SetAlignement(EHAlign hAlign=EHAlignLeft,EVAlign vAlign=EVAlignBaseligne);
    void AttachAttribToData(GLuint vPos,GLuint vNorm,GLuint vTex);
	void Draw();

private:
    void CommonInit();
	void SetVertex(SVertex &vertex,double posX,double posY,double texX,double texY);

private:
    CStream Text;
    EHAlign CurrentHAlign;
    EVAlign CurrentVAlign;
    SVertex *VertexTab;
	GLubyte *ElementTab;
    GLuint VBObuffer;
	GLuint VBIBuffer;
    int TotalX,MinY,MaxY;
};

}

#endif
