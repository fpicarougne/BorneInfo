template <class T,class F> OpenUtility::CHeap<T,F>::CHeap(bool opDec,int size) : TabTemp(size)
{
	pFnClass=NULL;
	fnCmp=NULL;
	fnCmpObj=NULL;
	DecOp=opDec;
}

template <class T,class F> OpenUtility::CHeap<T,F>::CHeap(bool (F::*Cmp)(T* Obj1,T* Obj2),F *pClass,int size) : TabTemp(size)
{
	pFnClass=pClass;
	fnCmpObj=Cmp;
	fnCmp=NULL;
	DecOp=false;
}

template <class T,class F> OpenUtility::CHeap<T,F>::CHeap(bool (*Cmp)(T* Obj1,T* Obj2),int size) : TabTemp(size)
{
	pFnClass=NULL;
	fnCmpObj=NULL;
	fnCmp=Cmp;
	DecOp=false;
}

template <class T,class F> OpenUtility::CHeap<T,F>::~CHeap()
{
}

template <class T,class F> void OpenUtility::CHeap<T,F>::AddElmt(T* Obj)
{
	int i;

	TabTemp.Add(Obj);
	i=TabTemp.GetSize();
	while ((i>1) && (fnCmp ? ((*fnCmp)(TabTemp[i-1],TabTemp[i/2-1])) : (fnCmpObj ? ((pFnClass->*fnCmpObj)(TabTemp[i-1],TabTemp[i/2-1])) : (DecOp ? (*(TabTemp[i-1])>*(TabTemp[i/2-1])) : (*(TabTemp[i-1])<*(TabTemp[i/2-1]))))))
	{
		TabTemp.Swap(i-1,i/2-1);
		i/=2;
	}
}

template <class T,class F> T* OpenUtility::CHeap<T,F>::DelElmt()
{
	T *extremum;
	unsigned int i,j,l;
	bool fini;

	extremum=TabTemp[0];
	l=TabTemp.GetSize()-1;
	TabTemp[0]=TabTemp[l];
	TabTemp.Remove(1);
	i=1;
	fini=false;
	while ((i<=l/2) && (!fini))
	{
		if ((2*i==l) || (fnCmp ? ((*fnCmp)(TabTemp[2*i-1],TabTemp[2*i])) : (fnCmpObj ? ((pFnClass->*fnCmpObj)(TabTemp[2*i-1],TabTemp[2*i])) : (DecOp ? (*(TabTemp[2*i-1])>*(TabTemp[2*i])) : (*(TabTemp[2*i-1])<*(TabTemp[2*i])))))) j=2*i;
		else j=2*i+1;
		if (fnCmp ? ((*fnCmp)(TabTemp[j-1],TabTemp[i-1])) : (fnCmpObj ? ((pFnClass->*fnCmpObj)(TabTemp[j-1],TabTemp[i-1])) : (DecOp ? (*(TabTemp[j-1])>*(TabTemp[i-1])) : (*(TabTemp[j-1])<*(TabTemp[i-1])))))
		{
			TabTemp.Swap(i-1,j-1);
			i=j;
		}
		else fini=true;
	}
	return(extremum);
}
