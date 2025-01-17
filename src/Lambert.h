#ifndef CSE168_LAMBERT_H_INCLUDED
#define CSE168_LAMBERT_H_INCLUDED

#include "Material.h"

class Lambert : public Material
{
public:
    Lambert(const Vector3 & kd = Vector3(1),
            const Vector3 & ka = Vector3(0));
    virtual ~Lambert();

    const Vector3 & kd() const {return m_kd;}
    const Vector3 & ka() const {return m_ka;}

    void setKd(const Vector3 & kd) {m_kd = kd;}
    void setKa(const Vector3 & ka) {m_ka = ka;}

    virtual void preCalc() {}
    
    const Vector3 shade(const unsigned int threadID, const Ray& ray, const HitInfo &hit, const Scene& scene, bool isSecondary = false) const;
protected:
    Vector3 m_kd;
    Vector3 m_ka;
};

#endif // CSE168_LAMBERT_H_INCLUDED
