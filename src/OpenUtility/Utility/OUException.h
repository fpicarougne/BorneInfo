#ifndef _OUException_h
	#define _OUException_h

#include <exception>
#include <iostream>
#include "Stream/Stream.h"

//#define THROW(class) OpenUtility::Exception::Throw(__FILE__,__LINE__,class)
#define CONCAT(...) __VA_ARGS__
#define THROW(class,...) {class e(__VA_ARGS__);e.SetFileLine(__FILE__,__LINE__);throw(e);}

#define DUMP(var) std::cout << #var << " = " << var << std::endl;

namespace OpenUtility
{

class Exception : public std::exception
{
public:
	Exception(const char *str=NULL,bool displayStack=true);
	Exception(const Exception &obj);
	virtual ~Exception() throw() {}
	friend inline std::ostream& operator<<(std::ostream &o,OpenUtility::Exception &e)
	{
		o << e.Error << std::endl;
		return(o);
	}
	Exception& SetFileLine(const char *file,long line);
	inline const char* what() const throw() {return(Error.GetStream());}
	inline void SetDetail(const char *str) {Detail=str;UpdateStr();}

private:
	void UpdateStr();

private:
	bool DisplayStack;
	CStream LineError;
	CStream Detail;
	CStream Error;
	CBlockStream Stack;
};

}

#endif
