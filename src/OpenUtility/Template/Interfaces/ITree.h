#ifndef _ITree_h
	#define _ITree_h

#include <iostream>

namespace OpenUtility
{

template<class T>
class ITree
{
public:
	class INode
	{
	public:
		INode() {}
		virtual ~INode() {}
		virtual T* GetValue() const =0;
		virtual INode* GetParent() const =0;
		virtual INode* GetChild(unsigned int i) const =0;
		virtual unsigned int GetNbChild() const =0;
		virtual INode* GetPrevSibling()=0;
		virtual INode* GetNextSibling()=0;
		virtual INode* AddChild(T *value)=0;
		virtual INode* AddChildAfter(T *node,T *value)=0;
		virtual INode* AddChildAfter(INode *node,T *value)=0;
		virtual T* RemoveChild(unsigned int i)=0;
		virtual void RemoveAllChild()=0;
		virtual void DeleteChild(unsigned int i)=0;
		virtual void DeleteAllChild()=0;
		virtual bool IsParent(INode *node)=0;
		friend inline std::ostream& operator<<(std::ostream &o,const INode &node)
		{
			unsigned int nbChild=node.GetNbChild();

			o << *node.GetValue();
			if (nbChild)
			{
				o << '[';
				for (unsigned int i=0;i<nbChild;i++)
				{
					if (i) o << ',';
					o << *node.GetChild(i);
				}
				o << ']';
			}
			return(o);
		}
	};

public:
	ITree() {}
	virtual ~ITree() {}
	virtual INode* GetRoot()=0;
	virtual INode* SetRoot(T *value,bool deletePrevRoot=false)=0;
	virtual void Swap(INode *node1,INode *node2)=0;
	inline void Swap(INode &node1,INode &node2) {Swap(&node1,&node2);}
	virtual void MoveToChild(INode *from,INode *to)=0;
	inline void MoveToChild(INode &from,INode &to) {MoveToChild(&from,&to);}
	friend inline std::ostream& operator<<(std::ostream &o,ITree<T> &tree)
	{
		INode *root;

		o << '{';
		if ((root=tree.GetRoot())!=NULL) o << (*root);
		o << '}';
		return(o);
	}
};

}

#endif
