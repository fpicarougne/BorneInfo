#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "Memory.h"

void* OpenUtility::MyMalloc(int Size)
{
	void *temp;

	if (NULL==(temp=malloc(Size)))
	{
		OpenUtility::GetCMyExceptionObj(E,ERR_MEMOIRE);
		throw(E);
	}
	return(temp);
}

void* OpenUtility::MyCalloc(size_t num,size_t size)
{
	void *temp;

	if (NULL==(temp=calloc(num,size)))
	{
		OpenUtility::GetCMyExceptionObj(E,ERR_MEMOIRE);
		throw(E);
	}
	return(temp);
}

void* OpenUtility::MyRealloc(void *Ptr,int Size)
{
	void *temp;

	if (NULL==(temp=realloc(Ptr,Size)))
	{
		OpenUtility::GetCMyExceptionObj(E,ERR_MEMOIRE);
		throw(E);
	}

	return(temp);
}

char* OpenUtility::MyStrdup(const char *strSource)
{
	char *temp;

#ifdef WIN32
	if (NULL==(temp=_strdup(strSource)))
#else
	if (NULL==(temp=strdup(strSource)))
#endif
	{
		OpenUtility::GetCMyExceptionObj(E,ERR_MEMOIRE);
		throw(E);
	}

	return(temp);
}

void OpenUtility::MyFree(void *Ptr)
{
	free(Ptr);
	Ptr=NULL;
}
