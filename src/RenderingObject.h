//coding: utf-8
#ifndef _RenderingObject_h
	#define _RenderingObject_h

#include <Utility/3D/3D.h>
#include <Utility/3D/CShaderProgram.h>
#include <Template/CMat4x4.h>

class IRenderingObject;

class IRenderingObjectComm
{
public:
	IRenderingObjectComm() {}
	virtual ~IRenderingObjectComm() {}
	virtual const OpenUtility::SFrustum& GetFrustum() const =0;
	virtual const OpenUtility::SPivotd& GetCameraPos() const =0;
	virtual const OpenUtility::CShaderProgram& GetGlobalShader() const =0;
	virtual void OnEndAnim(IRenderingObject *obj)=0;
	virtual unsigned long long GetTimeUnit()=0;
};

class IRenderingObject
{
public:
	IRenderingObject(IRenderingObjectComm *obj) : pClass(obj) {}
	virtual ~IRenderingObject() {}
	virtual void Init()=0;
	virtual void UnInit()=0;
	virtual bool PreRender()=0;
	virtual void Render()=0;
	virtual void SetMatrix(OpenUtility::CMat4x4<float> &p,OpenUtility::CMat4x4<float> &mv,OpenUtility::CMat4x4<float> &n) {ProjectionMatrix=p;ModelViewMatrix=mv;NormalMatrix=n;}
	virtual bool UseGlobalShaderProgram() {return(true);}
    virtual bool TestImpact(double x,double y) {return(false);}

protected:
	IRenderingObjectComm *pClass;
	OpenUtility::CMat4x4<float> ProjectionMatrix;
	OpenUtility::CMat4x4<float> ModelViewMatrix;
	OpenUtility::CMat4x4<float> NormalMatrix;
};

#endif
