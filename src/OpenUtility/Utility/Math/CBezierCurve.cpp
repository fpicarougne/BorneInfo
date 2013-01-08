#include "CBezierCurve.h"
#include "../Memory.h"
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <math.h>

OpenUtility::CBezierCurve::CBezierCurve(unsigned int dimention) :
	Points(NULL),
	NbPoints(0),
	dim(dimention)
{
}

OpenUtility::CBezierCurve::~CBezierCurve()
{
	OpenUtility::MyFree(Points);
}

void OpenUtility::CBezierCurve::FlushPoints()
{
	OpenUtility::MyFree(Points);
	Points=NULL;
	NbPoints=0;
}

void OpenUtility::CBezierCurve::AddPointsVectors(float *p,float *v,unsigned int nb)
{
	unsigned int i;

	Points=(float*)OpenUtility::MyRealloc(Points,(NbPoints+nb)*2*dim*sizeof(float));
	for (i=0;i<nb;i++)
	{
		memcpy(&Points[(NbPoints+i)*2*dim],&p[i*dim],dim*sizeof(float));
		memcpy(&Points[(NbPoints+i)*2*dim+dim],&v[i*dim],dim*sizeof(float));
	}
	NbPoints+=nb;
}

void OpenUtility::CBezierCurve::AddPointsVectors(float *pv,unsigned int nb)
{
	Points=(float*)OpenUtility::MyRealloc(Points,(NbPoints+nb)*2*dim*sizeof(float));
	memcpy(&Points[NbPoints*2*dim],pv,nb*2*dim*sizeof(float));
	NbPoints+=nb;
}

bool OpenUtility::CBezierCurve::GetPoint(float t,float *p)
{
	unsigned int i;

	for (i=0;i<dim;i++)
		if (!GetPoint(t,p,i)) return(false);
	return(true);
}

bool OpenUtility::CBezierCurve::GetPoint(float t,float *p,unsigned int dimRequest)
{
	unsigned int nb;

	if (dimRequest>=dim) return(false);

	nb=(int)t;
	if (nb+2>NbPoints)
	{
		if (t==(float)NbPoints-1) nb--;
		else return(false);
	}

	p[dimRequest]=	Points[nb*2*dim+dimRequest]*pow(1-(t-nb),3)+
					(Points[nb*2*dim+dimRequest]-Points[nb*2*dim+dim+dimRequest])*3*(t-nb)*pow(1-(t-nb),2)+
					(Points[(nb+1)*2*dim+dimRequest]+Points[(nb+1)*2*dim+dim+dimRequest])*3*pow((t-nb),2)*(1-(t-nb))+
					Points[(nb+1)*2*dim+dimRequest]*pow((t-nb),3);

	return(true);
}

bool OpenUtility::CBezierCurve::GetDerivePoint(float t,float *p)
{
	unsigned int i;

	for (i=0;i<dim;i++)
		if (!GetDerivePoint(t,p,i)) return(false);
	return(true);
}

bool OpenUtility::CBezierCurve::GetDerivePoint(float t,float *p,unsigned int dimRequest)
{
	unsigned int nb;

	if (dimRequest>=dim) return(false);

	nb=(int)t;
	if (nb+2>NbPoints)
	{
		if (t==(float)NbPoints-1) nb--;
		else return(false);
	}

	p[dimRequest]=-3*(Points[nb*2*dim+dimRequest]*(pow((t-nb),2)-2*(t-nb)+1)-
			(Points[nb*2*dim+dimRequest]-Points[nb*2*dim+dim+dimRequest])*(3*pow((t-nb),2)-4*(t-nb)+1)+
			(t-nb)*((Points[(nb+1)*2*dim+dimRequest]+Points[(nb+1)*2*dim+dim+dimRequest])*(3*(t-nb)-2)-
			Points[(nb+1)*2*dim+dimRequest]*(t-nb)));

	return(true);
}
