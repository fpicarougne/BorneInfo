#ifndef Heapsort_h
	#define Heapsort_h

#include "CVector.h"

#define DEF_SIZE_HEAPSORT 30

namespace OpenUtility
{

template <class T>
class CHeapSort
{
public:
	CHeapSort(int size=DEF_SIZE_HEAPSORT);
	~CHeapSort();
	void Sort(T** List,int nbElmt,bool (*CmpMin)(T* Obj1,T* Obj2));
	void Sort(T** List,int nbElmt);
	void Sort(T* List,int nbElmt);
	void Sort(T** List,int nbElmt,bool (T::*CmpMin)(T* Obj)=NULL);
	void Sort(IContener<T> &List,int nbElmt,bool (*CmpMin)(T* Obj1,T* Obj2));
	void Sort(IContener<T> &List,int nbElmt);
	inline void Sort(IContener<T> &List,bool (*CmpMin)(T* Obj1,T* Obj2)) {Sort(List,List.GetSize(),CmpMin);}
	inline void Sort(IContener<T> &List) {Sort(List,List.GetSize());}
	T* GetMin(unsigned int IndElmt);
	T* GetMax(unsigned int IndElmt);
	inline unsigned int GetSize() {return(TabTemp.GetSize());}
	inline void DeleteAll() {TabTemp.DeleteAll();}

private:
	void RemoveAll();
	void AddElmtOp(T* Obj);
	void AddElmtFn(T* Obj);
	T* DelElmtOp();
	T* DelElmtFn();

private:
	T *m_pClass;
	bool (*fnCompMinor)(T* Obj1,T* Obj2);
	CVector<T> TabTemp;
	int nbTabTempDel;

	bool Sorted;
};

}

#include "Res/CHeapsort.cxx"

#endif
