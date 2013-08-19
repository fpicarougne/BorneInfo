#include "../../Utility/Memory.h"

template <class T> OpenUtility::CHeapSort<T>::CHeapSort(int size)
{
	TabTemp.SetGrowSize(size);
	Sorted=false;
}

template <class T> OpenUtility::CHeapSort<T>::~CHeapSort()
{
}

template <class T> void OpenUtility::CHeapSort<T>::RemoveAll()
{
	TabTemp.RemoveAll();
	Sorted=false;
}

template <class T> void OpenUtility::CHeapSort<T>::AddElmtOp(T* Obj)
{
	int i;

	TabTemp.Add(Obj);
	i=nbTabTempDel=TabTemp.GetSize();
	while ((i>1) && (*(TabTemp[i-1])<*(TabTemp[i/2-1])))
	{
		TabTemp.Swap(i-1,i/2-1);
		i/=2;
	}
}

template <class T> void OpenUtility::CHeapSort<T>::AddElmtFn(T* Obj)
{
	int i;

	TabTemp.Add(Obj);
	i=nbTabTempDel=TabTemp.GetSize();
	while ((i>1) && (fnCompMinor(TabTemp[i-1],TabTemp[i/2-1])))
	{
		TabTemp.Swap(i-1,i/2-1);
		i/=2;
	}
}

template <class T> T* OpenUtility::CHeapSort<T>::DelElmtOp()
{
	T *extremum;
	int i,j;
	bool fini;

	extremum=TabTemp[0];
	TabTemp[0]=TabTemp[--nbTabTempDel];
	i=1;
	fini=false;
	while (!(i>nbTabTempDel/2) && (!fini))
	{
		if ((2*i==nbTabTempDel) || (*(TabTemp[2*i-1])<*(TabTemp[2*i]))) j=2*i;
		else j=2*i+1;
		if (*(TabTemp[j-1])<*(TabTemp[i-1]))
		{
			TabTemp.Swap(i-1,j-1);
			i=j;
		}
		else fini=true;
	}
	return(extremum);
}

template <class T> T* OpenUtility::CHeapSort<T>::DelElmtFn()
{
	T *extremum;
	int i,j;
	bool fini;

	extremum=TabTemp[0];
	TabTemp[0]=TabTemp[--nbTabTempDel];
	i=1;
	fini=false;
	while (!(i>nbTabTempDel/2) && (!fini))
	{
		if ((2*i==nbTabTempDel) || (fnCompMinor(TabTemp[2*i-1],TabTemp[2*i]))) j=2*i;
		else j=2*i+1;
		if (fnCompMinor(TabTemp[j-1],TabTemp[i-1]))
		{
			TabTemp.Swap(i-1,j-1);
			i=j;
		}
		else fini=true;
	}
	return(extremum);
}

/*template <class T> void OpenUtility::CHeapSort<T>::Sort(T** List,int nbElmt,bool (T::*CmpMin)(T* Obj))
{
}*/

template <class T> void OpenUtility::CHeapSort<T>::Sort(T** List,int nbElmt,bool (*CmpMin)(T* Obj1,T* Obj2))
{
	int i;

	RemoveAll();
	i=0;

	if (fnCompMinor=CmpMin)
	{
		while (i<nbElmt) AddElmtFn(List[i++]);
		i--;
		while (i>0) (TabTemp[i--])=DelElmtFn();
		Sorted=true;
	}
}

template <class T> void OpenUtility::CHeapSort<T>::Sort(T** List,int nbElmt)
{
	int i;

	RemoveAll();
	i=0;

	while (i<nbElmt) AddElmtOp(List[i++]);
	i--;
	while (i>0) (TabTemp[i--])=DelElmtOp();

	Sorted=true;
}

template <class T> void OpenUtility::CHeapSort<T>::Sort(T* List,int nbElmt)
{
	int i;

	RemoveAll();
	i=0;

	while (i<nbElmt) AddElmtOp(&(List[i++]));
	i--;
	while (i>0) (TabTemp[i--])=DelElmtOp();

	Sorted=true;
}

template <class T> void OpenUtility::CHeapSort<T>::Sort(IContener<T> &List,int nbElmt,bool (*CmpMin)(T* Obj1,T* Obj2))
{
	int i;

	RemoveAll();
	i=0;

	if (fnCompMinor=CmpMin)
	{
		while (i<nbElmt) AddElmtFn(List[i++]);
		i--;
		while (i>0) (TabTemp[i--])=DelElmtFn();
		Sorted=true;
	}
}

template <class T> void OpenUtility::CHeapSort<T>::Sort(IContener<T> &List,int nbElmt)
{
	int i;

	RemoveAll();
	i=0;

	while (i<nbElmt) AddElmtOp(List[i++]);
	i--;
	while (i>0) (TabTemp[i--])=DelElmtOp();

	Sorted=true;
}

template <class T> T* OpenUtility::CHeapSort<T>::GetMin(unsigned int IndElmt)
{
	if (!Sorted)
	{
		CMyException E(ERR_INIT);
		throw(E);
	}
	return(TabTemp[TabTemp.GetSize()-1-IndElmt]);
}

template <class T> T* OpenUtility::CHeapSort<T>::GetMax(unsigned int IndElmt)
{
	if (!Sorted)
	{
		CMyException E(ERR_INIT);
		throw(E);
	}
	return(TabTemp[IndElmt]);
}
