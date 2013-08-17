#ifndef _CTree_h
	#define _CTree_h

#include "Interfaces/ITree.h"
#include "CListe.h"
#include "CAllocator.h"

namespace OpenUtility
{

template<class T>
class CTree : public ITree<T>
{
	friend class CNode;

public:
	class CNode : public ITree<T>::INode
	{
		friend class CTree<T>;
		friend class CAllocator<CNode>;

	public:
		~CNode();
		inline T* GetValue() const {return(Value);}
		inline CNode* GetParent() const {return(Parent);}
		inline CNode* GetChild(unsigned int i) const  {return(Childs[i]);}
		inline unsigned int GetNbChild() const {return(Childs.GetSize());}
		CNode* GetPrevSibling();
		CNode* GetNextSibling();
		CNode* AddChild(T *value);
		CNode* AddChildAfter(T *node,T *value);
		CNode* AddChildAfter(typename ITree<T>::INode *node,T *value);
		T* RemoveChild(unsigned int i);
		void RemoveAllChild();
		void DeleteChild(unsigned int i);
		void DeleteAllChild();
		bool IsParent(typename ITree<T>::INode *node);

	private:
		CNode(T *node);
		CNode(const CNode &obj);
		CNode& operator=(const CNode &obj);
		void CopyChilds(const CNode &from);

	private:
		T *Value;
		CListe<CNode> Childs;
		typename CListe<CNode>::CListeIterator PosInParent;
		CNode *Parent;
	};

public:
	CTree(T *node=NULL);
	CTree(const CTree &obj);
	~CTree();
	CTree& operator=(const CTree &obj);
	inline CNode* GetRoot() {return(Root);}
	CNode* SetRoot(T *node,bool deletePrevRoot=false);
	void Swap(typename ITree<T>::INode *node1,typename ITree<T>::INode *node2);
	void MoveToChild(typename ITree<T>::INode *from,typename ITree<T>::INode *to);

private:
	CNode *Root;
	static CAllocator<CNode> Allocator;
};

#include "Res/CTree.cxx"

}

#endif
