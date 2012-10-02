#include "CTexture.h"
#include "CTexturePngLoader.h"

CTexturePngLoader PngLoader;

void CTexture::InitTextureLoader()
{
	static bool isInit=false;

	if (!isInit)
	{
		CTexture::RegisterTextureLoader(&PngLoader);
		isInit=true;
	}
}
