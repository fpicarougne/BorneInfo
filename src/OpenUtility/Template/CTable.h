#ifndef Table_h
	#define Table_h

#define DEF_SIZE_Table 30

#include <Utility/Memory.h>

namespace OpenUtility
{

template <class T>
class CTable
{
public:
	CTable(unsigned int size=DEF_SIZE_Table);
	~CTable();
	inline void SetGrowSize(unsigned int size) {IncBlock=size;}
	void Init(T &Obj,unsigned int nb);
	T& Add(T &Obj);
	void Delete(unsigned int nb);
	void DeleteAll();
	inline unsigned int GetSize() {return(Taille);}
	inline T* GetTable() const {return(Table);}
	T& ElementAt(unsigned int index);	//Bon pour lvalue et rvalue
	T GetAt(unsigned int index) const;	//Bon uniquement pour lvalue
	inline T& operator[](unsigned int index) {return(ElementAt(index));}	//Bon pour lvalue et rvalue
	inline T operator[](unsigned int index) const {return(GetAt(index));}	//Bon uniquement pour lvalue

private:
	inline void SetSize() {Table=(T*)MyRealloc(Table,TailleBlock*sizeof(T));}

private:
	T* Table;
	unsigned int IncBlock;
	unsigned int TailleBlock;
	unsigned int Taille;
};

}

#include "Res/CTable.cxx"

#endif
