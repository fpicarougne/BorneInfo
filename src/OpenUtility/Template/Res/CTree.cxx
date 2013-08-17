//-----------------------------------------------------------------------
// CTree<T>

#include "../../Utility/OUException.h"

template<class T> CAllocator<typename CTree<T>::CNode> CTree<T>::Allocator;

template<class T> CTree<T>::CTree(T *node) :
	Root(NULL)
{
	if (node) SetRoot(node);
}

template<class T> CTree<T>::CTree(const CTree &obj) :
	Root(NULL)
{
	if (obj.Root) Root=Allocator.New(obj.Root);
}

template<class T> CTree<T>::~CTree()
{
	Allocator.Delete(Root);
}

template<class T> CTree<T>& CTree<T>::operator=(const CTree<T> &obj)
{
	Allocator.Delete(Root);
	if (obj.Root) Root=Allocator.New(obj.Root);
	else Root=NULL;

	return(*this);
}

template<class T> typename CTree<T>::CNode* CTree<T>::SetRoot(T *value,bool deletePrevRoot)
{
	if (deletePrevRoot) Allocator.Delete(Root);
	return(Root=Allocator.New(value));
}

template<class T> void CTree<T>::Swap(typename ITree<T>::INode *node1,typename ITree<T>::INode *node2)
{
	CNode *n1=dynamic_cast<CNode*>(node1);
	CNode *n2=dynamic_cast<CNode*>(node2);

	if ((n1==NULL) || (n2==NULL)) THROW(Exception,"Nodes are not CTree<T>::CNode variables");
	if (n1->IsParent(n2) || n2->IsParent(n1)) THROW(Exception,"Cannot Swap with a children");
	if (n1->Parent==NULL || n2->Parent==NULL) THROW(Exception,"Cannot Swap a root node");

	typename CListe<CNode>::CListeIterator pos(n1->PosInParent);
	CNode *parent=n1->Parent;

	parent->Childs[pos]=n2;
	n2->Parent->Childs[n2->PosInParent]=n1;
	n1->PosInParent=n2->PosInParent;
	n1->Parent=n2->Parent;
	n2->PosInParent=pos;
	n2->Parent=parent;
}

template<class T> void CTree<T>::MoveToChild(typename ITree<T>::INode *from,typename ITree<T>::INode *to)
{
	CNode *nFrom=dynamic_cast<CNode*>(from);
	CNode *nTo=dynamic_cast<CNode*>(to);

	if ((nFrom==NULL) || (nTo==NULL)) THROW(Exception,"Nodes are not CTree<T>::CNode variables");
	if (nTo->IsParent(nFrom)) THROW(Exception,"Cannot Move to a children");
	if (nFrom->Parent==NULL) THROW(Exception,"Cannot Move a root node");

	nFrom->Parent->Childs.Remove(nFrom->PosInParent);
	nFrom->Parent=nTo;
	nTo->Childs.Add(nFrom,&nFrom->PosInParent);
}

//-----------------------------------------------------------------------
// CTree<T>::CNode

template<class T> CTree<T>::CNode::CNode(T *node) :
	Value(node),
	Parent(NULL)
{
}

template<class T> CTree<T>::CNode::CNode(const CNode &obj) :
	Value(obj.Value),
	Parent(NULL)
{
	CopyChilds(obj);
}

template<class T> CTree<T>::CNode::~CNode()
{
	RemoveAllChild();
}

template<class T> typename CTree<T>::CNode& CTree<T>::CNode::operator=(const CNode &obj)
{
	Value=obj.Value;
	if (Parent) Parent->Childs.Remove(PosInParent);
	Parent=NULL;
	CopyChilds(obj);
	return(*this);
}

template<class T> void CTree<T>::CNode::CopyChilds(const CNode &from)
{
	CNode *nTmp;
	for (unsigned int i=0;i<from.Childs.GetSize();i++)
	{
		nTmp=Allocator.New(*from.Childs[i]);
		nTmp->Parent=this;
		Childs.Add(nTmp,&nTmp->PosInParent);
	}
}

template<class T> typename CTree<T>::CNode* CTree<T>::CNode::AddChild(T *value)
{
	CNode *nTmp=Allocator.New(value);

	nTmp->Parent=this;
	return(Childs.Add(nTmp,&nTmp->PosInParent));
}

template<class T> typename CTree<T>::CNode* CTree<T>::CNode::AddChildAfter(T *node,T *value)
{
	bool found;

	typename CListe<CNode>::CListeIterator posDeb;
	Childs.GetFirst(&posDeb);
	typename CListe<CNode>::CListeIterator pos(posDeb);
	found=false;
	do
	{
		if ((*pos->Value)==*node) found=true;
		else pos++;
	}
	while (!found && (pos!=posDeb));

	if (found)
	{
		CNode *nTmp=Allocator.New(value);
		nTmp->Parent=this;
		return(Childs.AddAfter(nTmp,pos,&nTmp->PosInParent));
	}
	return(NULL);
}

template<class T> typename CTree<T>::CNode* CTree<T>::CNode::AddChildAfter(typename ITree<T>::INode *node,T *value)
{
	CNode *nTmp,*n=NULL;

	if (node && (n=dynamic_cast<CNode*>(node))==NULL) THROW(Exception,"node is not a CTree<T>::CNode variable");

	nTmp=Allocator.New(value);
	nTmp->Parent=this;

	if (node) Childs.AddAfter(nTmp,n->PosInParent,&nTmp->PosInParent);
	else Childs.AddFirst(nTmp,&nTmp->PosInParent);
	return(nTmp);
}

template<class T> bool CTree<T>::CNode::IsParent(typename ITree<T>::INode *node)
{
	bool res=false;
	CNode *tmpNode,*n;

	if ((n=dynamic_cast<CNode*>(node))==NULL) THROW(Exception,"node is not a CTree<T>::CNode variable");

	tmpNode=this;
	while(!res && (tmpNode!=NULL))
	{
		res=tmpNode->Parent==node;
		tmpNode=tmpNode->Parent;
	}

	return(res);
}

template<class T> T* CTree<T>::CNode::RemoveChild(unsigned int i)
{
	CNode *node=Childs.Remove(i);
	T *obj=node->Value;
	node->RemoveAllChild();
	CTree<T>::Allocator.Delete(node);
	return(obj);
}

template<class T> void CTree<T>::CNode::RemoveAllChild()
{
	while (Childs.GetSize())
		RemoveChild(0);
}

template<class T> void CTree<T>::CNode::DeleteChild(unsigned int i)
{
	CNode *node=Childs.Remove(i);
	delete node->Value;
	node->DeleteAllChild();
	CTree<T>::Allocator.Delete(node);
}

template<class T> void CTree<T>::CNode::DeleteAllChild()
{
	while (Childs.GetSize())
		DeleteChild(0);
}

template<class T> typename CTree<T>::CNode* CTree<T>::CNode::GetPrevSibling()
{
	if (!Parent) return(NULL);
	if (&(*PosInParent)==Parent->Childs.GetFirst()) return(NULL);
	typename CListe<CNode>::CListeIterator pos(PosInParent);
	pos--;
	return(&(*pos));
}

template<class T> typename CTree<T>::CNode* CTree<T>::CNode::GetNextSibling()
{
	if (!Parent) return(NULL);
	if (&(*PosInParent)==Parent->Childs.GetLast()) return(NULL);
	typename CListe<CNode>::CListeIterator pos(PosInParent);
	pos++;
	return(&(*pos));
}
