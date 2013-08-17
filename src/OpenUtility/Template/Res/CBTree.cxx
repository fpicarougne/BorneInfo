//-----------------------------------------------------------------------
// CBTree<T,I>::CNode

template<class T,int I> CBTree<T,I>::CNode::CNode(CNode *parent) :
	Parent(parent)
{
	memset(Values,0,sizeof(Values));
	memset(Childs,0,sizeof(Childs));
}

//-----------------------------------------------------------------------
// CBTree<T,I>

template<class T,int I> CAllocator<typename CBTree<T,I>::CNode> CBTree<T,I>::Allocator;

template<class T,int I> bool CBTree<T,I>::Add(T *obj)
{
	unsigned int i;
	bool found;
	CNode *node,*child;

	if (Root==NULL) Root=Allocator.New();
	node=Root;

	found=false;
	while (!found)
	{
		i=0;
		while (i<I*2-1 && node->Values[i])
		{
			if (*node->Values[i]==*obj) return(false);
			else if (*node->Values[i]<*obj) i++;
			else break;
		}
		if (node->Childs[i]==NULL) found=true;
		else node=node->Childs[i];
	}

	found=false;
	child=NULL;
	while (!found)
	{
		if (node->Values[I*2-2]==NULL)
		{
			// It exists a place in the node
			if (node->Values[i])
			{
				memmove(&node->Values[i+1],&node->Values[i],(I*2-2-i)*sizeof(T*));
				memmove(&node->Childs[i+1],&node->Childs[i],(I*2-2-i)*sizeof(CNode*));
			}
			node->Values[i]=obj;
			node->Childs[i+1]=child;
			found=true;
		}
		else
		{
			CNode *node2;

			if (!node->Parent)
			{
				Root=node->Parent=Allocator.New();
				Root->Childs[0]=node;
			}
			node2=Allocator.New(node->Parent);

			if (i<I)
			{
				memcpy(node2->Values,&node->Values[I-1],I*sizeof(T*));
				memcpy(node2->Childs,&node->Childs[I-1],(I+1)*sizeof(T*));
				memset(&node->Values[I-1],0,I*sizeof(T*));
				memset(&node->Childs[I-1],0,(I+1)*sizeof(T*));
				if (i<I-1)
				{
					memmove(&node->Values[i+1],&node->Values[i],(I-1-i)*sizeof(T*));
					memmove(&node->Childs[i+1],&node->Childs[i],(I-1-i)*sizeof(T*));
				}
				node->Values[i]=obj;
				node->Childs[i+1]=child;
			}
			else
			{
				memcpy(node2->Values,&node->Values[I],(i-(I-1)-1)*sizeof(T*));
				memcpy(node2->Childs,&node->Childs[I],(i-(I-1))*sizeof(T*));
				node2->Values[i-(I-1)-1]=obj;
				node2->Childs[i-(I-1)]=child;
				memcpy(&node2->Values[i-(I-1)],&node->Values[i],(2*I-1-i)*sizeof(T*));
//				memcpy(&node2->Childs[i-(I-1)],&node->Childs[i],(2*I-1-i)*sizeof(T*));
				memset(&node->Values[I],0,(I-1)*sizeof(T*));
				memset(&node->Childs[I],0,I*sizeof(T*));
			}

			obj=node->Values[I-1];
			node->Values[I-1]=NULL;
			node->Childs[I]=NULL;

			i=0;
			while (node->Parent->Childs[i]!=node) i++;
			node=node->Parent;
			child=node2;
//TODO: attach right branch
		}
	}

	return(false);
}

template<class T,int I> T* CBTree<T,I>::Remove(T *obj)
{
	return(NULL);
}

template<class T,int I> bool CBTree<T,I>::Exists(T *obj)
{
	unsigned int i;
	CNode *node=Root;

	while (node!=NULL)
	{
		i=0;
		while (i<I-1 && node->Values[i])
		{
			if (*node->Values[i]==*obj) return(true);
			else if (*node->Values[i]<*obj) i++;
			else break;
		}
		node=node->Childs[i];
	}

	return(false);
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

template<class T,int I> bool CBPlusTree<T,I>::Add(T *obj)
{
	return(false);
}

template<class T,int I> T* CBPlusTree<T,I>::Remove(T *obj)
{
	return(NULL);
}

template<class T,int I> bool CBPlusTree<T,I>::Exists(T *obj)
{
	return(false);
}
