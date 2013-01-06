#ifndef Vector_h
	#define Vector_h

#include "CContenerInterface.h"

#define DEF_SIZE_VECTOR 30

namespace OpenUtility
{

template <class T>
class CVector : public CContenerInterface<T>
{
public:
	CVector(unsigned int size=DEF_SIZE_VECTOR);
	~CVector();
	inline void SetGrowSize(unsigned int size) {IncBlock=size;}
	void Init(T Obj,unsigned int nb,bool Delete=true);
	void Init(T *pObj,unsigned int nb,bool Delete=true);
	void Init(unsigned int nb,bool Delete=true);
	void InitNULL(unsigned int nb,bool Delete=true);
	T* Add(T *Obj);
	void Remove(unsigned int nb);
	void RemoveAll();
	void Delete(unsigned int nb);
	void DeleteAll();
	inline unsigned int GetSize() const {return(Taille);}
	inline T** GetVector() const {return(Vecteur);}
	T*& ElementAt(unsigned int index);	//Bon pour lvalue et rvalue
	T* GetAt(unsigned int index) const;	//Bon uniquement pour lvalue
	inline T*& operator[](unsigned int index) {return(ElementAt(index));}	//Bon pour lvalue et rvalue
	inline T* operator[](unsigned int index) const {return(GetAt(index));}	//Bon uniquement pour lvalue
	void Swap(unsigned int index1,unsigned int index2);

private:
	T** Vecteur;
	unsigned int IncBlock;
	unsigned int TailleBlock;
	unsigned int Taille;
};

}

#include "Res/CVector.cxx"

#endif
