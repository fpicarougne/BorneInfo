#ifndef _ContenerInterface_h
	#define _ContenerInterface_h

#include "../../Utility/OUException.h"
#include "Iterator.h"

namespace OpenUtility
{

template <class T>
class IContener
{
public:
	class Exception : public OpenUtility::Exception
	{
	public:
		enum EError
		{
			EErrOutOfBounds
		};

	public:
		Exception(EError err,const char *detail=NULL) :
			OpenUtility::Exception(),
			ErrType(err)
		{
			CStream Err;
			switch(ErrType)
			{
			case EErrOutOfBounds:Err+="Out of bounds";break;
			}
			if (detail) Err.AddFormatStream(" (%s)",detail);
			SetDetail(Err.GetStream());
		}
		Exception(const Exception &obj) : OpenUtility::Exception(obj),ErrType(obj.ErrType) {}
		~Exception() throw() {}
		inline EError GetError() {return(ErrType);}

	private:
		EError ErrType;
	};

public:
	IContener() {}
	virtual ~IContener() {}
	virtual void DeleteAll()=0;
	virtual void RemoveAll()=0;
	virtual unsigned int GetSize() const =0;
	virtual T*& ElementAt(unsigned int index)=0;
	virtual T* GetAt(unsigned int index) const=0;
	virtual T*& operator[](unsigned int index)=0;
	virtual T* operator[](unsigned int index) const=0;
};

}

#endif
