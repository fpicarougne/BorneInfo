//-----------------------------------------------------------------------
// CRedBlackTree<K,V>

template<class K,class V> CAllocator<typename CRedBlackTree<K,V>::CNode> CRedBlackTree<K,V>::Allocator;

template<class K,class V> bool CRedBlackTree<K,V>::Add(K key,V value)
{
	CNode *node;

	if (Root==NULL) node=Root=Allocator.New(key,value);
	else if ((node=Insert(key,value))==NULL) return(false);

	Organize(node);
	if (Root->isRed) Root->isRed=false;

	return(true);
}

template<class K,class V> typename CRedBlackTree<K,V>::CNode* CRedBlackTree<K,V>::Insert(K key,V value)
{
	CNode *node=Root;

	while (node!=NULL)
	{
		if (node->Key==key) return(NULL);
		if (node->Key<key)
		{
			if (node->Right==NULL) return(node->Right=Allocator.New(key,value,node));
			node=node->Right;
		}
		else
		{
			if (node->Left==NULL) return(node->Left=Allocator.New(key,value,node));
			node=node->Left;
		}
	}
	return(NULL);
}

template<class K,class V> void CRedBlackTree<K,V>::Organize(CNode *node)
{
	while (node!=Root && node->Parent->isRed)
	{
		if (node->Parent==Root)	// case 0
			node->Parent->isRed=false;
		else
		{
			CNode *uncle=node->Parent->Parent;
			uncle=uncle->Left==node->Parent ? uncle->Right : uncle->Left;

			if (!uncle || !uncle->isRed)	// case 2
			{
				if (((node->Parent==node->Parent->Parent->Left) && (node==node->Parent->Left))
					|| ((node->Parent==node->Parent->Parent->Right) && (node==node->Parent->Right)))	// case 2a
				{
					node->Parent->isRed=false;
					node->Parent->Parent->isRed=true;
					Rotate(node->Parent);
				}
				else	// case 2b
				{
					node->isRed=false;
					node->Parent->Parent->isRed=true;
					Rotate(node);
					Rotate(node);
				}
			}
			else	// case 1
			{
				node->Parent->isRed=uncle->isRed=false;
				uncle->Parent->isRed=true;
				node=uncle->Parent;
			}
		}
	}
}

template<class K,class V> void CRedBlackTree<K,V>::Rotate(CNode *node)
{
	CNode *parent=node->Parent;

	if (!parent) return;

	if (parent->Parent)
	{
		if (parent==parent->Parent->Left) parent->Parent->Left=node;
		else parent->Parent->Right=node;
	}
	else Root=node;
	node->Parent=parent->Parent;
	parent->Parent=node;

	if (parent->Left==node)
	{
		// Rotation droite
		parent->Left=node->Right;
		if (node->Right) node->Right->Parent=parent;
		node->Right=parent;
	}
	else
	{
		// Rotation gauche
		parent->Right=node->Left;
		if (node->Left) node->Left->Parent=parent;
		node->Left=parent;
	}
}

template<class K,class V> V CRedBlackTree<K,V>::Remove(K key)
{
	CNode *node=Root;

	// Search node to delete
	while ((node!=NULL) && !(node->Key==key))
	{
		if (node->Key<key) node=node->Right;
		else node=node->Left;
	}
	if (node==NULL) THROW(CONCAT(typename ISortedTree<K,V>::Exception),ISortedTree<K,V>::Exception::EErrNotFound);
	V retObj=node->Value;

	// Deletion phase
	CNode *nRemp=NULL;
	CNode *nSibl,*nParent;

	// Reduce the problem to a single child problem
	if ((node->Left!=NULL) && (node->Right!=NULL))
	{
		nRemp=node;
		node=node->Left;
		while (node->Right!=NULL) node=node->Right;
		nRemp->Key=node->Key;
		nRemp->Value=node->Value;
	}

	// node is the Node to delete
	// nRemp is the Node to resolve
	// Hook child of node to the parent of node
	if (node->Right) nRemp=node->Right;
	else nRemp=node->Left;
	if (nRemp) nRemp->Parent=node->Parent;
	if ((nParent=node->Parent)!=NULL)
	{
		if (nParent->Left==node)
		{
			nSibl=nParent->Right;
			node->Parent->Left=nRemp;
		}
		else
		{
			nSibl=nParent->Left;
			node->Parent->Right=nRemp;
		}
	}
	else Root=nRemp;

	if (Root && !node->isRed)
	{
		if (nRemp && nRemp->isRed)
			nRemp->isRed=false;
		else
		{
			// nParent and nSibl has to be set
			// case #1
			while (nParent!=NULL)
			{
				// case #2
				if (nSibl && nSibl->isRed)
				{
					nParent->isRed=true;
					nSibl->isRed=false;
					Rotate(nSibl);
					if (nParent->Left==nRemp) nSibl=nParent->Right;
					else nSibl=nParent->Left;
				}
				// both nRemp and nSibl are black
				// case #3
				if (nParent->isRed ||
					nSibl->isRed ||
					(nSibl->Left && nSibl->Left->isRed) ||
					(nSibl->Right && nSibl->Right->isRed))
				{
					// case #4
					if (nParent->isRed &&
						(!nSibl->isRed) &&
						(nSibl->Left==NULL || !nSibl->Left->isRed) &&
						(nSibl->Right==NULL || !nSibl->Right->isRed))
					{
						nSibl->isRed=true;
						nParent->isRed=false;
					}
					else
					{
						// case #5
						if (!nSibl->isRed)
						{
							if (nParent->Left==nRemp &&
								(nSibl->Right==NULL || !nSibl->Right->isRed) &&
								(nSibl->Left && nSibl->Left->isRed))
							{
								nSibl->isRed=true;
								nSibl->Left->isRed=false;
								Rotate(nSibl->Left);
								if (nParent->Left==nRemp) nSibl=nParent->Right;
								else nSibl=nParent->Left;
							}
							else if (nParent->Right==nRemp &&
								(nSibl->Left==NULL || !nSibl->Left->isRed) &&
								(nSibl->Right && nSibl->Right->isRed))
							{
								nSibl->isRed=true;
								nSibl->Right->isRed=false;
								Rotate(nSibl->Right);
								if (nParent->Left==nRemp) nSibl=nParent->Right;
								else nSibl=nParent->Left;
							}
						}
						// case #6
						nSibl->isRed=nParent->isRed;
						nParent->isRed=false;
						if (nParent->Left==nRemp) nSibl->Right->isRed=false;
						else nSibl->Left->isRed=false;
						Rotate(nSibl);
					}
					nParent=NULL;
				}
				else
				{
					nSibl->isRed=true;
					nRemp=nParent;
					if ((nParent=nParent->Parent)!=NULL)
					{
						if (nParent->Left==nRemp) nSibl=nParent->Right;
						else nSibl=nParent->Left;
					}
					// return to case #1
				}
			}
		}
	}

	Allocator.Delete(node);
	return(retObj);
}

template<class K,class V> bool CRedBlackTree<K,V>::Exists(K key)
{
	CNode *node=Root;

	while (node!=NULL)
	{
		if (node->Key==key) return(true);
		if (node->Key<key) node=node->Right;
		else node=node->Left;
	}
	return(false);
}

template<class K,class V> V& CRedBlackTree<K,V>::operator[](K key)
{
	CNode *node=Root;

	while (node!=NULL)
	{
		if (node->Key==key) return(node->Value);
		if (node->Key<key) node=node->Right;
		else node=node->Left;
	}
	THROW(CONCAT(typename ISortedTree<K,V>::Exception),ISortedTree<K,V>::Exception::EErrNotFound);
}

template<class K,class V> V CRedBlackTree<K,V>::operator[](K key) const
{
	CNode *node=Root;

	while (node!=NULL)
	{
		if (node->Key==key) return(node->Value);
		if (node->Key<key) node=node->Right;
		else node=node->Left;
	}
	THROW(CONCAT(typename ISortedTree<K,V>::Exception),ISortedTree<K,V>::Exception::EErrNotFound);
}

template<class K,class V> typename CRedBlackTree<K,V>::CRedBlackTreeIterator CRedBlackTree<K,V>::GetMin()
{
	CNode *node=Root;

	if (node)
	{
		while (node->Left!=NULL) node=node->Left;
	}
	return(CRedBlackTreeIterator(node));
}

template<class K,class V> typename CRedBlackTree<K,V>::CRedBlackTreeIterator CRedBlackTree<K,V>::GetMax()
{
	CNode *node=Root;

	if (node)
	{
		while (node->Right!=NULL) node=node->Right;
	}
	return(CRedBlackTreeIterator(node));
}

template<class K,class V> bool CRedBlackTree<K,V>::Check()
{
	unsigned int nb;
	return(CheckRecurs(Root,nb));
}

template<class K,class V> bool CRedBlackTree<K,V>::CheckRecurs(CNode *node,unsigned int &nbBlack)
{
	unsigned int n1,n2;

	if (!node)
	{
		nbBlack=1;
		return(true);
	}

	if (node->isRed && ((node->Left && node->Left->isRed) || (node->Right && node->Right->isRed))) return(false);
	if (!CheckRecurs(node->Left,n1)) return(false);
	if (!CheckRecurs(node->Right,n2)) return(false);
	if (n1!=n2) return(false);

	nbBlack=n1;
	if (!node->isRed) nbBlack++;

	return(true);
}

template<class K,class V> unsigned int CRedBlackTree<K,V>::GetMaxHeight()
{
	return(GetMaxHeightRecurs(Root));
}

template<class K,class V> unsigned int CRedBlackTree<K,V>::GetMaxHeightRecurs(CNode *node)
{
	if (!node) return(0);

	unsigned int h1,h2;
	h1=GetMaxHeightRecurs(node->Left);
	h2=GetMaxHeightRecurs(node->Right);
	if (h1>h2) return(h1+1);
	return(h2+1);
}

/*template<class K,class V> void CRedBlackTree<K,V>::Draw()
{
	unsigned int h=GetMaxHeight();
	if (h==0) return;

	CVector<CNode> tabNode[2];
	const unsigned int sizeDisp=5,sizeInter=3;
	unsigned int i,j,k,tab;

	tabNode[tab=0].Add(Root);
	for (i=0;i<h;i++)
	{
		bool child;
		char state;

		unsigned int nbEnglob,nbEnglob2;
		nbEnglob=((int)pow(2.0,int(h-(i+1)))-1)*(sizeDisp+sizeInter)/2;

		tabNode[1-tab].RemoveAll();
		child=false;
		state=0;

		while (state!=4)
		{
			for (j=0;j<sizeInter;j++) std::cout << ' ';

			switch(state)
			{
			case 0:
				for (j=0;j<nbEnglob;j++) std::cout << ' ';
				for (k=0;k<tabNode[tab].GetSize();k++)
				{
					CNode *n;
					if (n=tabNode[tab][k])
					{
						if (tabNode[1-tab].Add(n->Left)) child=true;
						if (tabNode[1-tab].Add(n->Right)) child=true;
						if (n->Key<10) std::cout << ' ';
						std::cout << n->Key<< '[' << (n->isRed ? 'r' : 'b') << ']';
					}
					else
					{
						tabNode[1-tab].Add(NULL);
						tabNode[1-tab].Add(NULL);
						for (j=0;j<sizeDisp;j++) std::cout << ' ';
					}
					for (j=0;j<nbEnglob*2+sizeInter;j++) std::cout << ' ';
				}
				break;

			case 1:
				if (!child) break;
				for (j=0;j<nbEnglob+(sizeDisp-3)/2;j++) std::cout << ' ';
				for (k=0;k<tabNode[tab].GetSize();k++)
				{
					CNode *n;
					if (n=tabNode[tab][k]) std::cout << (n->Left ? '/' : ' ') << ' ' << (n->Right ? '\\' : ' ');
					else std::cout << "   ";
					for (j=0;j<(sizeDisp-3)+nbEnglob*2+sizeInter;j++) std::cout << ' ';
				}
				break;

			case 2:
				if (!child) break;
				if (h>i+1) nbEnglob2=((int)pow(2.0,int(h-(i+2)))-1)*(sizeDisp+sizeInter)/2;
				else nbEnglob2=0;

				for (j=0;j<nbEnglob2+(sizeDisp-1)/2;j++) std::cout << ' ';
				for (k=0;k<tabNode[tab].GetSize();k++)
				{
					CNode *n=tabNode[tab][k];

					for (j=0;j<nbEnglob2+(sizeDisp-1)/2+sizeInter/2;j++) std::cout << (n && n->Left ? '-' : ' ');
					std::cout << "   ";
					for (j=0;j<nbEnglob2+(sizeDisp-1)/2+sizeInter/2;j++) std::cout << (n && n->Right ? '-' : ' ');
					for (j=0;j<(sizeDisp-1)+nbEnglob2*2+sizeInter;j++) std::cout << ' ';
				}
				break;

			case 3:
				if (!child) break;
				for (j=0;j<nbEnglob2+(sizeDisp-1)/2;j++) std::cout << ' ';
				for (k=0;k<tabNode[tab].GetSize();k++)
				{
					CNode *n=tabNode[tab][k];

					std::cout << (n && n->Left ? '|' : ' ');
					for (j=0;j<(sizeDisp-1)+nbEnglob2*2+sizeInter;j++) std::cout << ' ';
					std::cout << (n && n->Right ? '|' : ' ');
					for (j=0;j<(sizeDisp-1)+nbEnglob2*2+sizeInter;j++) std::cout << ' ';
				}
				break;
			}
			state++;
			std::cout << std::endl;
		}
		tab=1-tab;
	}
}*/

template<class K,class V> void CRedBlackTree<K,V>::CRedBlackTreeIterator::operator++(int)
{
	if (!pos) return;
	if (pos->Right)
	{
		pos=pos->Right;
		while (pos->Left) pos=pos->Left;
	}
	else
	{
		bool cont;

		do
		{
			if (!pos->Parent)
			{
				pos=NULL;
				return;
			}
			cont=pos->Parent->Right==pos;
			pos=pos->Parent;
		}
		while (cont);
	}
}

template<class K,class V> void CRedBlackTree<K,V>::CRedBlackTreeIterator::operator--(int)
{
	if (!pos) return;
	if (pos->Left)
	{
		pos=pos->Left;
		while (pos->Right) pos=pos->Right;
	}
	else
	{
		bool cont;

		do
		{
			if (!pos->Parent)
			{
				pos=NULL;
				return;
			}
			cont=pos->Parent->Left==pos;
			pos=pos->Parent;
		}
		while (cont);
	}
}

template<class K,class V> void CRedBlackTree<K,V>::CRedBlackTreeIterator::operator+=(int i)
{
	if (!pos) return;

	if (i<0) for (;i!=0 && pos;i++) pos--;
	else for (;i!=0 && pos;i--) pos++;
}
