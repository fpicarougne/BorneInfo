#include <math.h>
#include <stdlib.h>
#include "../../Utility/Memory.h"

template<class T> OpenUtility::CListe<T>::CListe() :
	Liste(NULL),
	Taille(0),
	EnCours(NULL),
	PosEnCours(0)
{
}

template<class T> OpenUtility::CListe<T>::~CListe()
{
	RemoveAll();
}

template<class T> T* OpenUtility::CListe<T>::Add(T* Obj,CListeIterator *pos)
{
	if (Liste)
	{
		SCell *temp;

		temp=Liste->prev;
		temp->suiv=new SCell;
		Liste->prev=temp->suiv;
		Liste->prev->suiv=Liste;
		Liste->prev->prev=temp;
	}
	else
	{
		Liste=new SCell;
		Liste->prev=Liste;
		Liste->suiv=Liste;
	}
	Liste->prev->val=Obj;
	PosEnCours=Taille;
	EnCours=Liste->prev;
	Taille++;

	if (pos) pos->pos=EnCours;

	return(EnCours->val);
}

template<class T> T* OpenUtility::CListe<T>::AddFirst(T *Obj,CListeIterator *pos)
{
	if (Liste)
	{
		SCell *temp;

		temp=Liste->prev;
		temp->suiv=new SCell;
		Liste->prev=temp->suiv;
		Liste->prev->suiv=Liste;
		Liste->prev->prev=temp;
		Liste=Liste->prev;
	}
	else
	{
		Liste=new SCell;
		Liste->prev=Liste;
		Liste->suiv=Liste;
	}
	Liste->val=Obj;
	PosEnCours=1;
	EnCours=Liste;
	Taille++;

	if (pos) pos->pos=EnCours;

	return(EnCours->val);
}

template<class T> T* OpenUtility::CListe<T>::AddIncrease(T* Obj,CListeIterator *pos)
{
	SCell *temp=Liste,*temp2;
	unsigned int i=0;

	if (Liste)
	{
		while ((i<Taille) && (*(temp->val)<*Obj))
		{
			temp=temp->suiv;
			i++;
		}
		temp2=temp->prev;
		temp->prev=new SCell;
		temp->prev->suiv=temp;
		temp->prev->prev=temp2;
		temp2->suiv=temp->prev;
		if (!i) Liste=temp->prev;
		PosEnCours=i;
		EnCours=temp->prev;
	}
	else
	{
		Liste=new SCell;
		Liste->prev=Liste;
		Liste->suiv=Liste;
		PosEnCours=0;
		EnCours=Liste;
	}
	EnCours->val=Obj;
	Taille++;

	if (pos) pos->pos=EnCours;

	return(EnCours->val);
}

template<class T> T* OpenUtility::CListe<T>::AddDecrease(T* Obj,CListeIterator *pos)
{
	SCell *temp=Liste,*temp2;
	int i=0;

	if (Liste)
	{
		while ((i<Taille) && (*Obj<*(temp->val)))
		{
			temp=temp->suiv;
			i++;
		}
		temp2=temp->prev;
		temp->prev=new SCell;
		temp->prev->suiv=temp;
		temp->prev->prev=temp2;
		temp2->suiv=temp->prev;
		if (!i) Liste=temp->prev;
		PosEnCours=i;
		EnCours=temp->prev;
	}
	else
	{
		Liste=new SCell;
		Liste->prev=Liste;
		Liste->suiv=Liste;
		PosEnCours=0;
		EnCours=Liste;
	}
	EnCours->val=Obj;
	Taille++;

	if (pos) pos->pos=EnCours;

	return(EnCours->val);
}

template<class T> T* OpenUtility::CListe<T>::AddAfter(T *Obj,CListeIterator &posAfter,CListeIterator *pos)
{
	SCell *temp,*cell;
	
	if (posAfter.pos) cell=posAfter.pos;
	else if (Liste) cell=Liste;
	else return(NULL);

	temp=cell->suiv;
	cell->suiv=new SCell;
	temp->prev=cell->suiv;
	cell->suiv->suiv=temp;
	cell->suiv->prev=cell;
	cell->suiv->val=Obj;
	Taille++;

	if (cell==EnCours)
	{
		EnCours=cell->suiv;
		PosEnCours++;
	}
	else
	{
		EnCours=Liste;
		PosEnCours=0;
	}

	if (pos) pos->pos=cell->suiv;

	return(Obj);
}

template<class T> T* OpenUtility::CListe<T>::Remove(unsigned int index)
{
	SCell *temp;
	T *ObjRetour;
	int i;
	bool sensGauche;

	if (index>=Taille)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	if (index<Taille-index)
	{
		if (abs(((int)index)-((int)PosEnCours))<(int)index)
		{
			temp=EnCours;
			i=PosEnCours;
			sensGauche=i-index>0;
		}
		else
		{
			temp=Liste;
			i=0;
			sensGauche=false;
		}
	}
	else
	{
		if (abs(((int)index)-((int)PosEnCours))<(int)(Taille-index))
		{
			temp=EnCours;
			i=PosEnCours;
			sensGauche=i-index>0;
		}
		else
		{
			temp=Liste;
			i=0;
			sensGauche=true;
		}
	}

	while (i!=(int)index)
	{
		if (sensGauche)
		{
			temp=temp->prev;
			i--;
			if (i<0) i=Taille-1;
		}
		else
		{
			temp=temp->suiv;
			i++;
		}
	}

	temp->prev->suiv=temp->suiv;
	temp->suiv->prev=temp->prev;
	if (EnCours==temp)
	{
		EnCours=Liste;
		PosEnCours=0;
	}
	else
	{
		if (index>PosEnCours) PosEnCours--;	// TODO: peut-�re une erreur : index<PosEnCours
	}
	ObjRetour=temp->val;
	if (temp==Liste)
	{
		if (Taille==1)
		{
			delete temp;
			Liste=NULL;
		}
		else
		{
			Liste=Liste->suiv;
			delete temp;
		}
	}
	else delete temp;
	Taille--;
	return(ObjRetour);
}

template<class T> T* OpenUtility::CListe<T>::Remove(CListeIterator &pos)
{
	SCell *temp;
	T *ObjRetour;

	temp=pos.pos;
	if (Taille<1)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	temp->prev->suiv=temp->suiv;
	temp->suiv->prev=temp->prev;
	if (Taille==1) Liste=NULL;
	else
	{
		if (temp==Liste) Liste=temp->suiv;
		EnCours=Liste;
		PosEnCours=0;
	}

	Taille--;
	ObjRetour=temp->val;
	delete temp;
	return(ObjRetour);
}

template<class T> void OpenUtility::CListe<T>::RemoveAll()
{
	for(unsigned int i=1;i<Taille;i++)
	{
		Liste=Liste->suiv;
		delete Liste->prev;
	}
	delete Liste;
	Liste=NULL;
	Taille=0;
	EnCours=NULL;
	PosEnCours=0;
}

template<class T> T* OpenUtility::CListe<T>::RemoveFirst()
{
	SCell *temp;
	T *ObjRetour;

	if (Taille<1)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	Liste->prev->suiv=Liste->suiv;
	Liste->suiv->prev=Liste->prev;
	if (EnCours==Liste)
	{
		EnCours=Liste->suiv;
		PosEnCours=0;
	}
	else
	{
		PosEnCours--;
	}

	temp=Liste;
	if (Taille==1) Liste=NULL;
	else Liste=Liste->suiv;

	Taille--;
	ObjRetour=temp->val;
	delete temp;
	return(ObjRetour);
}

template<class T> T* OpenUtility::CListe<T>::RemoveLast()
{
	T *ObjRetour;
	SCell *temp;

	if (Taille<1)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	temp=Liste->prev;

	temp->prev->suiv=Liste;
	Liste->prev=temp->prev;
	if (EnCours==temp)
	{
		EnCours=Liste;
		PosEnCours=0;
	}

	ObjRetour=temp->val;
	if (Taille==1)
	{
		delete Liste;
		Liste=NULL;
	}
	else delete temp;
	Taille--;
	return(ObjRetour);
}

template<class T> void OpenUtility::CListe<T>::Delete(CListeIterator &pos)
{
	SCell *temp;

	temp=pos.pos;
	if (Taille<1 || !temp)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	temp->prev->suiv=temp->suiv;
	temp->suiv->prev=temp->prev;
	if (Taille==1) Liste=NULL;
	else
	{
		if (temp==Liste) Liste=temp->suiv;
		EnCours=Liste;
		PosEnCours=0;
	}

	Taille--;
	delete temp->val;
	delete temp;
}

template<class T> void OpenUtility::CListe<T>::Delete(unsigned int index)
{
	SCell *temp;
	int i;
	bool sensGauche;

	if (index>=Taille)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	if (index<Taille-index)
	{
		if (abs(((int)index)-((int)PosEnCours))<(int)index)
		{
			temp=EnCours;
			i=PosEnCours;
			sensGauche=i-index>0;
		}
		else
		{
			temp=Liste;
			i=0;
			sensGauche=false;
		}
	}
	else
	{
		if (abs(((int)index)-((int)PosEnCours))<(int)(Taille-index))
		{
			temp=EnCours;
			i=PosEnCours;
			sensGauche=i-index>0;
		}
		else
		{
			temp=Liste;
			i=0;
			sensGauche=true;
		}
	}

	while (i!=(int)index)
	{
		if (sensGauche)
		{
			temp=temp->prev;
			i--;
			if (i<0) i=Taille-1;
		}
		else
		{
			temp=temp->suiv;
			i++;
		}
	}

	temp->prev->suiv=temp->suiv;
	temp->suiv->prev=temp->prev;
	if (EnCours==temp)
	{
		EnCours=Liste;
		PosEnCours=0;
	}
	else
	{
		if (index>PosEnCours) PosEnCours--;
	}
	if (temp==Liste)
	{
		if (Taille==1)
		{
			delete temp->val;
			delete temp;
			Liste=NULL;
		}
		else
		{
			Liste=Liste->suiv;
			delete temp->val;
			delete temp;
		}
	}
	else
	{
		delete temp->val;
		delete temp;
	}
	Taille--;
}

template<class T> void OpenUtility::CListe<T>::DeleteAll()
{
	for(unsigned int i=1;i<Taille;i++)
	{
		Liste=Liste->suiv;
		delete Liste->prev->val;
		delete Liste->prev;
	}
	delete Liste;
	Liste=NULL;
	Taille=0;
	EnCours=NULL;
	PosEnCours=0;
}

template<class T> void OpenUtility::CListe<T>::DeleteFirst()
{
	SCell *temp;

	if (Taille<1)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	Liste->prev->suiv=Liste->suiv;
	Liste->suiv->prev=Liste->prev;
	if (EnCours==Liste)
	{
		EnCours=Liste->suiv;
		PosEnCours=0;
	}
	else
	{
		PosEnCours--;
	}

	if (Taille==1)
	{
		delete Liste->val;
		delete Liste;
		Liste=NULL;
	}
	else
	{
		temp=Liste;
		Liste=Liste->suiv;
		delete temp->val;
		delete temp;
	}
	Taille--;
}

template<class T> void OpenUtility::CListe<T>::DeleteLast()
{
	SCell *temp;

	if (Taille<1)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	temp=Liste->prev;

	temp->prev->suiv=Liste;
	Liste->prev=temp->prev;
	if (EnCours==temp)
	{
		EnCours=Liste;
		PosEnCours=0;
	}

	if (Taille==1)
	{
		delete Liste->val;
		delete Liste;
		Liste=NULL;
	}
	else
	{
		delete temp->val;
		delete temp;
	}
	Taille--;
}

template<class T> T*& OpenUtility::CListe<T>::ElementAt(unsigned int index,CListeIterator *pos)
{
	SCell *temp;
	int i;
	bool sensGauche;

	if (index>=Taille)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	if (index<Taille-index)
	{
		if (abs(((int)index)-((int)PosEnCours))<(int)index)
		{
			temp=EnCours;
			i=PosEnCours;
			sensGauche=i-index>0;
		}
		else
		{
			temp=Liste;
			i=0;
			sensGauche=false;
		}
	}
	else
	{
		if (abs(((int)index)-((int)PosEnCours))<(int)(Taille-index))
		{
			temp=EnCours;
			i=PosEnCours;
			sensGauche=i-index>0;
		}
		else
		{
			temp=Liste;
			i=0;
			sensGauche=true;
		}
	}

	while (i!=(int)index)
	{
		if (sensGauche)
		{
			temp=temp->prev;
			i--;
			if (i<0) i=Taille-1;
		}
		else
		{
			temp=temp->suiv;
			i++;
		}
	}

	EnCours=temp;
	PosEnCours=i;

	if (pos) pos->pos=EnCours;

	return(temp->val);
}

template<class T> T* OpenUtility::CListe<T>::GetAt(unsigned int index,CListeIterator *pos) const
{
	SCell *temp;
	int i;
	bool sensGauche;

	if (index>=Taille)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	if (index<Taille-index)
	{
		if (abs(((int)index)-((int)PosEnCours))<(int)index)
		{
			temp=EnCours;
			i=PosEnCours;
			sensGauche=i-index>0;
		}
		else
		{
			temp=Liste;
			i=0;
			sensGauche=false;
		}
	}
	else
	{
		if (abs(((int)index)-((int)PosEnCours))<(int)(Taille-index))
		{
			temp=EnCours;
			i=PosEnCours;
			sensGauche=i-index>0;
		}
		else
		{
			temp=Liste;
			i=0;
			sensGauche=true;
		}
	}

	while (i!=(int)index)
	{
		if (sensGauche)
		{
			temp=temp->prev;
			i--;
			if (i<0) i=Taille-1;
		}
		else
		{
			temp=temp->suiv;
			i++;
		}
	}

	if (pos) pos->pos=temp;

	return(temp->val);
}

template<class T> T* OpenUtility::CListe<T>::GetNextElmt(CListeIterator *pos)
{
	if (!Taille)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	if (pos)
	{
		if (!pos->pos) pos->pos=Liste;
		else pos->pos=pos->pos->suiv;
		return(pos->pos->val);
	}
	return(Liste->val);
}

template<class T> T* OpenUtility::CListe<T>::GetPrevElmt(CListeIterator *pos)
{
	if (!Taille)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	if (pos)
	{
		if (!pos->pos) pos->pos=Liste;
		else pos->pos=pos->pos->prev;
		return(pos->pos->val);
	}
	return(Liste->val);
}

template<class T> bool OpenUtility::CListe<T>::FindObj(T &ObjToFind,T** ObjFound,unsigned int *index)
{
	SCell *CurElmt=Liste;
	bool fini=false;
	unsigned int i=0;

	while ((!fini) && (i<Taille))
	{
		if (*(CurElmt->val)==ObjToFind) fini=true;
		else
		{
			i++;
			CurElmt=CurElmt->suiv;
		}
	}
	if (fini)
	{
		*ObjFound=CurElmt->val;
		*index=i;
		EnCours=CurElmt;
		PosEnCours=i;
	}
	return(fini);
}

template<class T> bool OpenUtility::CListe<T>::FindObj(T &ObjToFind)
{
	SCell *CurElmt=Liste;
	bool fini=false;
	unsigned int i=0;

	while ((!fini) && (i<Taille))
	{
		if (*(CurElmt->val)==ObjToFind) fini=true;
		else
		{
			i++;
			CurElmt=CurElmt->suiv;
		}
	}
	if (fini)
	{
		EnCours=CurElmt;
		PosEnCours=i;
	}
	return(fini);
}

template<class T> void OpenUtility::CListe<T>::Loop()
{
	if (!Taille)
	{
		GetCMyExceptionObj(E,ERR_ARGUMENT);
		throw(E);
	}

	if (Taille!=1)
	{
		Liste=Liste->suiv;
		if (PosEnCours) PosEnCours--;
		else PosEnCours=Taille-1;
	}
}

template<class T> void OpenUtility::CListe<T>::CListeIterator::operator+=(int i)
{
	if (pos)
	{
		if (i<0) for (;i!=0;i++) pos=pos->prev;
		else  for (;i!=0;i--) pos=pos->suiv;
	}
}
