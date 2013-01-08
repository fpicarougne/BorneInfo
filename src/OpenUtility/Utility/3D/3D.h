#ifndef _3D_h
	#define _3D_h

#define _USE_MATH_DEFINES
#include <math.h>
#include "../Stream/Stream.h"
#include <iostream>

#define DTOR 0.017453292519943295769236907684886
#define RTOD 57.295779513082320876798154814105

struct SFrustum
{
	double pLeft,pRight,pTop,pBottom,pNear,pFar;
};

class SPivotd;
class SPivotf;
struct XYf;
struct XYd;
struct XYZf;
struct XYZd;

bool operator==(const XYd &v1,const XYd &v2);
bool operator==(const XYf &v1,const XYf &v2);
bool operator!=(const XYd &v1,const XYd &v2);
bool operator!=(const XYf &v1,const XYf &v2);
bool operator==(const XYZd &v1,const XYZd &v2);
bool operator==(const XYZf &v1,const XYZf &v2);
bool operator!=(const XYZd &v1,const XYZd &v2);
bool operator!=(const XYZf &v1,const XYZf &v2);

XYZf operator+(const XYZf &v1,const XYZf &v2);
XYZd operator+(const XYZd &v1,const XYZd &v2);
XYZf operator+(const XYZf &v1,const float c);
XYZd operator+(const XYZd &v1,const double c);

XYZf operator-(const XYZf &v1,const XYZf &v2);
XYZd operator-(const XYZd &v1,const XYZd &v2);
XYZf operator-(const XYZf &v1,const float c);
XYZd operator-(const XYZd &v1,const double c);
XYZf operator-(const XYZf &v1);
XYZd operator-(const XYZd &v1);

XYZf operator*(const XYZf &v1,const XYZf &v2);
XYZd operator*(const XYZd &v1,const XYZd &v2);
XYZf operator*(const XYZf &v1,const float c);
XYZd operator*(const XYZd &v1,const double c);

XYZf operator/(const XYZf &v1,const XYZf &v2);
XYZd operator/(const XYZd &v1,const XYZd &v2);
XYZf operator/(const XYZf &v1,const float c);
XYZd operator/(const XYZd &v1,const double c);

struct XYf
{
	XYf() : x(0.0f),y(0.0f) {vect[0]=&x;vect[1]=&y;}
	XYf(float a,float b) : x(a),y(b) {vect[0]=&x;vect[1]=&y;}
	XYf(const XYf &s) : x(s.x),y(s.y) {vect[0]=&x;vect[1]=&y;}
	void Set(float a,float b) {x=a;y=b;vect[0]=&x;vect[1]=&y;}
	XYf& operator=(const XYf &v) {x=v.x;y=v.y;return(*this);}
	XYf& operator+=(const XYf &v);
	XYf& operator-=(const XYf &v);
	XYf& operator+=(const float c);
	XYf& operator-=(const float c);
	XYf& operator*=(const float c);
	XYf& operator/=(const float c);
	static int CompareHash(XYf &v1,XYf &v2) {return(CompareHash(const_cast<const XYf&>(v1),const_cast<const XYf&>(v2)));}
	static int CompareHash(const XYf &v1,const XYf &v2) {return(v1==v2 ? 0 : 1);}
	static unsigned int Hash(XYf *p) {return(Hash(const_cast<const XYf*>(p)));}
	static unsigned int Hash(const XYf *p);
	float x,y;
	float *vect[2];
};

struct XYd
{
	XYd() : x(0.0),y(0.0) {vect[0]=&x;vect[1]=&y;}
	XYd(double a,double b) : x(a),y(b) {vect[0]=&x;vect[1]=&y;}
	XYd(const XYd &s) : x(s.x),y(s.y) {vect[0]=&x;vect[1]=&y;}
	XYd(const XYf &s) : x(s.x),y(s.y) {vect[0]=&x;vect[1]=&y;}
	void Set(double a,double b) {x=a;y=b;vect[0]=&x;vect[1]=&y;}
	XYd& operator=(const XYd &v) {x=v.x;y=v.y;return(*this);}
	XYd& operator=(const XYf &v) {x=v.x;y=v.y;return(*this);}
	XYd& operator+=(const XYd &v);
	XYd& operator-=(const XYd &v);
	XYd& operator+=(const double c);
	XYd& operator-=(const double c);
	XYd& operator*=(const double c);
	XYd& operator/=(const double c);
	static int CompareHash(XYd &v1,XYd &v2) {return(CompareHash(const_cast<const XYd&>(v1),const_cast<const XYd&>(v2)));}
	static int CompareHash(const XYd &v1,const XYd &v2) {return(v1==v2 ? 0 : 1);}
	static unsigned int Hash(XYd *p) {return(Hash(const_cast<const XYd*>(p)));}
	static unsigned int Hash(const XYd *p);
	double x,y;
	double *vect[2];
};

struct XYZf
{
	XYZf() : x(0.0f),y(0.0f),z(0.0f) {vect[0]=&x;vect[1]=&y;vect[2]=&z;}
	XYZf(float a,float b,float c) : x(a),y(b),z(c) {vect[0]=&x;vect[1]=&y;vect[2]=&z;}
	XYZf(const XYZf &s) : x(s.x),y(s.y),z(s.z) {vect[0]=&x;vect[1]=&y;vect[2]=&z;}
	void Set(float a,float b,float c) {x=a;y=b;z=c;vect[0]=&x;vect[1]=&y;vect[2]=&z;}
	XYZf& operator=(const XYZf &v) {x=v.x;y=v.y;z=v.z;return(*this);}
	XYZf& operator+=(const XYZf &v);
	XYZf& operator-=(const XYZf &v);
	XYZf& operator+=(const float c);
	XYZf& operator-=(const float c);
	XYZf& operator*=(const float c);
	XYZf& operator/=(const float c);
	static int CompareHash(XYZf &v1,XYZf &v2) {return(CompareHash(const_cast<const XYZf&>(v1),const_cast<const XYZf&>(v2)));}
	static int CompareHash(const XYZf &v1,const XYZf &v2) {return(v1==v2 ? 0 : 1);}
	static unsigned int Hash(XYZf *p) {return(Hash(const_cast<const XYZf*>(p)));}
	static unsigned int Hash(const XYZf *p);
	float x,y,z;
	float *vect[3];
};

struct XYZd
{
	XYZd() : x(0.0),y(0.0),z(0.0) {vect[0]=&x;vect[1]=&y;vect[2]=&z;}
	XYZd(double a,double b,double c) : x(a),y(b),z(c) {vect[0]=&x;vect[1]=&y;vect[2]=&z;}
	XYZd(const XYZd &s) : x(s.x),y(s.y),z(s.z) {vect[0]=&x;vect[1]=&y;vect[2]=&z;}
	XYZd(const XYZf &s) : x(s.x),y(s.y),z(s.z) {vect[0]=&x;vect[1]=&y;vect[2]=&z;}
	void Set(double a,double b,double c) {x=a;y=b;z=c;vect[0]=&x;vect[1]=&y;vect[2]=&z;}
	XYZd& operator=(const XYZd &v) {x=v.x;y=v.y;z=v.z;return(*this);}
	XYZd& operator=(const XYZf &v) {x=v.x;y=v.y;z=v.z;return(*this);}
	XYZd& operator+=(const XYZd &v);
	XYZd& operator-=(const XYZd &v);
	XYZd& operator+=(const double c);
	XYZd& operator-=(const double c);
	XYZd& operator*=(const double c);
	XYZd& operator/=(const double c);
	static int CompareHash(XYZd &v1,XYZd &v2) {return(CompareHash(const_cast<const XYZd&>(v1),const_cast<const XYZd&>(v2)));}
	static int CompareHash(const XYZd &v1,const XYZd &v2) {return(v1==v2 ? 0 : 1);}
	static unsigned int Hash(XYZd *p) {return(Hash(const_cast<const XYZd*>(p)));}
	static unsigned int Hash(const XYZd *p);
	double x,y,z;
	double *vect[3];
};

inline XYZd operator+(const double c,const XYZd &v1) {return(operator+(v1,c));}
inline XYZf operator+(const float c,const XYZf &v1) {return(operator+(v1,c));}
inline XYZf operator-(const float c,const XYZf &v1) {return(operator-(v1,c));}
inline XYZd operator-(const double c,const XYZd &v1) {return(operator-(v1,c));}
inline XYZf operator*(const float c,const XYZf &v1) {return(operator*(v1,c));}
inline XYZd operator*(const double c,const XYZd &v1) {return(operator*(v1,c));}

inline double v3dModuleSquare(const XYZd &v) {return(v.x*v.x+v.y*v.y+v.z*v.z);}
inline double v3dModuleSquare(double x,double y,double z) {return(x*x+y*y+z*z);}
inline float v3fModuleSquare(const XYZf &v) {return(v.x*v.x+v.y*v.y+v.z*v.z);}
inline float v3fModuleSquare(float x,float y,float z) {return(x*x+y*y+z*z);}
double v3dModule(const XYZd &v);
double v3dModule(double x,double y,double z);
float v3fModule(const XYZf &v);
float v3fModule(float x,float y,float z);
double v3dScal(const XYZd &v1,const XYZd &v2);
double v3fScal(const XYZf &v1,const XYZf &v2);
XYZd* v3dNormalize(XYZd &v);
XYZf* v3fNormalize(XYZf &v);
XYZd* v3dCrossProd(const XYZd &v,const XYZd &w,XYZd &r);
XYZf* v3fCrossProd(const XYZf &v,const XYZf &w,XYZf &r);
XYZd* v3dRotate(XYZd &v,const XYZd &r,const double angle);
XYZd** v3dVRotate(XYZd **v,const unsigned int size,const XYZd &r,const double angle);
XYZf* v3dRotate(XYZf &v,const XYZf &r,const float angle);
XYZf** v3fVRotate(XYZf **v,const unsigned int size,const XYZf &r,const float angle);
double v3dSinAngle(const XYZd &v1,const XYZd &v2);
double v3dCosAngle(const XYZd &v1,const XYZd &v2);
double v3dAngle(const XYZd &v1,const XYZd &v2);
double v3fSinAngle(const XYZf &v1,const XYZf &v2);
double v3fCosAngle(const XYZf &v1,const XYZf &v2);
double v3fAngle(const XYZf &v1,const XYZf &v2);
void v3dComputeOrthoBase(XYZd &v1,XYZd &v2,XYZd &v3);
void v3dComputeOrthoBase2ndCst(XYZd &v1,XYZd &v2,XYZd &v3);
void v3fComputeOrthoBase(XYZf &v1,XYZf &v2,XYZf &v3);
void v3fComputeOrthoBase2ndCst(XYZd &v1,XYZd &v2,XYZd &v3);
void v3dComputeOrthoBase(SPivotd *pivot);
void v3fComputeOrthoBase(SPivotf *pivot);

class RGBAd
{
public:
	RGBAd() : r(0.0f),g(0.0f),b(0.0f),a(0.0f) {Init();}
	RGBAd(double w,double x,double y,double z) : r(w),g(x),b(y),a(z) {Init();Normalize();}
	RGBAd(RGBAd &s) : r(s.r),g(s.g),b(s.b),a(s.a) {Init();Normalize();}
	RGBAd& operator=(const RGBAd &v) {r=v.r;g=v.g;b=v.b;a=v.a;return(*this);}
	void Set(double w=1.0,double x=1.0,double y=1.0,double z=1.0) {r=w;g=x;b=y;a=z;Normalize();}
	double r,g,b,a;
	double *vect[4];

private:
	void Normalize();
	void Init();
};

class RGBAf
{
public:
	RGBAf() : r(0.0f),g(0.0f),b(0.0f),a(0.0f) {Init();}
	RGBAf(float w,float x,float y,float z) : r(w),g(x),b(y),a(z) {Init();Normalize();}
	RGBAf(RGBAf &s) : r(s.r),g(s.g),b(s.b),a(s.a) {Init();Normalize();}
	RGBAf& operator=(const RGBAf &v) {r=v.r;g=v.g;b=v.b;a=v.a;return(*this);}
	void Set(float w=1.0,float x=1.0,float y=1.0,float z=1.0) {r=w;g=x;b=y;a=z;Normalize();}
	float r,g,b,a;
	float *vect[4];

private:
	void Normalize();
	void Init();
};

class SPivotd
{
public:
	SPivotd();
	SPivotd(XYZd p,XYZd dv,XYZd dn);
	SPivotd(const SPivotd &obj);
	SPivotd& operator=(const SPivotd &obj);
	bool operator==(const SPivotd &obj);
	inline bool operator!=(const SPivotd &obj) {return(!this->operator==(obj));}
	void Set(XYZd p,XYZd dv,XYZd dn);
	void SetPos(XYZd& newPos);
	void SetPos(double x,double y,double z);
	void SetOrientation(XYZd& newdView,XYZd& newdNorm);
	void SetOrientation(double dViewX,double dViewY,double dViewZ,double dNormX,double dNormY,double dNormZ);
	inline const XYZd& GetPos() const {return(pos);}
	inline const XYZd& GetdView() const {return(dview);}
	inline const XYZd& GetdNorm() const {return(dnorm);}
	inline const XYZd& GetdCros() const {return(dcros);}
	inline const double* GetMatrix() const {return(matrix);}
	inline const double* GetRotMatrix() const {return(rotmatrix);}
	friend void v3dComputeOrthoBase(SPivotd *pivot);

private:
	void ComputeMatrix();

private:
	XYZd pos;
	XYZd dview;
	XYZd dnorm;
	XYZd dcros;
	double matrix[16];
	double rotmatrix[16];
};

class SPivotf
{
public:
	SPivotf();
	SPivotf(XYZf p,XYZf dv,XYZf dn);
	SPivotf(const SPivotf &obj);
	SPivotf& operator=(const SPivotf &obj);
	bool operator==(const SPivotf &obj);
	inline bool operator!=(const SPivotf &obj) {return(!this->operator==(obj));}
	void Set(XYZf p,XYZf dv,XYZf dn);
	void SetPos(XYZf& newPos);
	void SetPos(float x,float y,float z);
	void SetOrientation(XYZf& newdView,XYZf& newdNorm);
	void SetOrientation(float dViewX,float dViewY,float dViewZ,float dNormX,float dNormY,float dNormZ);
	inline const XYZf& GetPos() const {return(pos);}
	inline const XYZf& GetdView() const {return(dview);}
	inline const XYZf& GetdNorm() const {return(dnorm);}
	inline const XYZf& GetdCros() const {return(dcros);}
	inline const float* GetMatrix() const {return(matrix);}
	inline const float* GetRotMatrix() const {return(rotmatrix);}
	friend void v3fComputeOrthoBase(SPivotf *pivot);

private:
	void ComputeMatrix();

private:
	XYZf pos;
	XYZf dview;
	XYZf dnorm;
	XYZf dcros;
	float matrix[16];
	float rotmatrix[16];
};

OpenUtility::CStream& operator<<(OpenUtility::CStream &stream,XYZf &vect);
OpenUtility::CStream& operator<<(OpenUtility::CStream &stream,XYZd &vect);
OpenUtility::CStream& operator<<(OpenUtility::CStream &stream,SPivotd &pivot);
std::ostream& operator<<(std::ostream &os,const XYf &vect);
std::ostream& operator<<(std::ostream &os,const XYd &vect);
std::ostream& operator<<(std::ostream &os,const XYZf &vect);
std::ostream& operator<<(std::ostream &os,const XYZd &vect);
std::ostream& operator<<(std::ostream &os,const SPivotf &pivot);
std::ostream& operator<<(std::ostream &os,const SPivotd &pivot);

void Matrix44Mul44(double *product,const double *a,const double *b);
void Matrix34Mul31(XYZd *product,const double *a,const XYZd *b);
void Matrix44Mul31(XYZd *product,const double *a,const XYZd *b);

#endif