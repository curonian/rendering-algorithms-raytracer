#include "Lambert.h"
#include "Ray.h"
#include "Scene.h"

Lambert::Lambert(const Vector3 & kd, const Vector3 & ka) :
    m_kd(kd), m_ka(ka)
{

}

Lambert::~Lambert()
{
}

Vector3
Lambert::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{
    Vector3 L = Vector3(0.0f, 0.0f, 0.0f);
    
	Vector3 rayD = Vector3(ray.dx, ray.dy, ray.dz);
    
    const Lights *lightlist = scene.lights();
    
    // loop over all of the lights
    Lights::const_iterator lightIter;
    for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
    {
        PointLight* pLight = *lightIter;
    
		Vector3 P = Vector3(hit.px, hit.py, hit.pz);
        Vector3 l = pLight->position() - P;
        
        // the inverse-squared falloff
        float falloff = l.length2();
        
        // normalize the light direction
        l /= sqrt(falloff);

        // get the diffuse component
        float nDotL = dot(hit.N, l);
        Vector3 result = pLight->color();
        result *= m_kd;
        
        L += std::max(0.0f, nDotL/falloff * pLight->wattage() / PI) * result;
    }
    
	if (m_texture != NULL) {
		Vector4 texCol = m_texture->getLookup(hit.a, hit.b);
		L = L*Vector3(texCol.x, texCol.y, texCol.z);
	}
    // add the ambient component
    L += m_ka;
    
    return L;
}
