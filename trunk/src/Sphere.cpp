#include "Sphere.h"
#include "Ray.h"
#include "Console.h"

Sphere::Sphere()
{
	bBox = new AABB(Vector3(0), Vector3(0));
}

Sphere::~Sphere()
{
}

void
Sphere::renderGL()
{
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(m_center.x, m_center.y, m_center.z);
    glutWireSphere(m_radius, 20, 20);
    glPopMatrix();
}

bool
Sphere::intersect(HitInfo& result, const Ray& ray,
                  float tMin, float tMax)
{
    static Vector3 rayO = Vector3(ray.ox, ray.oy, ray.oz);
	const Vector3 toO = rayO - m_center; 

    static Vector3 rayD = Vector3(ray.dx, ray.dy, ray.dz);
	const float a = rayD.length2(); 
    const float b = dot(2*rayD, toO);
    const float c = toO.length2() - m_radius*m_radius;

    const float discrim = b*b-4.0f*a*c; 

    if (discrim < 0) 
        return false;   // quadratic equation would yield imaginary numbers

    const float sqrt_discrim = sqrt(discrim); 

    // solve the quadratic equation
    const float t[2] = {(-b-sqrt_discrim)/(2.0f*a), (-b+sqrt_discrim)/(2.0f*a)}; 

    // since we know that discrim >= 0, t[0] < t{1]
    // return the t closest to us that is within range
    if ((t[0] > tMin) && (t[0] < tMax))
    {
        result.t = t[0];
    }
    else if((t[1] > tMin) && (t[1] < tMax))
    {
        result.t = t[1];
    }
    else
    {
        // neither of the solutions are in the required range
        return false; 
    }
 
    Vector3 P = rayO + result.t*rayD;
	result.px = P.x; result.py = P.y; result.pz = P.z;
	result.N = (P-m_center); 
    result.N.normalize(); 
    result.material = this->m_material; 

    return true;
}