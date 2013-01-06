#include "CTexture.h"
#include "CTexturePngLoader.h"

OpenUtility::CTexturePngLoader PngLoader;

void OpenUtility::CTexture::InitTextureLoader()
{
	static bool isInit=false;

	if (!isInit)
	{
		CTexture::RegisterTextureLoader(&PngLoader);
		isInit=true;
	}
}
