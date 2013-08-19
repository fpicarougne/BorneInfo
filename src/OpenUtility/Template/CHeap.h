#ifndef Heap_h
	#define Heap_h

#include "CVector.h"

#define DEF_SIZE_HEAPSORT 30

namespace OpenUtility
{

template <class T,class F=T>
class CHeap
{
public:
	CHeap(bool opDec=true,int size=DEF_SIZE_HEAPSORT);
	CHeap(bool (F::*Cmp)(T* Obj1,T* Obj2),F *pClass,int size=DEF_SIZE_HEAPSORT);
	CHeap(bool (*Cmp)(T* Obj1,T* Obj2),int size=DEF_SIZE_HEAPSORT);
	~CHeap();
	void AddElmt(T* Obj);
	T* DelElmt();
	inline T* GetElmt(unsigned int index) {return(TabTemp[index]);}
	inline T* GetExtremum() {return(TabTemp[0]);}
	inline unsigned int GetSize() {return(TabTemp.GetSize());}
	inline void DeleteAll() {TabTemp.DeleteAll();}
	inline void RemoveAll() {TabTemp.RemoveAll();}

private:
	CVector<T> TabTemp;
	F *pFnClass;
	bool (F::*fnCmpObj)(T* Obj1,T* Obj2);
	bool (*fnCmp)(T* Obj1,T* Obj2);
	bool DecOp;
};

}

#include "Res/CHeap.cxx"

#endif
