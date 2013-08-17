#ifndef _CRBTree_h
	#define _CRBTree_h

#include "Interfaces/ISortedTree.h"
#include "CAllocator.h"
#include <iostream>
#include <typeinfo>

#include "CVector.h"

namespace OpenUtility
{

#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable : 4101)
#endif

template<class K,class V=void*>
class CRedBlackTree : public ISortedTree<K,V>
{
	friend class CNode;

public:
	class CNode
	{
		friend class CRedBlackTree<K,V>;

	public:
		CNode(K key,V val,CNode *parent=NULL) : Key(key),Value(val),isRed(true),Parent(parent),Right(NULL),Left(NULL) {}
		~CNode() {CRedBlackTree<K,V>::Allocator.Delete(Right);CRedBlackTree<K,V>::Allocator.Delete(Left);}
		friend inline std::ostream& operator<<(std::ostream &o,const CNode &node)
		{
			o << node.Key << '(' << node.Value << ')' << (node.isRed ? 'r' : 'b') << '[';
			if (node.Left) o << *node.Left;
			o << ',';
			if (node.Right) o << *node.Right;
			o << ']';
			return(o);
		}

	private:
		K Key;
		V Value;
		bool isRed;
		CNode *Parent;
		CNode *Right;
		CNode *Left;
	};

public:
	struct CRedBlackTreeIterator : public OpenUtility::Iterator<K>
	{
		friend class CRedBlackTree<K>;
		CRedBlackTreeIterator() : pos(NULL) {}
		void operator++(int);
		void operator+=(int i);
		void operator--(int);
		inline void operator-=(int i) {operator+=(-i);}
		inline bool operator==(const OpenUtility::Iterator<K> &obj)
		{
			try
			{
				const CRedBlackTreeIterator &ref=dynamic_cast<const CRedBlackTreeIterator&>(obj);
				return(*this==ref);
			}
			catch(const std::bad_cast &e) {return(false);}
		}
		inline bool operator!=(const OpenUtility::Iterator<K> &obj) {return(!(*this==obj));}
		inline bool operator==(const CRedBlackTreeIterator &obj) {return(pos==obj.pos);}
		inline bool operator!=(const CRedBlackTreeIterator &obj) {return(pos!=obj.pos);}
		inline K* operator->() {return(pos ? pos->Value : NULL);}
		inline K& operator*() {return(*pos->Value);}
		inline bool isNull() {return(pos==NULL);}

	private:
		CRedBlackTreeIterator(CNode *init) : pos(init) {}

	private:
		CNode *pos;
	};

public:
	inline CRedBlackTree() : Root(NULL) {}
	inline ~CRedBlackTree() {Allocator.Delete(Root);}
	bool Add(K key,V value=0);
	V Remove(K key);
	bool Exists(K key);
	V& operator[](K key);
	V operator[](K key) const;
	friend inline std::ostream& operator<<(std::ostream &o,const CRedBlackTree<K,V> &tree)
	{
		o << '{';
		if (tree.Root) o << *tree.Root;
		o << '}';
		return(o);
	}
	CRedBlackTreeIterator GetMin();
	CRedBlackTreeIterator GetMax();
	bool Check();
	unsigned int GetMaxHeight();
//	void Draw();

private:
	CRedBlackTree(const CRedBlackTree &obj) {}
	CRedBlackTree& operator=(const CRedBlackTree &obj) {}
	CNode* Insert(K key,V value=0);
	void Organize(CNode *node);
	void Rotate(CNode *node);
	bool CheckRecurs(CNode *node,unsigned int &nbBlack);
	unsigned int GetMaxHeightRecurs(CNode *node);

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
