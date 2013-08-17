#ifndef Memory_h
	#define Memory_h

#include <stdio.h>
#include <iostream>
#include "OUException.h"

namespace OpenUtility
{

class MemoryException : public OpenUtility::Exception
{
public:
	enum EError
	{
		EErrOutOfMemory
	};

public:
	MemoryException(EError err,const char *detail=NULL) :
		OpenUtility::Exception(),
		ErrType(err)
	{
		CStream Err;
		switch(ErrType)
		{
		case EErrOutOfMemory:Err+="Out of memory";break;
		}
		if (detail) Err.AddFormatStream(" (%s)",detail);
		SetDetail(Err.GetStream());
	}
	MemoryException(const MemoryException &obj) : OpenUtility::Exception(obj),ErrType(obj.ErrType) {}
	~MemoryException() throw() {}
	inline EError GetError() {return(ErrType);}

private:
	EError ErrType;
};

void* MyMalloc(int Size);
void* MyCalloc(size_t num,size_t size);
void* MyRealloc(void *Ptr,int Size);
char* MyStrdup(const char *strSource);
void MyFree(void *Ptr);

}

#endif
