#ifndef CSE168_MATERIAL_H_INCLUDED
#define CSE168_MATERIAL_H_INCLUDED

#include "Miro.h"
#include "Vector3.h"
#include "Texture.h"

class Material
{
public:
    Material();
    virtual ~Material();

    virtual void preCalc() {}
    
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;
	Texture* m_texture;

	static float fresnel(float n1, float n2, float cosThetaI) {		// Fn to compute fresnel coefficients.
		float n1CosTh = n1*cosThetaI;
		float n1_n2SinTh = n1*sin(acosf(cosThetaI))/n2;
		float n2CosTh = n2*std::max(0.0f, sqrt(1.0f - n1_n2SinTh*n1_n2SinTh));
		float Rs = (n1CosTh - n2CosTh) / (n1CosTh + n2CosTh);
		return Rs*Rs;
	}
};

#endif // CSE168_MATERIAL_H_INCLUDED