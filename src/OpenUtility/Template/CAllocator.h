#ifndef _CAllocator_h
	#define _CAllocator_h

#include "../Utility/OUException.h"
#include <string.h>

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

		AllocList(AllocList *obj,unsigned int size);
		~AllocList();
	};

public:
	CAllocator(bool destructorCall=true,unsigned int size=DEF_SIZE_ALLOCATOR);
	CAllocator(T ObjInit,bool destructorCall=true,unsigned int size=DEF_SIZE_ALLOCATOR);
	~CAllocator();
	void Delete(T *&obj);
	T* New()
	{
		T* objTemp=GetNewElmt();

		if (IsNewOperator) objTemp=new(objTemp) T();
		else memcpy(objTemp,objInit,sizeof(T));

		return(objTemp);
	}

	template<class A1> T* New(A1 a1)
	{
		T* objTemp=GetNewElmt();

		if (IsNewOperator) objTemp=new(objTemp) T(a1);
		else THROW(Exception,"Cannot call New with an argument");

		return(objTemp);
	}

	template<class A1,class A2> T* New(A1 a1,A2 a2)
	{
		T* objTemp=GetNewElmt();

		if (IsNewOperator) objTemp=new(objTemp) T(a1,a2);
		else THROW(Exception,"Cannot call New with arguments");

		return(objTemp);
	}

	template<class A1,class A2,class A3> T* New(A1 a1,A2 a2,A3 a3)
	{
		T* objTemp=GetNewElmt();

		if (IsNewOperator) objTemp=new(objTemp) T(a1,a2,a3);
		else THROW(Exception,"Cannot call New with arguments");

		return(objTemp);
	}

	template<class A1,class A2,class A3,class A4> T* New(A1 a1,A2 a2,A3 a3,A4 a4)
	{
		T* objTemp=GetNewElmt();

		if (IsNewOperator) objTemp=new(objTemp) T(a1,a2,a3,a4);
		else THROW(Exception,"Cannot call New with arguments");

		return(objTemp);
	}

private:
	static void VerifSystem();
	CAllocator(const CAllocator &obj) {}
	CAllocator& operator=(const CAllocator &obj) {}
	T* GetNewElmt();

private:
	AllocList *pBlock;
	unsigned int iLastBlock;
	AllocItem *pFreeItem;
	bool IsNewOperator;
	bool IsDeleteOperator;
	unsigned char objInit[sizeof(T)];
	unsigned int incSize;
	static bool isVerifDone;
};

}

#include "Res/CAllocator.cxx"

#endif
