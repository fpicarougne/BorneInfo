#ifndef _CRBTree_h
	#define _CRBTree_h

#include "Interfaces/ISortedTree.h"
#include "CAllocator.h"
#include <iostream>

#include "CVector.h"

namespace OpenUtility
{

#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable : 4101)
#endif

template<class T>
class CRedBlackTree : public ISortedTree<T>
{
	friend class CNode;

public:
	struct CRedBlackTreeIterator : public OpenUtility::Iterator<T>
	{
		friend class CRedBlackTree<T>;
		CRedBlackTreeIterator() : pos(NULL) {}
		void operator++(int);
		void operator+=(int i);
		void operator--(int);
		inline void operator-=(int i) {operator+=(-i);}
		inline bool operator==(const OpenUtility::Iterator<T> &obj)
		{
			try
			{
				const CRedBlackTreeIterator &ref=dynamic_cast<const CRedBlackTreeIterator&>(obj);
				return(*this==ref);
			}
			catch(const std::bad_cast &e) {return(false);}
		}
		inline bool operator!=(const OpenUtility::Iterator<T> &obj) {return(!(*this==obj));}
		inline bool operator==(const CRedBlackTreeIterator &obj) {return(pos==obj.pos);}
		inline bool operator!=(const CRedBlackTreeIterator &obj) {return(pos!=obj.pos);}
		inline T* operator->() {return(pos ? pos->Value : NULL);}
		inline T& operator*() {return(*pos->Value);}
		inline bool isNull() {return(pos==NULL);}

	private:
		CRedBlackTreeIterator(CNode *init) : pos(init) {}

	private:
		CNode *pos;
	};

public:
	class CNode
	{
		friend class CRedBlackTree<T>;

	public:
		CNode(T *val,CNode *parent=NULL) : Value(val),isRed(true),Parent(parent),Right(NULL),Left(NULL) {}
		~CNode() {CRedBlackTree<T>::Allocator.Delete(Right);CRedBlackTree<T>::Allocator.Delete(Left);}
		friend inline std::ostream& operator<<(std::ostream &o,const CNode &node)
		{
			o << *node.Value << (node.isRed ? 'r' : 'b') << '[';
			if (node.Left) o << *node.Left;
			o << ',';
			if (node.Right) o << *node.Right;
			o << ']';
			return(o);
		}

	private:
		T *Value;
		bool isRed;
		CNode *Parent;
		CNode *Right;
		CNode *Left;
	};

public:
	inline CRedBlackTree() : Root(NULL) {}
	inline ~CRedBlackTree() {Allocator.Delete(Root);}
	bool Add(T *obj);
	inline bool Delete(T *obj) {delete(obj=Remove(obj));return(obj!=NULL);}
	T* Remove(T *obj);
	bool Exists(T *obj);
	friend inline std::ostream& operator<<(std::ostream &o,const CRedBlackTree<T> &tree)
	{
		o << '{';
		if (tree.Root) o << *tree.Root;
		o << '}';
		return(o);
	}
	CRedBlackTreeIterator GetMin();
	CRedBlackTreeIterator GetMax();
	bool Check();
	unsigned int GetMaxHight();
	void Draw();

private:
	CRedBlackTree(const CRedBlackTree &obj) {}
	CRedBlackTree& operator=(const CRedBlackTree &obj) {}
	CNode* Insert(T *obj);
	void Organize(CNode *node);
	void Rotate(CNode *node);
	bool CheckRecurs(CNode *node,unsigned int &nbBlack);
	unsigned int GetMaxHightRecurs(CNode *node);

private:
	CNode *Root;
	static CAllocator<CNode> Allocator;
};

#if defined(_MSC_VER)
	#pragma warning(pop)
#endif

#include "Res/CRBTree.cxx"

}

#endif
