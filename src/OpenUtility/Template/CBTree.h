#ifndef _CBTree_h
	#define _CBTree_h

#include "Interfaces/ISortedTree.h"
#include <memory.h>

namespace OpenUtility
{

template<class T,int I>
class CBTree : public ISortedTree<T>
{
	friend class CNode;

private:
	class CNode
	{
		friend class CBTree<T,I>;

	public:
		CNode(CNode *parent=NULL);
		~CNode() {for(unsigned int i=0;i<I*2;i++) CBTree<T,I>::Allocator.Delete(Childs[i]);}

	private:
		T *Values[I*2-1];
		CNode *Parent;
		CNode *Childs[I*2];
	};

public:
	inline CBTree() : Root(NULL) {}
	inline ~CBTree() {Allocator.Delete(Root);}
	bool Add(T *obj);
	T* Remove(T *obj);
	inline bool Delete(T *obj) {delete(obj=Remove(obj));return(obj!=NULL);}
	bool Exists(T *obj);

private:
	CNode *Root;
	static CAllocator<CNode> Allocator;
};

template<class T,int I>
class CBPlusTree : public ISortedTree<T>
{
private:
	class CNode
	{
		friend class CBPlusTree<T,I>;

	public:
		CNode() {}
		virtual ~CNode() {}

	private:
		T *Values[I*2];
		CNode *Childs[I*2+1];
	};

public:
	bool Add(T *obj);
	T* Remove(T *obj);
	inline bool Delete(T *obj) {delete(obj=Remove(obj));return(obj!=NULL);}
	bool Exists(T *obj);

private:
	CNode *Root;
};

#include "Res/CBTree.cxx"

}

#endif
