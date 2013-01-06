#ifndef _OUException_h
	#define _OUException_h

#include <exception>
#include "Stream/Stream.h"

//#define THROW(class) OpenUtility::Exception::Throw(__FILE__,__LINE__,class)
#define THROW(class,...) {class e(__VA_ARGS__);e.SetFileLine(__FILE__,__LINE__);throw(e);}

namespace OpenUtility
{

class Exception : public std::exception
{
public:
	Exception() {}
	Exception(const Exception &obj) : std::exception(obj),Error(obj.Error) {}
	virtual ~Exception() throw() {}
	friend inline std::ostream& operator<<(std::ostream &o,OpenUtility::Exception &e)
	{
		o << e.Error << std::endl;
		return(o);
	}
	inline Exception& SetFileLine(const char *file,long line) {if (line>=0) Error.Format("File \"%s\" - line %ld",file,line);else Error="";UpdateStr();return(*this);}
	inline const char* what() const throw() {return(Error);}

protected:
	virtual void UpdateStr() {}

protected:
	CStream Error;
};

}

#endif
