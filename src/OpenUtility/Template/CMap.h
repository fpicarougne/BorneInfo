#ifndef CMap_h
	#define CMap_h

#ifdef WIN32
	#pragma warning(disable : 4345)
#endif

#include "CAllocator.h"

typedef void* MapPOSITION;

namespace OpenUtility
{

template <class T,class F=T,class G=F>
class CMap
{
private:
	// Association
	struct CAssoc
	{
		CAssoc* pNext;
		unsigned int nHashValue;	// needed for efficient iteration
		T key;
		void* value;
	};

public:
// Construction
	CMap(unsigned int nBlockSize=10,unsigned int nHashTableSize=17);
	CMap(unsigned int (F::*Hash)(T* Obj),F *pClass,unsigned int nBlockSize=10,unsigned int nHashTableSize=17);
	CMap(unsigned int (*Hash)(T* Obj),unsigned int nBlockSize=10,unsigned int nHashTableSize=17);
	CMap(unsigned int (*Hash)(T* Obj),int (*fnCmp)(T &Obj1,T &Obj2),unsigned int nBlockSize=10,unsigned int nHashTableSize=17);
	CMap(unsigned int (*Hash)(T* Obj),int (*fnCmp)(T Obj1,T Obj2),unsigned int nBlockSize=10,unsigned int nHashTableSize=17);
	~CMap();
	void SetFnCompare(int (G::*fnCmp)(T &Obj1,T &Obj2),G *pClass);
	void SetFnCompare(int (G::*fnCmp)(T Obj1,T Obj2),G *pClass);
	void SetFnCompare(int (*fnCmp)(T &Obj1,T &Obj2));
	void SetFnCompare(int (*fnCmp)(T Obj1,T Obj2));

// Attributes
	// number of elements
	int GetCount() const;
	bool IsEmpty() const;

	// Lookup
	bool Lookup(T *key,void *&rValue) const;
	bool Lookup(T key,void *&rValue) const;

// Operations
	// Lookup and add if not there
	void*& operator[](T *key);
	void*& operator[](T key);

	// add a new (key, value) pair
	void SetAt(T *key,void *newValue);
	void SetAt(T key,void *newValue);

	// removing existing (key, ?) pair
	bool RemoveKey(T *key);
	bool RemoveKey(T key);
	void RemoveAll();

	// iterating all (key, value) pairs
	MapPOSITION GetStartPosition() const;
	void GetNextAssoc(MapPOSITION &rNextPosition,T &rKey,void *&rValue) const;

	// advanced features for derived classes
	unsigned int GetHashTableSize() const;
	void InitHashTable(unsigned int hashSize);

private:
	void CommonConstruction(unsigned int nBlockSize,unsigned int nHashTableSize);
	void VerifSystem();
	CAssoc* GetAssocAt(T*,unsigned int&) const;
	unsigned int HashKey(T *key) const;

// Implementation
private:
	static bool isVerifDone;
	CAssoc **m_pHashTable;
	unsigned int m_nHashTableSize;
	int m_nCount;
	CAllocator<CAssoc> m_pBlocks;
	F *pFnClass;
	unsigned int (F::*fnHashObj)(T* Obj);
	unsigned int (*fnHash)(T* Obj);
	G *pFnClassComp;
	int (G::*fnCompObj)(T &Obj1,T &Obj2);
	int (G::*fnCompObj2)(T Obj1,T Obj2);
	int (*fnComp)(T &Obj1,T &Obj2);
	int (*fnComp2)(T Obj1,T Obj2);
};
}

#include "Res/CMap.cxx"

#ifdef WIN32
	#pragma warning(default : 4345)
#endif

#endif
