#include <string.h>
#include "../../Utility/Memory.h"

template <class T,class F,class G> bool OpenUtility::CMap<T,F,G>::isVerifDone=false;

template <class T,class F,class G> OpenUtility::CMap<T,F,G>::CMap(unsigned int nBlockSize,unsigned int nHashTableSize)
	: m_pBlocks(true,nBlockSize)
{
	CommonConstruction(nBlockSize,nHashTableSize);

	pFnClass=NULL;
	fnHashObj=NULL;
	fnHash=NULL;
}

template <class T,class F,class G> OpenUtility::CMap<T,F,G>::CMap(unsigned int (F::*Hash)(T* Obj),F *pClass,unsigned int nBlockSize,unsigned int nHashTableSize)
	: m_pBlocks(true,nBlockSize)
{
	CommonConstruction(nBlockSize,nHashTableSize);

	pFnClass=pClass;
	fnHashObj=Hash;
	fnHash=NULL;
}

template <class T,class F,class G> OpenUtility::CMap<T,F,G>::CMap(unsigned int (*Hash)(T* Obj),unsigned int nBlockSize,unsigned int nHashTableSize)
	: m_pBlocks(true,nBlockSize)
{
	CommonConstruction(nBlockSize,nHashTableSize);

	pFnClass=NULL;
	fnHashObj=NULL;
	fnHash=Hash;
}

template <class T,class F,class G> OpenUtility::CMap<T,F,G>::CMap(unsigned int (*Hash)(T* Obj),int (*fnCmp)(T &Obj1,T &Obj2),unsigned int nBlockSize,unsigned int nHashTableSize)
	: m_pBlocks(true,nBlockSize)
{
	CommonConstruction(nBlockSize,nHashTableSize);

	pFnClass=NULL;
	fnHashObj=NULL;
	fnHash=Hash;
	pFnClassComp=NULL;
	fnCompObj=NULL;
	fnCompObj2=NULL;
	fnComp=fnCmp;
	fnComp2=NULL;
}

template <class T,class F,class G> OpenUtility::CMap<T,F,G>::CMap(unsigned int (*Hash)(T* Obj),int (*fnCmp)(T Obj1,T Obj2),unsigned int nBlockSize,unsigned int nHashTableSize)
	: m_pBlocks(true,nBlockSize)
{
	CommonConstruction(nBlockSize,nHashTableSize);

	pFnClass=NULL;
	fnHashObj=NULL;
	fnHash=Hash;
	pFnClassComp=NULL;
	fnCompObj=NULL;
	fnCompObj2=NULL;
	fnComp=NULL;
	fnComp2=fnCmp;
}

template <class T,class F,class G> void OpenUtility::CMap<T,F,G>::CommonConstruction(unsigned int nBlockSize,unsigned int nHashTableSize)
{
	VerifSystem();

	if (!nBlockSize)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	m_pHashTable=NULL;
	m_nHashTableSize=nHashTableSize;	// default size
	m_nCount=0;

	pFnClassComp=NULL;
	fnCompObj=NULL;
	fnCompObj2=NULL;
	fnComp=NULL;
	fnComp2=NULL;
}

template <class T,class F,class G> OpenUtility::CMap<T,F,G>::~CMap()
{
	RemoveAll();
}

template <class T,class F,class G> void OpenUtility::CMap<T,F,G>::VerifSystem()
{
	if (!isVerifDone)
	{
		if (sizeof(unsigned char)!=1)
		{
			GetCMyExceptionObj(E,ERR_BAD_INIT);
			throw(E);
		}
		isVerifDone=true;
	}
}

template <class T,class F,class G> void OpenUtility::CMap<T,F,G>::SetFnCompare(int (G::*fnCmp)(T &Obj1,T &Obj2),G *pClass)
{
	pFnClassComp=pClass;
	fnCompObj=fnCmp;
	fnCompObj2=NULL;
	fnComp=NULL;
	fnComp2=NULL;
}

template <class T,class F,class G> void OpenUtility::CMap<T,F,G>::SetFnCompare(int (G::*fnCmp)(T Obj1,T Obj2),G *pClass)
{
	pFnClassComp=pClass;
	fnCompObj=NULL;
	fnCompObj2=fnCmp;
	fnComp=NULL;
	fnComp2=NULL;
}

template <class T,class F,class G> void OpenUtility::CMap<T,F,G>::SetFnCompare(int (*fnCmp)(T &Obj1,T &Obj2))
{
	pFnClassComp=NULL;
	fnCompObj=NULL;
	fnCompObj2=NULL;
	fnComp=fnCmp;
	fnComp2=NULL;
}

template <class T,class F,class G> void OpenUtility::CMap<T,F,G>::SetFnCompare(int (*fnCmp)(T Obj1,T Obj2))
{
	pFnClassComp=NULL;
	fnCompObj=NULL;
	fnCompObj2=NULL;
	fnComp=NULL;
	fnComp2=fnCmp;
}

template <class T,class F,class G> unsigned int OpenUtility::CMap<T,F,G>::HashKey(T *key) const
{
	unsigned int nHash,nbElmt,i;
	unsigned char *elmt;

	nHash=0;
	nbElmt=sizeof(T);
	elmt=(unsigned char*)key;
	for (i=0;i<nbElmt;i++) nHash=(nHash<<5)+nHash+elmt[i];

	return nHash;
}

template <class T,class F,class G> void OpenUtility::CMap<T,F,G>::InitHashTable(unsigned int nHashSize)
{
//
// Used to force allocation of a hash table or to override the default
//   hash table size of (which is fairly small)
	if (m_nCount && !nHashSize)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	if (m_pHashTable!=NULL)
	{
		// free hash table
		delete[] m_pHashTable;
		m_pHashTable=NULL;
	}

	m_pHashTable=new CAssoc*[nHashSize];
	memset(m_pHashTable,0,sizeof(CAssoc*)*nHashSize);

	m_nHashTableSize=nHashSize;
}

template <class T,class F,class G> void OpenUtility::CMap<T,F,G>::RemoveAll()
{
	if (m_pHashTable)
	{
		// destroy elements
		for (unsigned int nHash=0;nHash<m_nHashTableSize;nHash++)
		{
			CAssoc *pAssoc,*oldAssoc;
			for (pAssoc=m_pHashTable[nHash];pAssoc!=NULL;pAssoc=oldAssoc)
			{
				oldAssoc=pAssoc->pNext;
				m_pBlocks.Delete(pAssoc);
			}
		}

		// free hash table
		delete[] m_pHashTable;
		m_pHashTable=NULL;
	}

	m_nCount=0;

//TODO: à voir si on peut pas implémenter un FreeDataChain dans CAllocator
//	m_pBlocks->FreeDataChain();
}

template <class T,class F,class G> typename OpenUtility::CMap<T,F,G>::CAssoc* OpenUtility::CMap<T,F,G>::GetAssocAt(T *key,unsigned int &nHash) const
// find association (or return NULL)
{
	(fnHashObj ? nHash=(pFnClass->*fnHashObj)(key) : (fnHash ? nHash=(*fnHash)(key) : nHash=HashKey(key)));
	nHash%=m_nHashTableSize;

	if (!m_pHashTable) return (NULL);

	// see if it exists
	CAssoc *pAssoc;
	for (pAssoc=m_pHashTable[nHash];pAssoc!=NULL;pAssoc=pAssoc->pNext)
	{
		if (fnCompObj ? !(pFnClassComp->*fnCompObj)(pAssoc->key,*key) : (fnCompObj2 ? !(pFnClassComp->*fnCompObj2)(pAssoc->key,*key) :
			(fnComp ? !(*fnComp)(pAssoc->key,*key) : (fnComp2 ? !(*fnComp2)(pAssoc->key,*key) : pAssoc->key==*key))))
			return(pAssoc);
	}
	return (NULL);
}

/////////////////////////////////////////////////////////////////////////////

template <class T,class F,class G> bool OpenUtility::CMap<T,F,G>::Lookup(T *key,void *&rValue) const
{
	unsigned int nHash;
	CAssoc *pAssoc=GetAssocAt(key,nHash);

	if (!pAssoc) return (false);	// not in map

	rValue=pAssoc->value;
	
	return (true);
}

template <class T,class F,class G> bool OpenUtility::CMap<T,F,G>::Lookup(T key,void *&rValue) const
{
	unsigned int nHash;
	CAssoc *pAssoc=GetAssocAt(&key,nHash);

	if (!pAssoc) return (false);	// not in map

	rValue=pAssoc->value;
	
	return (true);
}

template <class T,class F,class G> void*& OpenUtility::CMap<T,F,G>::operator[](T *key)
{
	unsigned int nHash;
	CAssoc* pAssoc;

	if (!(pAssoc=GetAssocAt(key,nHash)))
	{
		if (!m_pHashTable) InitHashTable(m_nHashTableSize);

		// it doesn't exist, add a new Association
		pAssoc=m_pBlocks.New();
		pAssoc->nHashValue=nHash;
		pAssoc->key=*key;
		// 'pAssoc->value' is a constructed object, nothing more

		// put into hash table
		pAssoc->pNext=m_pHashTable[nHash];
		m_pHashTable[nHash]=pAssoc;
		m_nCount++;
	}
	return (pAssoc->value);	// return new reference
}

template <class T,class F,class G> void*& OpenUtility::CMap<T,F,G>::operator[](T key)
{
	unsigned int nHash;
	CAssoc* pAssoc;

	if (!(pAssoc=GetAssocAt(&key,nHash)))
	{
		if (!m_pHashTable) InitHashTable(m_nHashTableSize);

		// it doesn't exist, add a new Association
		pAssoc=m_pBlocks.New();
		pAssoc->nHashValue=nHash;
		pAssoc->key=key;
		// 'pAssoc->value' is a constructed object, nothing more

		// put into hash table
		pAssoc->pNext=m_pHashTable[nHash];
		m_pHashTable[nHash]=pAssoc;
		m_nCount++;
	}
	return (pAssoc->value);	// return new reference
}

template <class T,class F,class G> bool OpenUtility::CMap<T,F,G>::RemoveKey(T *key)
// remove key - return TRUE if removed
{
	unsigned int nHash;

	if (!m_pHashTable) return (false);	// nothing in the table
	CAssoc **ppAssocPrev;
	CAssoc* pAssoc;

	(fnHashObj ? nHash=(pFnClass->*fnHashObj)(key) : (fnHash ? nHash=(*fnHash)(key) : nHash=HashKey(key)));
	nHash%=m_nHashTableSize;
	ppAssocPrev=&m_pHashTable[nHash];

	for (pAssoc=*ppAssocPrev;pAssoc!=NULL;pAssoc=pAssoc->pNext)
	{
		if (fnCompObj ? !(pFnClassComp->*fnCompObj)(pAssoc->key,*key) : (fnCompObj2 ? !(pFnClassComp->*fnCompObj2)(pAssoc->key,*key) :
			(fnComp ? !(*fnComp)(pAssoc->key,*key) : (fnComp2 ? !(*fnComp2)(pAssoc->key,*key) : pAssoc->key==*key))))
		{
			// remove it
			*ppAssocPrev=pAssoc->pNext;  // remove from list
			m_pBlocks.Delete(pAssoc);
			m_nCount--;
			return (true);
		}
		ppAssocPrev=&pAssoc->pNext;
	}
	return (false);	// not found
}

template <class T,class F,class G> bool OpenUtility::CMap<T,F,G>::RemoveKey(T key)
// remove key - return TRUE if removed
{
	unsigned int nHash;

	if (!m_pHashTable) return (false);	// nothing in the table
	CAssoc **ppAssocPrev;
	CAssoc* pAssoc;

	(fnHashObj ? nHash=(pFnClass->*fnHashObj)(&key) : (fnHash ? nHash=(*fnHash)(&key) : nHash=HashKey(&key)));
	nHash%=m_nHashTableSize;
	ppAssocPrev=&m_pHashTable[nHash];

	for (pAssoc=*ppAssocPrev;pAssoc!=NULL;pAssoc=pAssoc->pNext)
	{
		if (fnCompObj ? !(pFnClassComp->*fnCompObj)(pAssoc->key,key) : (fnCompObj2 ? !(pFnClassComp->*fnCompObj2)(pAssoc->key,key) :
			(fnComp ? !(*fnComp)(pAssoc->key,key) : (fnComp2 ? !(*fnComp2)(pAssoc->key,key) : pAssoc->key==key))))
		{
			// remove it
			*ppAssocPrev=pAssoc->pNext;  // remove from list
			m_pBlocks.Delete(pAssoc);
			m_nCount--;
			return (true);
		}
		ppAssocPrev=&pAssoc->pNext;
	}
	return (false);	// not found
}

/////////////////////////////////////////////////////////////////////////////
// Iterating

template <class T,class F,class G> void OpenUtility::CMap<T,F,G>::GetNextAssoc(MapPOSITION &rNextMapPOSITION,T &rKey,void *&rValue) const
{
	CAssoc *pAssocRet,*pAssocNext;

	if (!m_pHashTable || !m_nCount)	// never call on empty map
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	pAssocRet=(CAssoc*)rNextMapPOSITION;
	if (pAssocRet==NULL)
	{
		// find the first association
		for (unsigned int nBucket=0;nBucket<m_nHashTableSize;nBucket++)
			if (pAssocRet=m_pHashTable[nBucket]) break;
	}

	// find next association
	if (!(pAssocNext=pAssocRet->pNext))
	{
		// go to next bucket
		for (unsigned int nBucket=pAssocRet->nHashValue+1;nBucket<m_nHashTableSize;nBucket++)
			if (pAssocNext=m_pHashTable[nBucket]) break;
	}

	rNextMapPOSITION=(MapPOSITION)pAssocNext;

	// fill in return data
	rKey=pAssocRet->key;
	rValue=pAssocRet->value;
}

template <class T,class F,class G> inline int OpenUtility::CMap<T,F,G>::GetCount() const
{
	return (m_nCount);
}

template <class T,class F,class G> inline bool OpenUtility::CMap<T,F,G>::IsEmpty() const
{
	return (m_nCount==0);
}

template <class T,class F,class G> inline void OpenUtility::CMap<T,F,G>::SetAt(T *key,void* newValue)
{
	(*this)[key]=newValue;
}

template <class T,class F,class G> inline void OpenUtility::CMap<T,F,G>::SetAt(T key,void* newValue)
{
	(*this)[key]=newValue;
}

template <class T,class F,class G> inline MapPOSITION OpenUtility::CMap<T,F,G>::GetStartPosition() const
{
	return (NULL);
}

template <class T,class F,class G> inline unsigned int OpenUtility::CMap<T,F,G>::GetHashTableSize() const
{
	return (m_nHashTableSize);
}
