#ifndef Table_h
	#define Table_h

#include "../Utility/OUException.h"
#include "../Utility/Memory.h"

#define DEF_SIZE_Table 30

namespace OpenUtility
{

template <class T>
class CTable
{
public:
	class Exception : public OpenUtility::Exception
	{
	public:
		enum EError
		{
			EErrOutOfBounds
		};

	public:
		Exception(EError err,const char *detail=NULL) :
			OpenUtility::Exception(),
			ErrType(err)
		{
			CStream Err;
			switch(ErrType)
			{
			case EErrOutOfBounds:Err+="Out of bounds";break;
			}
			if (detail) Err.AddFormatStream(" (%s)",detail);
			SetDetail(Err.GetStream());
		}
		Exception(const Exception &obj) : OpenUtility::Exception(obj),ErrType(obj.ErrType) {}
		~Exception() throw() {}
		inline EError GetError() {return(ErrType);}

	private:
		EError ErrType;
	};

public:
	CTable(unsigned int size=DEF_SIZE_Table);
	~CTable();
	inline void SetGrowSize(unsigned int size) {IncBlock=size;}
	void Init(T obj,unsigned int nb);
	T& Add(T obj);
	inline T& Push(T obj) {return(Add(obj));}
	T Pop();
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
