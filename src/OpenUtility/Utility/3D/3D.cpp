#include "3D.h"
#include <stdlib.h>
#include <string.h>

XYf& XYf::operator+=(const XYf &v)
{
	this->x+=v.x;
	this->y+=v.y;

	return(*this);
}

XYf& XYf::operator-=(const XYf &v)
{
	this->x-=v.x;
	this->y-=v.y;

	return(*this);
}

XYf& XYf::operator+=(const float c)
{
	this->x+=c;
	this->y+=c;

	return(*this);
}

XYf& XYf::operator-=(const float c)
{
	this->x-=c;
	this->y-=c;

	return(*this);
}

XYf& XYf::operator*=(const float c)
{
	this->x*=c;
	this->y*=c;

	return(*this);
}

XYf& XYf::operator/=(const float c)
{
	this->x/=c;
	this->y/=c;

	return(*this);
}

XYd& XYd::operator+=(const XYd &v)
{
	this->x+=v.x;
	this->y+=v.y;

	return(*this);
}

XYd& XYd::operator-=(const XYd &v)
{
	this->x-=v.x;
	this->y-=v.y;

	return(*this);
}

XYd& XYd::operator+=(const double c)
{
	this->x+=c;
	this->y+=c;

	return(*this);
}

XYd& XYd::operator-=(const double c)
{
	this->x-=c;
	this->y-=c;

	return(*this);
}

XYd& XYd::operator*=(const double c)
{
	this->x*=c;
	this->y*=c;

	return(*this);
}

XYd& XYd::operator/=(const double c)
{
	this->x/=c;
	this->y/=c;

	return(*this);
}

XYZf& XYZf::operator+=(const XYZf &v)
{
	this->x+=v.x;
	this->y+=v.y;
	this->z+=v.z;

	return(*this);
}

XYZf& XYZf::operator-=(const XYZf &v)
{
	this->x-=v.x;
	this->y-=v.y;
	this->z-=v.z;

	return(*this);
}

XYZf& XYZf::operator+=(const float c)
{
	this->x+=c;
	this->y+=c;
	this->z+=c;

	return(*this);
}

XYZf& XYZf::operator-=(const float c)
{
	this->x-=c;
	this->y-=c;
	this->z-=c;

	return(*this);
}

XYZf& XYZf::operator*=(const float c)
{
	this->x*=c;
	this->y*=c;
	this->z*=c;

	return(*this);
}

XYZf& XYZf::operator/=(const float c)
{
	this->x/=c;
	this->y/=c;
	this->z/=c;

	return(*this);
}

XYZd& XYZd::operator+=(const XYZd &v)
{
	this->x+=v.x;
	this->y+=v.y;
	this->z+=v.z;

	return(*this);
}

XYZd& XYZd::operator-=(const XYZd &v)
{
	this->x-=v.x;
	this->y-=v.y;
	this->z-=v.z;

	return(*this);
}

XYZd& XYZd::operator+=(const double c)
{
	this->x+=c;
	this->y+=c;
	this->z+=c;

	return(*this);
}

XYZd& XYZd::operator-=(const double c)
{
	this->x-=c;
	this->y-=c;
	this->z-=c;

	return(*this);
}

XYZd& XYZd::operator*=(const double c)
{
	this->x*=c;
	this->y*=c;
	this->z*=c;

	return(*this);
}

XYZd& XYZd::operator/=(const double c)
{
	this->x/=c;
	this->y/=c;
	this->z/=c;

	return(*this);
}

bool operator==(const XYZd &v1,const XYZd &v2)
{
	double precision=0.0000000000001;
	return((fabs(v1.x-v2.x)<precision) && (fabs(v1.y-v2.y)<precision) && (fabs(v1.z-v2.z)<precision));
}

bool operator==(const XYZf &v1,const XYZf &v2)
{
	double precision=0.000000001;
	return((fabs(v1.x-v2.x)<precision) && (fabs(v1.y-v2.y)<precision) && (fabs(v1.z-v2.z)<precision));
}

bool operator!=(const XYZd &v1,const XYZd &v2)
{
	return(!(v1==v2));
}

bool operator!=(const XYZf &v1,const XYZf &v2)
{
	return(!(v1==v2));
}

bool operator==(const XYd &v1,const XYd &v2)
{
	double precision=0.0000000000001;
	return((fabs(v1.x-v2.x)<precision) && (fabs(v1.y-v2.y)<precision));
}

bool operator==(const XYf &v1,const XYf &v2)
{
	double precision=0.000000001;
	return((fabs(v1.x-v2.x)<precision) && (fabs(v1.y-v2.y)<precision));
}

bool operator!=(const XYd &v1,const XYd &v2)
{
	return(!(v1==v2));
}

bool operator!=(const XYf &v1,const XYf &v2)
{
	return(!(v1==v2));
}
/*
bool operator==(XYZd &v1,XYZd &v2)
{
	double precision=0.0000000000001;
	return((fabs(v1.x-v2.x)<precision) && (fabs(v1.y-v2.y)<precision) && (fabs(v1.z-v2.z)<precision));
}

bool operator==(XYZf &v1,XYZf &v2)
{
	double precision=0.000000001;
	return((fabs(v1.x-v2.x)<precision) && (fabs(v1.y-v2.y)<precision) && (fabs(v1.z-v2.z)<precision));
}

bool operator!=(XYZd &v1,XYZd &v2)
{
	return(!(v1==v2));
}

bool operator!=(XYZf &v1,XYZf &v2)
{
	return(!(v1==v2));
}*/

XYZf operator+(const XYZf &v1,const XYZf &v2)
{
	XYZf val;

	val.x=v1.x+v2.x;
	val.y=v1.y+v2.y;
	val.z=v1.z+v2.z;

	return(val);
}

XYZd operator+(const XYZd &v1,const XYZd &v2)
{
	XYZd val;

	val.x=v1.x+v2.x;
	val.y=v1.y+v2.y;
	val.z=v1.z+v2.z;

	return(val);
}

XYZf operator+(const XYZf &v1,const float c)
{
	XYZf val;

	val.x=v1.x+c;
	val.y=v1.y+c;
	val.z=v1.z+c;

	return(val);
}

XYZd operator+(const XYZd &v1,const double c)
{
	XYZd val;

	val.x=v1.x+c;
	val.y=v1.y+c;
	val.z=v1.z+c;

	return(val);
}

XYZf operator-(const XYZf &v1,const XYZf &v2)
{
	XYZf val;

	val.x=v1.x-v2.x;
	val.y=v1.y-v2.y;
	val.z=v1.z-v2.z;

	return(val);
}

XYZd operator-(const XYZd &v1,const XYZd &v2)
{
	XYZd val;

	val.x=v1.x-v2.x;
	val.y=v1.y-v2.y;
	val.z=v1.z-v2.z;

	return(val);
}

XYZf operator-(const XYZf &v1,const float c)
{
	XYZf val;

	val.x=v1.x-c;
	val.y=v1.y-c;
	val.z=v1.z-c;

	return(val);
}

XYZd operator-(const XYZd &v1,const double c)
{
	XYZd val;

	val.x=v1.x-c;
	val.y=v1.y-c;
	val.z=v1.z-c;

	return(val);
}

XYZf operator-(const XYZf &v1)
{
	XYZf val;

	val.x=-v1.x;
	val.y=-v1.y;
	val.z=-v1.z;

	return(val);
}

XYZd operator-(const XYZd &v1)
{
	XYZd val;

	val.x=-v1.x;
	val.y=-v1.y;
	val.z=-v1.z;

	return(val);
}

XYZf operator*(const XYZf &v1,const XYZf &v2)
{
	XYZf val;

	val.x=v1.x*v2.x;
	val.y=v1.y*v2.y;
	val.z=v1.z*v2.z;

	return(val);
}

XYZd operator*(const XYZd &v1,const XYZd &v2)
{
	XYZd val;

	val.x=v1.x*v2.x;
	val.y=v1.y*v2.y;
	val.z=v1.z*v2.z;

	return(val);
}

XYZf operator*(const XYZf &v1,const float c)
{
	XYZf val;

	val.x=v1.x*c;
	val.y=v1.y*c;
	val.z=v1.z*c;

	return(val);
}

XYZd operator*(const XYZd &v1,const double c)
{
	XYZd val;

	val.x=v1.x*c;
	val.y=v1.y*c;
	val.z=v1.z*c;

	return(val);
}

XYZf operator/(const XYZf &v1,const XYZf &v2)
{
	XYZf val;

	val.x=v1.x/v2.x;
	val.y=v1.y/v2.y;
	val.z=v1.z/v2.z;

	return(val);
}

XYZd operator/(const XYZd &v1,const XYZd &v2)
{
	XYZd val;

	val.x=v1.x/v2.x;
	val.y=v1.y/v2.y;
	val.z=v1.z/v2.z;

	return(val);
}

XYZf operator/(const XYZf &v1,const float c)
{
	XYZf val;

	val.x=v1.x/c;
	val.y=v1.y/c;
	val.z=v1.z/c;

	return(val);
}

XYZd operator/(const XYZd &v1,const double c)
{
	XYZd val;

	val.x=v1.x/c;
	val.y=v1.y/c;
	val.z=v1.z/c;

	return(val);
}

void RGBAf::Normalize()
{
	unsigned int i;

	for (i=0;i<4;i++)
	{
		if ((*vect[i])>1.0) *vect[i]=1.0;
		if ((*vect[i])<0.0) *vect[i]=0.0;
	}
}

void RGBAf::Init()
{
	vect[0]=&r;
	vect[1]=&g;
	vect[2]=&b;
	vect[3]=&a;
}

void RGBAd::Normalize()
{
	unsigned int i;

	for (i=0;i<4;i++)
	{
		if ((*vect[i])>1.0) *vect[i]=1.0;
		if ((*vect[i])<0.0) *vect[i]=0.0;
	}
}

void RGBAd::Init()
{
	vect[0]=&r;
	vect[1]=&g;
	vect[2]=&b;
	vect[3]=&a;
}

double v3dModule(const XYZd &v)
{
	double tmp;

	tmp=v3dModuleSquare(v);
	return(tmp>0 ? sqrt(tmp) : 0);
}

double v3dModule(double x,double y,double z)
{
	double tmp;

	tmp=v3dModuleSquare(x,y,z);
	return(tmp>0 ? sqrt(tmp) : 0);
}

float v3fModule(const XYZf &v)
{
	float tmp;

	tmp=v3fModuleSquare(v);
	return(tmp>0 ? sqrt(tmp) : 0);
}

float v3fModule(float x,float y,float z)
{
	float tmp;

	tmp=v3fModuleSquare(x,y,z);
	return(tmp>0 ? sqrt(tmp) : 0);
}

double v3dScal(const XYZd &v1,const XYZd &v2)
{
	return(v1.x*v2.x+v1.y*v2.y+v1.z*v2.z);
}

double v3fScal(const XYZf &v1,const XYZf &v2)
{
	return(v1.x*v2.x+v1.y*v2.y+v1.z*v2.z);
}

XYZd* v3dNormalize(XYZd &v)
{
	double norm;
	
	if ((norm=v3dModule(v))!=0)
	{
		v.x/=norm;
		v.y/=norm;
		v.z/=norm;
	}
	return(&v);
}

XYZf* v3fNormalize(XYZf &v)
{
	float norm;

	if ((norm=v3fModule(v))!=0)
	{
		v.x/=norm;
		v.y/=norm;
		v.z/=norm;
	}
	return(&v);
}

XYZd* v3dCrossProd(const XYZd &v,const XYZd &w,XYZd &r)
{
	r.x=v.y*w.z-v.z*w.y;
	r.y=v.z*w.x-v.x*w.z;
	r.z=v.x*w.y-v.y*w.x;
	return(&r);
}

XYZf* v3fCrossProd(const XYZf &v,const XYZf &w,XYZf &r)
{
	r.x=v.y*w.z-v.z*w.y;
	r.y=v.z*w.x-v.x*w.z;
	r.z=v.x*w.y-v.y*w.x;
	return(&r);
}

XYZd* v3dRotate(XYZd &v,const XYZd &r,const double angle)
{
	double theta=angle/180*M_PI;
	double c=cos(theta);
	double s=sin(theta);
	double t=1-c;
	XYZd tmp=v;

	v.x=(t*r.x*r.x+c)*tmp.x + (t*r.x*r.y+s*r.z)*tmp.y + (t*r.x*r.z-s*r.y)*tmp.z;
	v.y=(t*r.x*r.y-s*r.z)*tmp.x + (t*r.y*r.y+c)*tmp.y + (t*r.y*r.z+s*r.x)*tmp.z;
	v.z=(t*r.x*r.z+s*r.y)*tmp.x + (t*r.y*r.z-s*r.x)*tmp.y + (t*r.z*r.z+c)*tmp.z;

	return(&v);
}

XYZd** v3dVRotate(XYZd **v,const unsigned int size,const XYZd &r,const double angle)
{
	double theta=angle/180*M_PI;
	double c=cos(theta);
	double s=sin(theta);
	double t=1-c;
	double m[9];

	m[0]=t*r.x*r.x+c;
	m[1]=t*r.x*r.y+s*r.z;
	m[2]=t*r.x*r.z-s*r.y;
	m[3]=t*r.x*r.y-s*r.z;
	m[4]=t*r.y*r.y+c;
	m[5]=t*r.y*r.z+s*r.x;
	m[6]=t*r.x*r.z+s*r.y;
	m[7]=t*r.y*r.z-s*r.x;
	m[8]=t*r.z*r.z+c;

	for (unsigned int i=0;i<size;i++)
	{
		XYZd tmp=*v[i];

		v[i]->x=m[0]*tmp.x + m[1]*tmp.y + m[2]*tmp.z;
		v[i]->y=m[3]*tmp.x + m[4]*tmp.y + m[5]*tmp.z;
		v[i]->z=m[6]*tmp.x + m[7]*tmp.y + m[8]*tmp.z;
	}

	return(v);
}

XYZf* v3fRotate(XYZf &v,const XYZf &r,const float angle)
{
	float theta=(float)(angle/180*M_PI);
	float c=cos(theta);
	float s=sin(theta);
	float t=1-c;
	XYZf tmp=v;

	v.x=(t*r.x*r.x+c)*tmp.x + (t*r.x*r.y+s*r.z)*tmp.y + (t*r.x*r.z-s*r.y)*tmp.z;
	v.y=(t*r.x*r.y-s*r.z)*tmp.x + (t*r.y*r.y+c)*tmp.y + (t*r.y*r.z+s*r.x)*tmp.z;
	v.z=(t*r.x*r.z+s*r.y)*tmp.x + (t*r.y*r.z-s*r.x)*tmp.y + (t*r.z*r.z+c)*tmp.z;

	return(&v);
}

XYZf** v3fVRotate(XYZf **v,const unsigned int size,const XYZf &r,const float angle)
{
	float theta=(float)(angle/180*M_PI);
	float c=cos(theta);
	float s=sin(theta);
	float t=1-c;
	float m[9];

	m[0]=t*r.x*r.x+c;
	m[1]=t*r.x*r.y+s*r.z;
	m[2]=t*r.x*r.z-s*r.y;
	m[3]=t*r.x*r.y-s*r.z;
	m[4]=t*r.y*r.y+c;
	m[5]=t*r.y*r.z+s*r.x;
	m[6]=t*r.x*r.z+s*r.y;
	m[7]=t*r.y*r.z-s*r.x;
	m[8]=t*r.z*r.z+c;

	for (unsigned int i=0;i<size;i++)
	{
		XYZf tmp=*v[i];

		v[i]->x=m[0]*tmp.x + m[1]*tmp.y + m[2]*tmp.z;
		v[i]->y=m[3]*tmp.x + m[4]*tmp.y + m[5]*tmp.z;
		v[i]->z=m[6]*tmp.x + m[7]*tmp.y + m[8]*tmp.z;
	}

	return(v);
}

double v3dSinAngle(const XYZd &v1,const XYZd &v2)
{
	XYZd r;
	return(v3dModule(*v3dCrossProd(v1,v2,r))/(v3dModule(v1)*v3dModule(v2)));
}

double v3dCosAngle(const XYZd &v1,const XYZd &v2)
{
	return(v3dScal(v1,v2)/(v3dModule(v1)*v3dModule(v2)));
}

double v3dAngle(const XYZd &v1,const XYZd &v2)
{
	return(RTOD*acos(v3dCosAngle(v1,v2)));
}

double v3fSinAngle(const XYZf &v1,const XYZf &v2)
{
	XYZf r;
	return(v3fModule(*v3fCrossProd(v1,v2,r))/(v3fModule(v1)*v3fModule(v2)));
}

double v3fCosAngle(const XYZf &v1,const XYZf &v2)
{
	return(v3fScal(v1,v2)/(v3fModule(v1)*v3fModule(v2)));
}

double v3fAngle(const XYZf &v1,const XYZf &v2)
{
	return(RTOD*acos(v3fCosAngle(v1,v2)));
}

void v3dComputeOrthoBase(XYZd &v1,XYZd &v2,XYZd &v3)
{
	v3dNormalize(v1);
	v3dCrossProd(v1,v2,v3);
	v3dNormalize(v3);
	v3dCrossProd(v3,v1,v2);
	v3dNormalize(v2);
}

void v3dComputeOrthoBase2ndCst(XYZd &v1,XYZd &v2,XYZd &v3)
{
	v3dNormalize(v2);
	v3dCrossProd(v1,v2,v3);
	v3dNormalize(v3);
	v3dCrossProd(v2,v3,v1);
	v3dNormalize(v1);
}

void v3fComputeOrthoBase(XYZf &v1,XYZf &v2,XYZf &v3)
{
	v3fNormalize(v1);
	v3fCrossProd(v1,v2,v3);
	v3fNormalize(v3);
	v3fCrossProd(v3,v1,v2);
	v3fNormalize(v2);
}

void v3fComputeOrthoBase2ndCst(XYZf &v1,XYZf &v2,XYZf &v3)
{
	v3fNormalize(v2);
	v3fCrossProd(v1,v2,v3);
	v3fNormalize(v3);
	v3fCrossProd(v2,v3,v1);
	v3fNormalize(v1);
}

void v3dComputeOrthoBase(SPivotd *pivot)
{
	v3dComputeOrthoBase2ndCst(pivot->dnorm,pivot->dview,pivot->dcros);
}

void v3fComputeOrthoBase(SPivotf *pivot)
{
	v3fComputeOrthoBase2ndCst(pivot->dnorm,pivot->dview,pivot->dcros);
}

unsigned int sdbm_hash(const void *data_in,int size,unsigned int seed=5381)
{
	const unsigned char *data=(const unsigned char*)data_in;
	unsigned int h=seed;
	while (size>0)
	{
		size--;
		h=(h<<16)+(h<<6)-h+(unsigned)data[size];
	}

	return h;
}

unsigned int HashFloat(const float f)
{
	if (f==0) return(0);
	else
	{
		int exponent;
		float mantissa=frexp(f,&exponent);
		return((unsigned int)((2*fabs(mantissa)-1)*~0U));
	}
}

unsigned int HashDouble(const double d)
{
	if (d==0) return(0);
	else
	{
		int exponent;
		double mantissa=frexp(d,&exponent);
		return((unsigned int)((2*fabs(mantissa)-1)*~0U));
	}
}

unsigned int XYf::Hash(const XYf *p)
{
	unsigned int h,temp;
	temp=HashFloat(p->x);
	h=sdbm_hash(&temp,sizeof(unsigned int));
	temp=HashFloat(p->y);
	h=sdbm_hash(&temp,sizeof(unsigned int),h);
	return(h);
}

unsigned int XYd::Hash(const XYd *p)
{
	unsigned int h,temp;
	temp=HashDouble(p->x);
	h=sdbm_hash(&temp,sizeof(unsigned int));
	temp=HashDouble(p->y);
	h=sdbm_hash(&temp,sizeof(unsigned int),h);
	return(h);
}

unsigned int XYZf::Hash(const XYZf *p)
{
	unsigned int h,temp;
	temp=HashFloat(p->x);
	h=sdbm_hash(&temp,sizeof(unsigned int));
	temp=HashFloat(p->y);
	h=sdbm_hash(&temp,sizeof(unsigned int),h);
	temp=HashFloat(p->z);
	h=sdbm_hash(&temp,sizeof(unsigned int),h);
	return(h);
}

unsigned int XYZd::Hash(const XYZd *p)
{
	unsigned int h,temp;
	temp=HashDouble(p->x);
	h=sdbm_hash(&temp,sizeof(unsigned int));
	temp=HashDouble(p->y);
	h=sdbm_hash(&temp,sizeof(unsigned int),h);
	temp=HashDouble(p->z);
	h=sdbm_hash(&temp,sizeof(unsigned int),h);
	return(h);
}
/*CVector3 GetOGLPos(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return CVector3(posX, posY, posZ);
}*/

OpenUtility::CStream& operator<<(OpenUtility::CStream &stream,const XYZf &vect)
{
	stream.AddFormatStream("(%f;%f;%f)",vect.x,vect.y,vect.z);
	return(stream);
}

OpenUtility::CStream& operator<<(OpenUtility::CStream &stream,const XYZd &vect)
{
	stream.AddFormatStream("(%f;%f;%f)",vect.x,vect.y,vect.z);
	return(stream);
}

OpenUtility::CStream& operator<<(OpenUtility::CStream &stream,const SPivotd &pivot)
{
	stream << pivot.GetPos() << pivot.GetdView() << pivot.GetdNorm();
	return(stream);
}

std::ostream& operator<<(std::ostream &os,const XYf &vect)
{
	os << "<" << vect.x << ";" << vect.y << ">";
	return(os);
}

std::ostream& operator<<(std::ostream &os,const XYd &vect)
{
	os << "<" << vect.x << ";" << vect.y << ">";
	return(os);
}

std::ostream& operator<<(std::ostream &os,const XYZf &vect)
{
	os << "<" << vect.x << ";" << vect.y << ";" << vect.z << ">";
	return(os);
}

std::ostream& operator<<(std::ostream &os,const XYZd &vect)
{
	os << "<" << vect.x << ";" << vect.y << ";" << vect.z << ">";
	return(os);
}

std::ostream& operator<<(std::ostream &os,const SPivotf &pivot)
{
	os << "Pos" << pivot.GetPos() << " dView" << pivot.GetdView() << " dNorm" << pivot.GetdNorm() << " dCross" << pivot.GetdCros();
	return(os);
}

std::ostream& operator<<(std::ostream &os,const SPivotd &pivot)
{
	os << "Pos" << pivot.GetPos() << " dView" << pivot.GetdView() << " dNorm" << pivot.GetdNorm() << " dCross" << pivot.GetdCros();
	return(os);
}


SPivotd::SPivotd() :
	dview(0.0,0.0,1.0),
	dnorm(0.0,1.0,0.0)
{
	v3dComputeOrthoBase(this);
	ComputeMatrix();
}

SPivotd::SPivotd(const SPivotd &obj) :
	pos(obj.pos),
	dview(obj.dview),
	dnorm(obj.dnorm),
	dcros(obj.dcros)
{
	memcpy(matrix,obj.matrix,16*sizeof(double));
	memcpy(rotmatrix,obj.rotmatrix,16*sizeof(double));
}

SPivotd& SPivotd::operator=(const SPivotd &obj)
{
	pos=obj.pos;
	dview=obj.dview;
	dnorm=obj.dnorm;
	dcros=obj.dcros;
	memcpy(matrix,obj.matrix,16*sizeof(double));
	memcpy(rotmatrix,obj.rotmatrix,16*sizeof(double));
	return(*this);
}

bool SPivotd::operator==(const SPivotd &obj)
{
	return((pos==obj.pos) && (dview==obj.dview) && (dnorm==obj.dnorm));
}

SPivotd::SPivotd(XYZd p,XYZd dv,XYZd dn) :
	pos(p),
	dview(dv),
	dnorm(dn)
{
	v3dComputeOrthoBase(this);
	ComputeMatrix();
}

void SPivotd::Set(XYZd p,XYZd dv,XYZd dn)
{
	pos=p;
	dview=dv;
	dnorm=dn;
	v3dComputeOrthoBase(this);
	ComputeMatrix();
}

void SPivotd::SetPos(XYZd& newPos)
{
	pos=newPos;
	ComputeMatrix();
}

void SPivotd::SetPos(double x,double y,double z)
{
	pos.x=x;
	pos.y=y;
	pos.z=z;
	ComputeMatrix();
}

void SPivotd::SetOrientation(XYZd& newdView,XYZd& newdNorm)
{
	dview=newdView;
	dnorm=newdNorm;
	v3dComputeOrthoBase(this);
	ComputeMatrix();
}

void SPivotd::SetOrientation(double dViewX,double dViewY,double dViewZ,double dNormX,double dNormY,double dNormZ)
{
	dview.x=dViewX;
	dview.y=dViewY;
	dview.z=dViewZ;
	dnorm.x=dNormX;
	dnorm.y=dNormY;
	dnorm.z=dNormZ;
	v3dComputeOrthoBase(this);
	ComputeMatrix();
}

void SPivotd::ComputeMatrix()
{
#define M(row,col) matrix[(col<<2)+row]
#define MR(row,col) rotmatrix[(col<<2)+row]
	MR(0,0)=M(0,0)=dcros.x;
	MR(0,1)=M(0,1)=dnorm.x;
	MR(0,2)=M(0,2)=dview.x;
	MR(0,3)=0.0;
	M(0,3)=pos.x;
	MR(1,0)=M(1,0)=dcros.y;
	MR(1,1)=M(1,1)=dnorm.y;
	MR(1,2)=M(1,2)=dview.y;
	MR(1,3)=0.0;
	M(1,3)=pos.y;
	MR(2,0)=M(2,0)=dcros.z;
	MR(2,1)=M(2,1)=dnorm.z;
	MR(2,2)=M(2,2)=dview.z;
	MR(2,3)=0.0;
	M(2,3)=pos.z;
	MR(3,0)=M(3,0)=0.0;
	MR(3,1)=M(3,1)=0.0;
	MR(3,2)=M(3,2)=0.0;
	MR(3,3)=M(3,3)=1.0;
#undef MR
#undef M
}

SPivotf::SPivotf() :
	dview(0.0,0.0,1.0),
	dnorm(0.0,1.0,0.0)
{
	v3fComputeOrthoBase(this);
	ComputeMatrix();
}

SPivotf::SPivotf(const SPivotf &obj) :
	pos(obj.pos),
	dview(obj.dview),
	dnorm(obj.dnorm),
	dcros(obj.dcros)
{
	memcpy(matrix,obj.matrix,16*sizeof(float));
	memcpy(rotmatrix,obj.rotmatrix,16*sizeof(float));
}

SPivotf& SPivotf::operator=(const SPivotf &obj)
{
	pos=obj.pos;
	dview=obj.dview;
	dnorm=obj.dnorm;
	dcros=obj.dcros;
	memcpy(matrix,obj.matrix,16*sizeof(float));
	memcpy(rotmatrix,obj.rotmatrix,16*sizeof(float));
	return(*this);
}

bool SPivotf::operator==(const SPivotf &obj)
{
	return((pos==obj.pos) && (dview==obj.dview) && (dnorm==obj.dnorm));
}

SPivotf::SPivotf(XYZf p,XYZf dv,XYZf dn) :
	pos(p),
	dview(dv),
	dnorm(dn)
{
	v3fComputeOrthoBase(this);
	ComputeMatrix();
}

void SPivotf::Set(XYZf p,XYZf dv,XYZf dn)
{
	pos=p;
	dview=dv;
	dnorm=dn;
	v3fComputeOrthoBase(this);
	ComputeMatrix();
}

void SPivotf::SetPos(XYZf& newPos)
{
	pos=newPos;
	ComputeMatrix();
}

void SPivotf::SetPos(float x,float y,float z)
{
	pos.x=x;
	pos.y=y;
	pos.z=z;
	ComputeMatrix();
}

void SPivotf::SetOrientation(XYZf& newdView,XYZf& newdNorm)
{
	dview=newdView;
	dnorm=newdNorm;
	v3fComputeOrthoBase(this);
	ComputeMatrix();
}

void SPivotf::SetOrientation(float dViewX,float dViewY,float dViewZ,float dNormX,float dNormY,float dNormZ)
{
	dview.x=dViewX;
	dview.y=dViewY;
	dview.z=dViewZ;
	dnorm.x=dNormX;
	dnorm.y=dNormY;
	dnorm.z=dNormZ;
	v3fComputeOrthoBase(this);
	ComputeMatrix();
}

void SPivotf::ComputeMatrix()
{
#define M(row,col) matrix[(col<<2)+row]
#define MR(row,col) rotmatrix[(col<<2)+row]
	MR(0,0)=M(0,0)=dcros.x;
	MR(0,1)=M(0,1)=dnorm.x;
	MR(0,2)=M(0,2)=dview.x;
	MR(0,3)=0.0;
	M(0,3)=pos.x;
	MR(1,0)=M(1,0)=dcros.y;
	MR(1,1)=M(1,1)=dnorm.y;
	MR(1,2)=M(1,2)=dview.y;
	MR(1,3)=0.0;
	M(1,3)=pos.y;
	MR(2,0)=M(2,0)=dcros.z;
	MR(2,1)=M(2,1)=dnorm.z;
	MR(2,2)=M(2,2)=dview.z;
	MR(2,3)=0.0;
	M(2,3)=pos.z;
	MR(3,0)=M(3,0)=0.0;
	MR(3,1)=M(3,1)=0.0;
	MR(3,2)=M(3,2)=0.0;
	MR(3,3)=M(3,3)=1.0;
#undef MR
#undef M
}

void Matrix44Mul44(double *product,const double *a,const double *b)
{
#define A(row,col) a[(col<<2)+row]
#define B(row,col) b[(col<<2)+row]
#define P(row,col) product[(col<<2)+row]

	for (unsigned int i=0;i<4;i++)
	{
		const double ai0=A(i,0),ai1=A(i,1),ai2=A(i,2),ai3=A(i,3);

		P(i,0)=ai0*B(0,0)+ai1*B(1,0)+ai2*B(2,0)+ai3*B(3,0);
		P(i,1)=ai0*B(0,1)+ai1*B(1,1)+ai2*B(2,1)+ai3*B(3,1);
		P(i,2)=ai0*B(0,2)+ai1*B(1,2)+ai2*B(2,2)+ai3*B(3,2);
		P(i,3)=ai0*B(0,3)+ai1*B(1,3)+ai2*B(2,3)+ai3*B(3,3);
	}

#undef P
#undef B
#undef A
}

void Matrix34Mul31(XYZd *product,const double *a,const XYZd *b)
{
#define A(row,col) a[(col<<2)+row]
#define P(row,col) product[(col<<2)+row]

	product->x=A(0,0)*b->x+A(0,1)*b->y+A(0,2)*b->z+A(0,3);
	product->y=A(1,0)*b->x+A(1,1)*b->y+A(1,2)*b->z+A(1,3);
	product->z=A(2,0)*b->x+A(2,1)*b->y+A(2,2)*b->z+A(2,3);

#undef P
#undef A
}

void Matrix44Mul31(XYZd *product,const double *a,const XYZd *b)
{
#define A(row,col) a[(col<<2)+row]
#define P(row,col) product[(col<<2)+row]

	double w;

	w=A(3,0)*b->x+A(3,1)*b->y+A(3,2)*b->z+A(3,3);
	product->x=(A(0,0)*b->x+A(0,1)*b->y+A(0,2)*b->z+A(0,3))/w;
	product->y=(A(1,0)*b->x+A(1,1)*b->y+A(1,2)*b->z+A(1,3))/w;
	product->z=(A(2,0)*b->x+A(2,1)*b->y+A(2,2)*b->z+A(2,3))/w;

#undef P
#undef A
}
