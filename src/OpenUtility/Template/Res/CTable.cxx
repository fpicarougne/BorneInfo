#include "../../Utility/Memory.h"

template <class T> OpenUtility::CTable<T>::CTable(unsigned int size) :
	Table(NULL),
	IncBlock(size)
{
	TailleBlock=IncBlock;
	Taille=0;
	SetSize();
}

template <class T> OpenUtility::CTable<T>::~CTable()
{
	MyFree(Table);
}

template <class T> void OpenUtility::CTable<T>::Init(T obj,unsigned int nb)
{
	unsigned int size,i;

	size=((nb/IncBlock)+1)*IncBlock;
	if (size!=TailleBlock)
	{
		TailleBlock=size;
		SetSize();
	}
	Taille=nb;
	for (i=0;i<nb;i++) Table[i]=obj;
}

template <class T> T& OpenUtility::CTable<T>::Add(T obj)
{
	if (Taille+1>TailleBlock)
	{
		if (IncBlock) TailleBlock+=IncBlock;
		else	// Heuristique piquée aux MFC
		{
			int Inc;
			Inc=(Taille+1) / 8;
			Inc=(Inc<4) ? 4 : ((Inc>1024) ? 1024 : Inc);
			TailleBlock+=Inc;
		}
		SetSize();
	}
	Table[Taille++]=obj;
	return(Table[Taille-1]);
}

template <class T> T OpenUtility::CTable<T>::Pop()
{
	if (Taille==0)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}
	T obj=Table[Taille-1];
	Delete(1);
	return(obj);
}

template <class T> void OpenUtility::CTable<T>::Delete(unsigned int nb)
{
	if (nb>Taille)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	if (Taille-nb<TailleBlock/3)
	{
		TailleBlock=(((Taille-nb)/IncBlock)+1)*IncBlock;
		SetSize();
	}
	Taille-=nb;
}

template <class T> void OpenUtility::CTable<T>::DeleteAll()
{
	if (TailleBlock!=IncBlock)
	{
		TailleBlock=IncBlock;
		SetSize();
	}
	Taille=0;
}

template <class T> T OpenUtility::CTable<T>::GetAt(unsigned int index) const
{
	if (index>=Taille)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	return(Table[index]);
}

template <class T> T& OpenUtility::CTable<T>::ElementAt(unsigned int index)
{
	if (index>=Taille)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	return(Table[index]);
}
