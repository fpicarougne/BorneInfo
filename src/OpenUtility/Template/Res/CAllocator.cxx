#include <new>
#include <memory.h>

template <class T> bool OpenUtility::CAllocator<T>::isVerifDone=false;

template <class T> OpenUtility::CAllocator<T>::CAllocator(bool destructorCall,unsigned int size) :
	pBlock(NULL),
	iLastBlock(0),
	pFreeItem(NULL),
	IsNewOperator(true),
	IsDeleteOperator(destructorCall),
	incSize(size)
{
	VerifSystem();
}

template <class T> OpenUtility::CAllocator<T>::CAllocator(T ObjInit,bool destructorCall,unsigned int size) :
	pBlock(NULL),
	iLastBlock(0),
	pFreeItem(NULL),
	IsNewOperator(false),
	IsDeleteOperator(destructorCall),
	incSize(size)
{
	VerifSystem();

	memcpy(objInit,&ObjInit,sizeof(T));
}

template <class T> void OpenUtility::CAllocator<T>::VerifSystem()
{
	if (!isVerifDone)
	{
		AllocItem *itemTest=(AllocItem*)new unsigned char[sizeof(AllocItem)];
		if ((sizeof(unsigned char)!=1) || (((void*)itemTest)!=((void*)&itemTest->Elmt)))
			THROW(Exception,"Error in system requierment");
		delete[] (unsigned char*)itemTest;
		isVerifDone=true;
	}
}

template <class T> OpenUtility::CAllocator<T>::~CAllocator()
{
	AllocList *block;
	while (pBlock)
	{
		block=pBlock;
		pBlock=pBlock->pNext;
		delete block;
	}
}

template <class T> T* OpenUtility::CAllocator<T>::GetNewElmt()
{
	T* objTemp;

	if (iLastBlock==0)
	{
		if (!pFreeItem)
		{
			pBlock=new AllocList(pBlock,incSize);
			iLastBlock=incSize-1;
			incSize=int(incSize*1.5);
			objTemp=&pBlock->VectItem[iLastBlock].Elmt;
		}
		else
		{
			objTemp=&pFreeItem->Elmt;
			pFreeItem=pFreeItem->pNext;
		}
	}
	else objTemp=&pBlock->VectItem[--iLastBlock].Elmt;

	return(objTemp);
}

template <class T> void OpenUtility::CAllocator<T>::Delete(T *&obj)
{
	if (obj)
	{
		AllocItem *objTemp=(AllocItem*)obj;

		if (IsDeleteOperator) obj->~T();
		objTemp->pNext=pFreeItem;
		pFreeItem=objTemp;
		obj=NULL;
	}
}

template <class T> OpenUtility::CAllocator<T>::AllocList::AllocList(AllocList *obj,unsigned int size)
{
	pNext=obj;
	VectItem=(AllocItem*)new unsigned char[sizeof(AllocItem)*size];
}

template <class T> OpenUtility::CAllocator<T>::AllocList::~AllocList()
{
	delete[] (unsigned char*)VectItem;
}
