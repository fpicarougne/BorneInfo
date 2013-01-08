#ifndef _CMat4x4_h
	#define _CMat4x4_h

#include <string.h>
#include <iostream>

namespace OpenUtility
{

// Format: column-major order
// [ 0 , 4 , 8 ,12 ]
// [ 1 , 5 , 9 ,13 ]
// [ 2 , 6 ,10 ,14 ]
// [ 3 , 7 ,11 ,15 ]

// PS: Traditionnal view
// [ 0 , 1 , 2 , 3 ]
// [ 4 , 5 , 6 , 7 ]
// [ 8 , 9 ,10 ,11 ]
// [12 ,13 ,14 ,15 ]

template<class T>
class CMat4x4
{
public:
	CMat4x4();
	inline CMat4x4(T m[16]) {memcpy(mat,m,sizeof(mat));}
	CMat4x4(T m11,T m12,T m13,T m14,T m21,T m22,T m23,T m24,T m31,T m32,T m33,T m34,T m41,T m42,T m43,T m44);
	CMat4x4(const CMat4x4<T> &obj);
	CMat4x4<T>& operator=(const CMat4x4<T> &obj);
	~CMat4x4();
	inline CMat4x4<T>& Set(const T m[16]) {memcpy(mat,m,sizeof(mat));return(*this);}
	inline CMat4x4<T>& Set(T m11,T m12,T m13,T m14,T m21,T m22,T m23,T m24,T m31,T m32,T m33,T m34,T m41,T m42,T m43,T m44)
	{
		mat[0]=m11;mat[1]=m12;mat[2]=m13;mat[3]=m14;
		mat[4]=m21;mat[5]=m22;mat[6]=m23;mat[7]=m24;
		mat[8]=m31;mat[9]=m32;mat[10]=m33;mat[11]=m34;
		mat[12]=m41;mat[13]=m42;mat[14]=m43;mat[15]=m44;
		return(*this);
	}
	inline CMat4x4<T>& SetNull() {bzero(mat,sizeof(mat));return(*this);}
	CMat4x4<T>& SetIdentity();
	inline T Get(unsigned int i,unsigned int j) const {return(mat[i+j*4]);}
	inline const T* GetMatrix() const {return(mat);}
	inline CMat4x4<T>& Set(unsigned int i,unsigned int j,T val) {mat[i+j*4]=val;return(*this);}

	// Math operations
	CMat4x4<T>& operator*=(CMat4x4<T> &m);
	CMat4x4<T>& operator+=(CMat4x4<T> &m);
	CMat4x4<T>& operator-=(CMat4x4<T> &m);
	CMat4x4<T>& operator*=(T s);
	inline CMat4x4<T>& operator/=(T s) {return((*this)*=1/s);}
	CMat4x4<T>& operator+=(T s);
	inline CMat4x4<T>& operator-=(T s) {return((*this)+=-s);}
	friend inline CMat4x4<T> operator*(CMat4x4<T> &m1,CMat4x4<T> &m2)
	{
		return(CMat4x4<T>(	m1.mat[0]*m2.mat[0]+m1.mat[4]*m2.mat[1]+m1.mat[8]*m2.mat[2]+m1.mat[12]*m2.mat[3],
							m1.mat[1]*m2.mat[0]+m1.mat[5]*m2.mat[1]+m1.mat[9]*m2.mat[2]+m1.mat[13]*m2.mat[3],
							m1.mat[2]*m2.mat[0]+m1.mat[6]*m2.mat[1]+m1.mat[10]*m2.mat[2]+m1.mat[14]*m2.mat[3],
							m1.mat[3]*m2.mat[0]+m1.mat[7]*m2.mat[1]+m1.mat[11]*m2.mat[2]+m1.mat[15]*m2.mat[3],
							m1.mat[0]*m2.mat[4]+m1.mat[4]*m2.mat[5]+m1.mat[8]*m2.mat[6]+m1.mat[12]*m2.mat[7],
							m1.mat[1]*m2.mat[4]+m1.mat[5]*m2.mat[5]+m1.mat[9]*m2.mat[6]+m1.mat[13]*m2.mat[7],
							m1.mat[2]*m2.mat[4]+m1.mat[6]*m2.mat[5]+m1.mat[10]*m2.mat[6]+m1.mat[14]*m2.mat[7],
							m1.mat[3]*m2.mat[4]+m1.mat[7]*m2.mat[5]+m1.mat[11]*m2.mat[6]+m1.mat[15]*m2.mat[7],
							m1.mat[0]*m2.mat[8]+m1.mat[4]*m2.mat[9]+m1.mat[8]*m2.mat[10]+m1.mat[12]*m2.mat[11],
							m1.mat[1]*m2.mat[8]+m1.mat[5]*m2.mat[9]+m1.mat[9]*m2.mat[10]+m1.mat[13]*m2.mat[11],
							m1.mat[2]*m2.mat[8]+m1.mat[6]*m2.mat[9]+m1.mat[10]*m2.mat[10]+m1.mat[14]*m2.mat[11],
							m1.mat[3]*m2.mat[8]+m1.mat[7]*m2.mat[9]+m1.mat[11]*m2.mat[10]+m1.mat[15]*m2.mat[11],
							m1.mat[0]*m2.mat[12]+m1.mat[4]*m2.mat[13]+m1.mat[8]*m2.mat[14]+m1.mat[12]*m2.mat[15],
							m1.mat[1]*m2.mat[12]+m1.mat[5]*m2.mat[13]+m1.mat[9]*m2.mat[14]+m1.mat[13]*m2.mat[15],
							m1.mat[2]*m2.mat[12]+m1.mat[6]*m2.mat[13]+m1.mat[10]*m2.mat[14]+m1.mat[14]*m2.mat[15],
							m1.mat[3]*m2.mat[12]+m1.mat[7]*m2.mat[13]+m1.mat[11]*m2.mat[14]+m1.mat[15]*m2.mat[15]));
	}
	friend inline CMat4x4<T> operator+(CMat4x4<T> &m1,CMat4x4<T> &m2)
	{
		return(CMat4x4<T>(	m1.mat[0]+m2.mat[0],m1.mat[1]+m2.mat[1],m1.mat[2]+m2.mat[2],m1.mat[3]+m2.mat[3],
							m1.mat[4]+m2.mat[4],m1.mat[5]+m2.mat[5],m1.mat[6]+m2.mat[6],m1.mat[7]+m2.mat[7],
							m1.mat[8]+m2.mat[8],m1.mat[9]+m2.mat[9],m1.mat[10]+m2.mat[10],m1.mat[11]+m2.mat[11],
							m1.mat[12]+m2.mat[12],m1.mat[13]+m2.mat[13],m1.mat[14]+m2.mat[14],m1.mat[15]+m2.mat[15]));
	}
	friend inline CMat4x4<T> operator-(CMat4x4<T> &m1,CMat4x4<T> &m2)
	{
		return(CMat4x4<T>(	m1.mat[0]-m2.mat[0],m1.mat[1]-m2.mat[1],m1.mat[2]-m2.mat[2],m1.mat[3]-m2.mat[3],
							m1.mat[4]-m2.mat[4],m1.mat[5]-m2.mat[5],m1.mat[6]-m2.mat[6],m1.mat[7]-m2.mat[7],
							m1.mat[8]-m2.mat[8],m1.mat[9]-m2.mat[9],m1.mat[10]-m2.mat[10],m1.mat[11]-m2.mat[11],
							m1.mat[12]-m2.mat[12],m1.mat[13]-m2.mat[13],m1.mat[14]-m2.mat[14],m1.mat[15]-m2.mat[15]));
	}
	friend inline CMat4x4<T> operator+(T s,CMat4x4<T> &m) {return(m+s);}
	friend inline CMat4x4<T> operator+(CMat4x4<T> &m,T s)
	{
		return(CMat4x4<T>(	m.mat[0]+s,m.mat[1]+s,m.mat[2]+s,m.mat[3]+s,
							m.mat[4]+s,m.mat[5]+s,m.mat[6]+s,m.mat[7]+s,
							m.mat[8]+s,m.mat[9]+s,m.mat[10]+s,m.mat[11]+s,
							m.mat[12]+s,m.mat[13]+s,m.mat[14]+s,m.mat[15]+s));
	}
	friend inline CMat4x4<T> operator-(T s,CMat4x4<T> &m) {return(m-s);}
	friend inline CMat4x4<T> operator-(CMat4x4<T> &m,T s)
	{
		return(CMat4x4<T>(	m.mat[0]-s,m.mat[1]-s,m.mat[2]-s,m.mat[3]-s,
							m.mat[4]-s,m.mat[5]-s,m.mat[6]-s,m.mat[7]-s,
							m.mat[8]-s,m.mat[9]-s,m.mat[10]-s,m.mat[11]-s,
							m.mat[12]-s,m.mat[13]-s,m.mat[14]-s,m.mat[15]-s));
	}
	CMat4x4<T>& Transpose();
	CMat4x4<T>& Invert();

	// 3D operations
	CMat4x4<T>& SetFrustum(T left,T right,T bottom,T top,T nearVal,T farVal);
	CMat4x4<T>& SetLookAt(T eyeX,T eyeY,T eyeZ,T centerX,T centerY,T centerZ,T upX,T upY,T upZ);
	CMat4x4<T>& SetTranslate(T tx,T ty,T tz);
	CMat4x4<T>& SetRotate(T angle,T rx,T ry,T rz);
	CMat4x4<T>& SetScale(T sx,T sy,T sz);

	// Display informations
	friend inline std::ostream& operator<<(std::ostream &o,const CMat4x4<T> &obj)
	{
		unsigned int i,j;

		for (j=0;j<4;j++)
		{
			o << '[';
			for (i=0;i<4;i++)
			{
				if (i) o << ',';
				o << ' ' << obj.mat[i*4+j];
			}
			o << " ]" << std::endl;
		}
		return(o);
	}

private:
	T mat[16];
};

}

#include "Res/CMat4x4.cxx"

#endif
