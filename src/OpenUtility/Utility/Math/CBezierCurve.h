#ifndef CBezierCurve_h
	#define CBezierCurve_h

namespace OpenUtility
{

class CBezierCurve
{
public:
	CBezierCurve(unsigned int dimention);
	~CBezierCurve();
	void FlushPoints();
	void AddPointsVectors(float *p,float *v,unsigned int nb=1);
	void AddPointsVectors(float *pv,unsigned int nb=1);
	bool GetPoint(float t,float *p);
	bool GetPoint(float t,float *p,unsigned int dimRequest);
	bool GetDerivePoint(float t,float *p);
	bool GetDerivePoint(float t,float *p,unsigned int dimRequest);

private:
	// Stockage Point-Vecteur-Point-Vecteur ...
	float *Points;
	unsigned int NbPoints;
	unsigned int dim;
};

}

#endif
