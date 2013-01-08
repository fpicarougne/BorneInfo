#include "../../Utility/Memory.h"

template <class T> OpenUtility::CVector<T>::CVector(unsigned int size)
{
	IncBlock=size;
	TailleBlock=0;
	Taille=0;
	Vecteur=NULL;
}

template <class T> OpenUtility::CVector<T>::~CVector()
{
	RemoveAll();
}

template <class T> void OpenUtility::CVector<T>::Init(T Obj,unsigned int nb,bool Delete)
{
	unsigned int size,i;

	size=((nb/IncBlock)+1)*IncBlock;
	if (Delete)
	{
		DeleteAll();
		TailleBlock=size;
		Vecteur=(T**)MyRealloc(Vecteur,TailleBlock*sizeof(T*));
	}
	else
	{
		if (size!=TailleBlock)
		{
			TailleBlock=size;
			Vecteur=(T**)MyRealloc(Vecteur,TailleBlock*sizeof(T*));
		}
	}
	Taille=nb;
	for (i=0;i<nb;i++) Vecteur[i]=new T(Obj);
}

template <class T> void OpenUtility::CVector<T>::Init(T *pObj,unsigned int nb,bool Delete)
{
	unsigned int size,i;

	size=((nb/IncBlock)+1)*IncBlock;
	if (Delete)
	{
		DeleteAll();
		TailleBlock=size;
		Vecteur=(T**)MyRealloc(Vecteur,TailleBlock*sizeof(T*));
	}
	else
	{
		if (size!=TailleBlock)
		{
			TailleBlock=size;
			Vecteur=(T**)MyRealloc(Vecteur,TailleBlock*sizeof(T*));
		}
	}
	Taille=nb;
	for (i=0;i<nb;i++) Vecteur[i]=pObj;
}

template <class T> void OpenUtility::CVector<T>::Init(unsigned int nb,bool Delete)
{
	unsigned int size,i;

	size=((nb/IncBlock)+1)*IncBlock;
	if (Delete)
	{
		DeleteAll();
		TailleBlock=size;
		Vecteur=(T**)MyRealloc(Vecteur,TailleBlock*sizeof(T*));
	}
	else
	{
		if (size!=TailleBlock)
		{
			TailleBlock=size;
			Vecteur=(T**)MyRealloc(Vecteur,TailleBlock*sizeof(T*));
		}
	}
	Taille=nb;
	for (i=0;i<nb;i++) Vecteur[i]=new T;
}

template <class T> void OpenUtility::CVector<T>::InitNULL(unsigned int nb,bool Delete)
{
	unsigned int size,i;

	size=((nb/IncBlock)+1)*IncBlock;
	if (Delete)
	{
		DeleteAll();
		TailleBlock=size;
		Vecteur=(T**)MyRealloc(Vecteur,TailleBlock*sizeof(T*));
	}
	else
	{
		if (size!=TailleBlock)
		{
			TailleBlock=size;
			Vecteur=(T**)MyRealloc(Vecteur,TailleBlock*sizeof(T*));
		}
	}
	Taille=nb;
	memset(Vecteur,0,Taille*sizeof(T*));
}

template <class T> T* OpenUtility::CVector<T>::Add(T* Obj)
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
		Vecteur=(T**)MyRealloc(Vecteur,TailleBlock*sizeof(T*));
	}
	Vecteur[Taille++]=Obj;
	return(Obj);
}

template <class T> void OpenUtility::CVector<T>::Remove(unsigned int nb)
{
	if (nb>Taille)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	if (Taille-nb<TailleBlock/3)
	{
		TailleBlock=(((Taille-nb)/IncBlock)+1)*IncBlock;
		Vecteur=(T**)MyRealloc(Vecteur,TailleBlock*sizeof(T*));
	}
	Taille-=nb;
}

template <class T> void OpenUtility::CVector<T>::RemoveAll()
{
	MyFree(Vecteur);
	Vecteur=NULL;
	TailleBlock=0;
	Taille=0;
}

template <class T> void OpenUtility::CVector<T>::Delete(unsigned int nb)
{
	if (nb>Taille)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	for (unsigned int i=1;i<=nb;i++) delete Vecteur[Taille-i];
	if (Taille-nb<TailleBlock/3)
	{
		TailleBlock=(((Taille-nb)/IncBlock)+1)*IncBlock;
		Vecteur=(T**)MyRealloc(Vecteur,TailleBlock*sizeof(T*));
	}
	Taille-=nb;
}

template <class T> void OpenUtility::CVector<T>::DeleteAll()
{
	for (unsigned int i=0;i<Taille;i++)
		delete Vecteur[i];
	MyFree(Vecteur);
	Vecteur=NULL;
	TailleBlock=0;
	Taille=0;
}

template <class T> T* OpenUtility::CVector<T>::GetAt(unsigned int index) const
{
	if (index>=Taille)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	return(Vecteur[index]);
}

template <class T> T*& OpenUtility::CVector<T>::ElementAt(unsigned int index)
{
	if (index>=Taille)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	return(Vecteur[index]);
}

template <class T> void OpenUtility::CVector<T>::Swap(unsigned int index1,unsigned int index2)
{
	T* PtrT;

	if ((index1>=Taille) || (index2>=Taille))
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	if (index1!=index2)
	{
		PtrT=Vecteur[index1];
		Vecteur[index1]=Vecteur[index2];
		Vecteur[index2]=PtrT;
	}
}
