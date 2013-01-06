#include <math.h>
#include "../../Utility/3D/3D.h"

template<class T>
OpenUtility::CMat4x4<T>::CMat4x4()
{
	SetIdentity();
}

template<class T>
OpenUtility::CMat4x4<T>::CMat4x4(T m11,T m12,T m13,T m14,T m21,T m22,T m23,T m24,T m31,T m32,T m33,T m34,T m41,T m42,T m43,T m44)
{
	Set(m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,m41,m42,m43,m44);
}

template<class T>
OpenUtility::CMat4x4<T>::CMat4x4(const CMat4x4<T> &obj)
{
	Set(obj.mat);
}

template<class T>
OpenUtility::CMat4x4<T>& OpenUtility::CMat4x4<T>::operator=(const CMat4x4<T> &obj)
{
	return(Set(obj.mat));
}

template<class T>
OpenUtility::CMat4x4<T>::~CMat4x4()
{
}

template<class T>
OpenUtility::CMat4x4<T>& OpenUtility::CMat4x4<T>::SetIdentity()
{
	SetNull();
	mat[0]=mat[5]=mat[10]=mat[15]=1;
	return(*this);
}

template<class T>
OpenUtility::CMat4x4<T>& OpenUtility::CMat4x4<T>::operator*=(CMat4x4<T> &m)
{
	Set(mat[0]*m.mat[0]+mat[4]*m.mat[1]+mat[8]*m.mat[2]+mat[12]*m.mat[3],
		mat[1]*m.mat[0]+mat[5]*m.mat[1]+mat[9]*m.mat[2]+mat[13]*m.mat[3],
		mat[2]*m.mat[0]+mat[6]*m.mat[1]+mat[10]*m.mat[2]+mat[14]*m.mat[3],
		mat[3]*m.mat[0]+mat[7]*m.mat[1]+mat[11]*m.mat[2]+mat[15]*m.mat[3],
		mat[0]*m.mat[4]+mat[4]*m.mat[5]+mat[8]*m.mat[6]+mat[12]*m.mat[7],
		mat[1]*m.mat[4]+mat[5]*m.mat[5]+mat[9]*m.mat[6]+mat[13]*m.mat[7],
		mat[2]*m.mat[4]+mat[6]*m.mat[5]+mat[10]*m.mat[6]+mat[14]*m.mat[7],
		mat[3]*m.mat[4]+mat[7]*m.mat[5]+mat[11]*m.mat[6]+mat[15]*m.mat[7],
		mat[0]*m.mat[8]+mat[4]*m.mat[9]+mat[8]*m.mat[10]+mat[12]*m.mat[11],
		mat[1]*m.mat[8]+mat[5]*m.mat[9]+mat[9]*m.mat[10]+mat[13]*m.mat[11],
		mat[2]*m.mat[8]+mat[6]*m.mat[9]+mat[10]*m.mat[10]+mat[14]*m.mat[11],
		mat[3]*m.mat[8]+mat[7]*m.mat[9]+mat[11]*m.mat[10]+mat[15]*m.mat[11],
		mat[0]*m.mat[12]+mat[4]*m.mat[13]+mat[8]*m.mat[14]+mat[12]*m.mat[15],
		mat[1]*m.mat[12]+mat[5]*m.mat[13]+mat[9]*m.mat[14]+mat[13]*m.mat[15],
		mat[2]*m.mat[12]+mat[6]*m.mat[13]+mat[10]*m.mat[14]+mat[14]*m.mat[15],
		mat[3]*m.mat[12]+mat[7]*m.mat[13]+mat[11]*m.mat[14]+mat[15]*m.mat[15]);

	return(*this);
}

template<class T>
OpenUtility::CMat4x4<T>& OpenUtility::CMat4x4<T>::operator+=(CMat4x4<T> &m)
{
	for (int i=0;i<16;i++) mat[i]+=m.mat[i];
	return(*this);
}

template<class T>
OpenUtility::CMat4x4<T>& OpenUtility::CMat4x4<T>::operator-=(CMat4x4<T> &m)
{
	for (int i=0;i<16;i++) mat[i]-=m.mat[i];
	return(*this);
}

template<class T>
OpenUtility::CMat4x4<T>& OpenUtility::CMat4x4<T>::operator*=(T s)
{
	for (int i=0;i<16;i++) mat[i]*=s;
	return(*this);
}

template<class T>
OpenUtility::CMat4x4<T>& OpenUtility::CMat4x4<T>::operator+=(T s)
{
	for (int i=0;i<16;i++) mat[i]+=s;
	return(*this);
}

template<class T>
OpenUtility::CMat4x4<T>& OpenUtility::CMat4x4<T>::Transpose()
{
	Set(mat[0],mat[4],mat[8],mat[12],
		mat[1],mat[5],mat[9],mat[13],
		mat[2],mat[6],mat[10],mat[14],
		mat[3],mat[7],mat[11],mat[15]);
	return(*this);
}

template<class T>
OpenUtility::CMat4x4<T>& OpenUtility::CMat4x4<T>::Invert()
{
	T det;
	T inv[16];
	int i;

	inv[0] =   mat[5]*mat[10]*mat[15] - mat[5]*mat[11]*mat[14] - mat[9]*mat[6]*mat[15]
			 + mat[9]*mat[7]*mat[14] + mat[13]*mat[6]*mat[11] - mat[13]*mat[7]*mat[10];
	inv[4] =  -mat[4]*mat[10]*mat[15] + mat[4]*mat[11]*mat[14] + mat[8]*mat[6]*mat[15]
			 - mat[8]*mat[7]*mat[14] - mat[12]*mat[6]*mat[11] + mat[12]*mat[7]*mat[10];
	inv[8] =   mat[4]*mat[9]*mat[15] - mat[4]*mat[11]*mat[13] - mat[8]*mat[5]*mat[15]
			 + mat[8]*mat[7]*mat[13] + mat[12]*mat[5]*mat[11] - mat[12]*mat[7]*mat[9];
	inv[12] = -mat[4]*mat[9]*mat[14] + mat[4]*mat[10]*mat[13] + mat[8]*mat[5]*mat[14]
			 - mat[8]*mat[6]*mat[13] - mat[12]*mat[5]*mat[10] + mat[12]*mat[6]*mat[9];
	inv[1] =  -mat[1]*mat[10]*mat[15] + mat[1]*mat[11]*mat[14] + mat[9]*mat[2]*mat[15]
			 - mat[9]*mat[3]*mat[14] - mat[13]*mat[2]*mat[11] + mat[13]*mat[3]*mat[10];
	inv[5] =   mat[0]*mat[10]*mat[15] - mat[0]*mat[11]*mat[14] - mat[8]*mat[2]*mat[15]
			 + mat[8]*mat[3]*mat[14] + mat[12]*mat[2]*mat[11] - mat[12]*mat[3]*mat[10];
	inv[9] =  -mat[0]*mat[9]*mat[15] + mat[0]*mat[11]*mat[13] + mat[8]*mat[1]*mat[15]
			 - mat[8]*mat[3]*mat[13] - mat[12]*mat[1]*mat[11] + mat[12]*mat[3]*mat[9];
	inv[13] =  mat[0]*mat[9]*mat[14] - mat[0]*mat[10]*mat[13] - mat[8]*mat[1]*mat[14]
			 + mat[8]*mat[2]*mat[13] + mat[12]*mat[1]*mat[10] - mat[12]*mat[2]*mat[9];
	inv[2] =   mat[1]*mat[6]*mat[15] - mat[1]*mat[7]*mat[14] - mat[5]*mat[2]*mat[15]
			 + mat[5]*mat[3]*mat[14] + mat[13]*mat[2]*mat[7] - mat[13]*mat[3]*mat[6];
	inv[6] =  -mat[0]*mat[6]*mat[15] + mat[0]*mat[7]*mat[14] + mat[4]*mat[2]*mat[15]
			 - mat[4]*mat[3]*mat[14] - mat[12]*mat[2]*mat[7] + mat[12]*mat[3]*mat[6];
	inv[10] =  mat[0]*mat[5]*mat[15] - mat[0]*mat[7]*mat[13] - mat[4]*mat[1]*mat[15]
			 + mat[4]*mat[3]*mat[13] + mat[12]*mat[1]*mat[7] - mat[12]*mat[3]*mat[5];
	inv[14] = -mat[0]*mat[5]*mat[14] + mat[0]*mat[6]*mat[13] + mat[4]*mat[1]*mat[14]
			 - mat[4]*mat[2]*mat[13] - mat[12]*mat[1]*mat[6] + mat[12]*mat[2]*mat[5];
	inv[3] =  -mat[1]*mat[6]*mat[11] + mat[1]*mat[7]*mat[10] + mat[5]*mat[2]*mat[11]
			 - mat[5]*mat[3]*mat[10] - mat[9]*mat[2]*mat[7] + mat[9]*mat[3]*mat[6];
	inv[7] =   mat[0]*mat[6]*mat[11] - mat[0]*mat[7]*mat[10] - mat[4]*mat[2]*mat[11]
			 + mat[4]*mat[3]*mat[10] + mat[8]*mat[2]*mat[7] - mat[8]*mat[3]*mat[6];
	inv[11] = -mat[0]*mat[5]*mat[11] + mat[0]*mat[7]*mat[9] + mat[4]*mat[1]*mat[11]
			 - mat[4]*mat[3]*mat[9] - mat[8]*mat[1]*mat[7] + mat[8]*mat[3]*mat[5];
	inv[15] =  mat[0]*mat[5]*mat[10] - mat[0]*mat[6]*mat[9] - mat[4]*mat[1]*mat[10]
			 + mat[4]*mat[2]*mat[9] + mat[8]*mat[1]*mat[6] - mat[8]*mat[2]*mat[5];

	det=mat[0]*inv[0] + mat[1]*inv[4] + mat[2]*inv[8] + mat[3]*inv[12];
	if (det==0) return(*this);
	det=1/det;

	for (i=0;i<16;i++)
		mat[i]=inv[i]*det;

	return(*this);
}

template<class T>
OpenUtility::CMat4x4<T>& OpenUtility::CMat4x4<T>::SetFrustum(T left,T right,T bottom,T top,T zNear,T zFar)
{
	Set(2*zNear/(right-left),0,0,0,
		0,2*zNear/(top-bottom),0,0,
		(right+left)/(right-left),(top+bottom)/(top-bottom),-(zFar+zNear)/(zFar-zNear),-1,
		0,0,-(2*zFar*zNear)/(zFar-zNear),0);
	return(*this);
}

template<class T>
OpenUtility::CMat4x4<T>& OpenUtility::CMat4x4<T>::SetLookAt(T eyeX,T eyeY,T eyeZ,T centerX,T centerY,T centerZ,T upX,T upY,T upZ)
{
	XYZd view(centerX-eyeX,centerY-eyeY,centerZ-eyeZ);
	XYZd up(upX,upY,upZ);
	XYZd cross;

	v3dComputeOrthoBase(view,up,cross);
	Set(cross.x,up.x,-view.x,0,
		cross.y,up.y,-view.y,0,
		cross.z,up.z,-view.z,0,
		-eyeX,-eyeY,-eyeZ,1);

	return(*this);
}

template<class T>
OpenUtility::CMat4x4<T>& OpenUtility::CMat4x4<T>::SetTranslate(T tx,T ty,T tz)
{
	SetIdentity();
	mat[12]=tx;
	mat[13]=ty;
	mat[14]=tz;
	return(*this);
}

template<class T>
OpenUtility::CMat4x4<T>& OpenUtility::CMat4x4<T>::SetRotate(T angle,T rx,T ry,T rz)
{
	T t;

	if (fabs((t=rx*rx+ry*ry+rz*rz)-1)<0.000000001)
	{
		if (t==0) return(*this);
		t=sqrt(t);
		rx/=t;
		ry/=t;
		rz/=t;
	}

	T theta=angle/180*M_PI;
	T c=cos(theta);
	T s=sin(theta);
	t=1-c;

	Set(t*rx*rx+c,t*rx*ry+s*rz,t*rx*rz-s*ry,0,
		t*rx*ry-s*rz,t*ry*ry+c,t*ry*rz+s*rx,0,
		t*rx*rz+s*ry,t*ry*rz-s*rx,t*rz*rz+c,0,
		0,0,0,1);
	return(*this);
}

template<class T>
OpenUtility::CMat4x4<T>& OpenUtility::CMat4x4<T>::SetScale(T sx,T sy,T sz)
{
	SetNull();
	mat[0]=sx;
	mat[5]=sy;
	mat[10]=sz;
	mat[15]=1;
	return(*this);
}
