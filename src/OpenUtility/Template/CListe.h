#ifndef CListe_h
	#define CListe_h

#ifndef NULL
	#define NULL 0
#endif

#include "CContenerInterface.h"

namespace OpenUtility
{

template <class T>
class CListe : public CContenerInterface<T>
{
private:
	struct SCell
	{
		T *val;
		SCell *prev;
		SCell *suiv;
	};

public:
	struct CListeIterator : public OpenUtility::Iterator<T>
	{
		friend class CListe<T>;
		CListeIterator() : pos(NULL) {}
		inline void operator++() {if (pos) pos=pos->suiv;}
		void operator+=(int i);
		inline void operator--() {if (pos) pos=pos->prev;}
		inline void operator-=(int i) {operator+=(-i);}
		inline bool operator==(const CListeIterator &obj) {return(pos==obj.pos);}
		inline bool operator!=(const CListeIterator &obj) {return(pos!=obj.pos);}
		inline T* operator->() {return(pos ? pos->val : NULL);}
		inline T& operator*() {return(*pos->val);}
		inline bool isNull() {return(pos==NULL);}

	private:
		SCell *pos;
	};

public:
	CListe();
	~CListe();
	T* Add(T *Obj,CListeIterator *pos=NULL);	// Insère à la fin
	inline T& Add(T &Obj,CListeIterator *pos=NULL) {return(*Add(&Obj,pos));}
	T* AddFirst(T *Obj,CListeIterator *pos=NULL);	// Insère au début
	inline T& AddFirst(T &Obj,CListeIterator *pos=NULL) {return(*AddFirst(&Obj,pos));}
	T* AddIncrease(T *Obj,CListeIterator *pos=NULL);	// Insère en classant par ordre >
	inline T& AddIncrease(T &Obj,CListeIterator *pos=NULL) {return(*AddIncrease(&Obj,pos));}
	T* AddDecrease(T *Obj,CListeIterator *pos=NULL);	// Insère en classant par ordre <
	inline T& AddDecrease(T &Obj,CListeIterator *pos=NULL) {return(*AddDecrease(&Obj,pos));}
	T* AddAfter(T *Obj,CListeIterator &posAfter,CListeIterator *pos=NULL);	// Insère à la suite d'un élément
	inline T& AddAfter(T &Obj,CListeIterator &posAfter,CListeIterator *pos=NULL) {return(*AddAfter(&Obj,posAfter,pos));}
	T* Remove(CListeIterator &pos);
	T* Remove(unsigned int index);
	T* RemoveFirst();
	T* RemoveLast();
	void RemoveAll();
	void Delete(CListeIterator &pos);
	void Delete(unsigned int index);
	void DeleteFirst();
	void DeleteLast();
	void DeleteAll();
	void Loop();
	inline unsigned int GetSize() const {return(Taille);}
	T*& ElementAt(unsigned int index,CListeIterator *pos);	//Bon pour lvalue et rvalue
	inline T*& ElementAt(unsigned int index) {return(ElementAt(index,NULL));};
	inline T*& ElementAt(CListeIterator &pos) {return(pos.pos->val);}	//Bon pour lvalue et rvalue
	T* GetAt(unsigned int index,CListeIterator *pos) const;	//Bon uniquement pour lvalue
	T* GetAt(unsigned int index) const {return(GetAt(index,NULL));}
	inline T* GetAt(CListeIterator pos) const {return(pos.pos->val);};	//Bon uniquement pour lvalue
	inline T*& operator[](unsigned int index) {return(ElementAt(index));}	//Bon pour lvalue et rvalue
	inline T* operator[](unsigned int index) const {return(GetAt(index));}	//Bon uniquement pour lvalue
	inline T*& operator[](CListeIterator &pos) {return(ElementAt(pos));}	//Bon pour lvalue et rvalue
	inline T* operator[](CListeIterator &pos) const {return(GetAt(pos));}	//Bon uniquement pour lvalue
	T* GetNextElmt(CListeIterator *pos=NULL);
	T* GetPrevElmt(CListeIterator *pos=NULL);
	bool FindObj(T &ObjToFind,T** ObjFound,unsigned int *index);
	bool FindObj(T &ObjToFind);


private:
	SCell *Liste;	// Le premier élément de la liste
	unsigned long Taille;	// La taille de la liste
	SCell *EnCours;
	unsigned long PosEnCours;
};
}

#include "Res/CListe.cxx"

#endif
