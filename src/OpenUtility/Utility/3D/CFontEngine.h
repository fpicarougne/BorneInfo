#ifndef _CFontEngine_h	#define _CFontEngine_h#include "../OUException.h"#include "../Stream/Stream.h"#include "../../Template/CRBTree.h"#include "CTexture.h"#include <stdio.h>#include <ft2build.h>#include FT_FREETYPE_Hnamespace OpenUtility{class CFontLoader{public:	class Exception : public OpenUtility::Exception	{	public:		enum EError		{			EErrBadInit		};	public:		Exception(EError err,const char *detail=NULL) :			OpenUtility::Exception(),			ErrType(err)		{			CStream Err;			switch(ErrType)			{			case EErrBadInit:Err+="There was an initialisation error";break;			}			if (detail) Err.AddFormatStream(" (%s)",detail);			SetDetail(Err.GetStream());		}		Exception(const Exception &obj) : OpenUtility::Exception(obj),ErrType(obj.ErrType) {}		~Exception() throw() {}		inline EError GetError() {return(ErrType);}	private:		EError ErrType;	};	class IFontEngine	{	public:		enum EHAlign		{			EHAlignLeft,			EHAlignCenter,			EHAlignRight		};		enum EVAlign		{			EVAlignTop,			EVAlignMiddle,			EVAlignBaseligne,			EVAlignBottom		};	public:		IFontEngine(const CFontLoader *loader) : FontLoader(loader) {}		virtual ~IFontEngine() {}		virtual void SetText(const char *text,EHAlign hAlign=EHAlignLeft,EVAlign vAlign=EVAlignBaseligne)=0;		virtual void UpdateText(const char *text)=0;		virtual void SetAlignement(EHAlign hAlign=EHAlignLeft,EVAlign vAlign=EVAlignBaseligne)=0;		virtual void Draw()=0;	protected:		const CFontLoader *FontLoader;	};	struct SFontChar	{		unsigned int texX,texY,texW,texH;		unsigned int BearingX,BearingY;		unsigned int Advance;	};	class CFontId	{	public:		CFontId(const char *fName,const char *fStyle,unsigned int fSize) : FontName(fName),FontStyle(fStyle),FontSize(fSize) {}		inline bool operator==(const CFontId &obj) const {return(FontName==obj.FontName && FontStyle==obj.FontStyle && FontSize==obj.FontSize);}		bool operator<(const CFontId &obj) const;		inline unsigned int GetSize() const {return(FontSize);}	private:		CStream FontName;		CStream FontStyle;		unsigned int FontSize;	};	class CFontData	{	friend class CFontLoader;	public:		CFontData(CFontId &fId);		~CFontData() {delete FontTexture;}		inline unsigned int GetSize() const {return(FontId.GetSize());}		inline const CTexture* GetFontTexture() const {return(FontTexture);}		inline const SFontChar* GetCharData(unsigned char c) const {return(&CharsData[c]);}	private:		CFontId FontId;		CTexture *FontTexture;		SFontChar CharsData[256];		unsigned int nbInUse;		bool KeepLoaded;	};public:	CFontLoader(const char *font,unsigned int size,const char *cache=NULL,bool keepLoaded=false);	~CFontLoader();	CFontLoader(const CFontLoader &obj);	CFontLoader& operator=(const CFontLoader &obj);	unsigned int GetSize() const;	const CTexture* GetFontTexture() const;	const SFontChar* GetCharData(unsigned char c) const;private:	CFontData* GenFont(FT_Face face,CStream &file,unsigned int size,CFontId &fId);	CFontData* ReadFont(CStream &file,CFontId &fId);	void PutFile16b(FILE *fp,int val);	int GetFile16b(FILE *fp);	bool WritePngBitmap(FILE *fp,unsigned char *bitmap,unsigned int width,unsigned int height);	unsigned char* ReadPngBitmap(FILE *fp,unsigned int &width,unsigned int &height);private:	static CRedBlackTree<CFontId,CFontData*> FontTree;	CFontData *pFontData;};}#endif