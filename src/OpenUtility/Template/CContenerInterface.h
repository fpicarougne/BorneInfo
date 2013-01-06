#ifndef ContenerInterface_h
	#define ContenerInterface_h

namespace OpenUtility
{

template <class T>
struct Iterator
{
	virtual ~Iterator() {}
	virtual void operator++()=0;
	virtual void operator+=(int i)=0;
	virtual void operator--()=0;
	virtual void operator-=(int i)=0;
	virtual T* operator->()=0;
	virtual T& operator*()=0;
};

template <class T>
class CContenerInterface
{
public:
	CContenerInterface() {}
	virtual ~CContenerInterface() {}
	virtual void DeleteAll()=0;
	virtual void RemoveAll()=0;
	virtual unsigned int GetSize() const =0;
	virtual T*& ElementAt(unsigned int index)=0;
	virtual T* GetAt(unsigned int index) const=0;
	virtual T*& operator[](unsigned int index)=0;
	virtual T* operator[](unsigned int index) const=0;
};

}

#endif
