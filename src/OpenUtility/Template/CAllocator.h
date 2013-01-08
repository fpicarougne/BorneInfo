#ifndef CAllocator_h
	#define CAllocator_h

#include "../Utility/Memory.h"

#define DEF_SIZE_ALLOCATOR 30

namespace OpenUtility
{

template <class T>
class CAllocator
{
private:
	struct AllocItem
	{
		T Elmt;
		AllocItem *pNext;
	};

	struct AllocList
	{
		AllocList *pNext;
		AllocItem *VectItem;

		AllocList(AllocList *obj) {pNext=obj;}
	};

public:
	CAllocator(bool destructorCall=true,unsigned int size=DEF_SIZE_ALLOCATOR);
	CAllocator(T& ObjInit,bool destructorCall=true,unsigned int size=DEF_SIZE_ALLOCATOR);
	~CAllocator();
	T* New();
	void Delete(T *obj);

private:
	void VerifSystem();

private:
	AllocList *pBlock;
	unsigned int iLastBlock;
	AllocItem *pFreeItem;
	bool IsNewOperator;
	bool IsDeleteOperator;
	char objInit[sizeof(T)];
	unsigned int incSize;
	static bool isVerifDone;
};

}

#include "Res/CAllocator.cxx"

#endif
