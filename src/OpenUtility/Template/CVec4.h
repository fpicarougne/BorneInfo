#ifndef _CVec4_h
	#define _CVec4_h

#include <string.h>
#include <iostream>

namespace OpenUtility
{

template<class T>
class CVec4
{
public:
	CVec4(T a=0,T b=0,T c=0,T d=1) : x(vec[0]),y(vec[1]),z(vec[2]),w(vec[3]),r(vec[0]),g(vec[1]),b(vec[2]),a(vec[3]) {Set(a,b,c,d);}
	CVec4(const CVec4<T> &obj) : x(vec[0]),y(vec[1]),z(vec[2]),w(vec[3]),r(vec[0]),g(vec[1]),b(vec[2]),a(vec[3]) {memcpy(vec,obj.vec,sizeof(vec));}
	void Set(T a,T b,T c,T d) {x=a;y=b;z=c;w=d;}
	inline T& operator[](int i) {return(vec[i]);}
	inline const T& operator[](int i) const {return(vec[i]);}
	inline CVec4<T>& operator=(const CVec4<T> &v) {memcpy(vec,v.vec,sizeof(vec));return(*this);}
	inline CVec4<T>& operator+=(const CVec4<T> &v) {for (unsigned int i=0;i<4;i++) vec[i]+=v.vec[i];return(*this);}
	inline CVec4<T>& operator-=(const CVec4<T> &v) {for (unsigned int i=0;i<4;i++) vec[i]-=v.vec[i];return(*this);}
	inline CVec4<T>& operator+=(const T c) {for (unsigned int i=0;i<4;i++) vec[i]+=c;return(*this);}
	inline CVec4<T>& operator-=(const T c) {for (unsigned int i=0;i<4;i++) vec[i]-=c;return(*this);}
	inline CVec4<T>& operator*=(const T c) {for (unsigned int i=0;i<4;i++) vec[i]*=c;return(*this);}
	inline CVec4<T>& operator/=(const T c) {for (unsigned int i=0;i<4;i++) vec[i]/=c;return(*this);}

	// Display informations
	friend inline std::ostream& operator<<(std::ostream &o,const CVec4<T> &obj)
	{
		o << '[';
		for (unsigned int i=0;i<4;i++)
		{
			if (i) o << ',';
			o << ' ' << obj.vec[i];
		}
		o << " ]";
		return(o);
	}

public:
	T &x,&y,&z,&w;
	T &r,&g,&b,&a;
	T vec[4];
};

}

#endif
