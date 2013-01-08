#include <new>
#include <memory.h>

template <class T> bool OpenUtility::CAllocator<T>::isVerifDone=false;

template <class T> OpenUtility::CAllocator<T>::CAllocator(bool destructorCall,unsigned int size)
{
	VerifSystem();

	incSize=size;
	pBlock=NULL;
	iLastBlock=0;
	pFreeItem=NULL;
	IsNewOperator=true;
	IsDeleteOperator=destructorCall;
}

template <class T> OpenUtility::CAllocator<T>::CAllocator(T& ObjInit,bool destructorCall,unsigned int size)
{
	incSize=size;
	pBlock=NULL;
	iLastBlock=0;
	pFreeItem=NULL;
	IsNewOperator=false;
	IsDeleteOperator=destructorCall;
	memcpy(objInit,ObjInit,sizeof(T));
}

template <class T> void OpenUtility::CAllocator<T>::VerifSystem()
{
	if (!isVerifDone)
	{
		AllocItem *itemTest=(AllocItem*)new unsigned char[sizeof(AllocItem)];
		if ((sizeof(unsigned char)!=1) || (((void*)itemTest)!=((void*)&itemTest->Elmt)))
		{
			GetCMyExceptionObj(E,ERR_BAD_INIT);
			throw(E);
		}
		delete[] (unsigned char*)itemTest;
		isVerifDone=true;
	}
}

template <class T> OpenUtility::CAllocator<T>::~CAllocator()
{
	while (pBlock)
	{
		delete[] (unsigned char*)pBlock->VectItem;
		pBlock=pBlock->pNext;
	}
}

template <class T> T* OpenUtility::CAllocator<T>::New()
{
	T* objTemp;

	if (!iLastBlock)
	{
		if (!pFreeItem)
		{
			pBlock=new AllocList(pBlock);
			pBlock->VectItem=(AllocItem*)new unsigned char[sizeof(AllocItem)*incSize];
			iLastBlock=incSize-1;
			objTemp=&pBlock->VectItem[iLastBlock].Elmt;
		}
		else
		{
			objTemp=&pFreeItem->Elmt;
			pFreeItem=pFreeItem->pNext;
		}
	}
	else objTemp=&pBlock->VectItem[--iLastBlock].Elmt;

	if (IsNewOperator) objTemp=new(objTemp) T();
	else memcpy(objTemp,objInit,sizeof(T));

	return(objTemp);
}

template <class T> void OpenUtility::CAllocator<T>::Delete(T *obj)
{
	AllocItem *objTemp=(AllocItem*)obj;

	if (IsDeleteOperator) obj->~T();
	objTemp->pNext=pFreeItem;
	pFreeItem=objTemp;
}
