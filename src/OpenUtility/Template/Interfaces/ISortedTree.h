#ifndef _ISortedTree_h
	#define _ISortedTree_h

#include "Iterator.h"
#include "../../Utility/OUException.h"

namespace OpenUtility
{

template<class K,class V>
class ISortedTree
{
public:
	class Exception : public OpenUtility::Exception
	{
	public:
		enum EError
		{
			EErrNotFound
		};

	public:
		Exception(EError err,const char *detail=NULL) :
			OpenUtility::Exception(),
			ErrType(err)
		{
			CStream Err;
			switch(ErrType)
			{
			case EErrNotFound:Err+="Key not found";break;
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
	ISortedTree() {}
	virtual ~ISortedTree() {}
	virtual bool Add(K key,V val=0)=0;
	virtual V Remove(K key)=0;
	virtual bool Exists(K key)=0;
	virtual V& operator[](K key)=0;
	virtual V operator[](K key) const =0;
};

}

#endif
