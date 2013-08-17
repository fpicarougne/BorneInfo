#ifndef _Iterator_h
	#define _Iterator_h

namespace OpenUtility
{

template <class T>
struct Iterator
{
	virtual ~Iterator() {}
	virtual void operator++(int)=0;
	virtual void operator+=(int i)=0;
	virtual void operator--(int)=0;
	virtual void operator-=(int i)=0;
	virtual T* operator->()=0;
	virtual T& operator*()=0;
	virtual bool isNull()=0;
	virtual bool operator==(const Iterator<T> &obj)=0;
	virtual bool operator!=(const Iterator<T> &obj)=0;
};

}

#endif
