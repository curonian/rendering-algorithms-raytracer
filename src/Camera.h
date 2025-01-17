#ifndef CSE168_CAMERA_H_INCLUDED
#define CSE168_CAMERA_H_INCLUDED

#include "Scene.h"
#include "Vector3.h"
#include "Miro.h"
#include "Ray.h"

class Camera
{
public:
    Camera();
    virtual ~Camera();

    enum
    {
        RENDER_OPENGL   = 0,
        RENDER_RAYTRACE = 1
    };

    void click(Scene* pScene, Image* pImage);

    inline bool isOpenGL() const {return m_renderer == RENDER_OPENGL;}
    inline void setRenderer(int i) {m_renderer = i;}

    inline void setEye(float x, float y, float z);
    inline void setEye(const Vector3& eye);
    inline void setUp(float x, float y, float z);
    inline void setUp(const Vector3& up);
    inline void setViewDir(float x, float y, float z);
    inline void setViewDir(const Vector3& vd);
    inline void setLookAt(float x, float y, float z);
    inline void setLookAt(const Vector3& look);

    inline void setFOV(float fov) {m_fov = fov;}
	inline float FOV() {return m_fov;}

	inline void setFocusPlane(float f) {m_focusPlane = f;}
	inline float focusPlane() {return m_focusPlane;}

	inline void setAperture(float f) {m_aperture = f;}
	inline float aperture() {return m_aperture;}

	inline void setShutterSpeed(float f) {m_shutterSpeed = f;}
	inline float shutterSpeed()			 {return m_shutterSpeed;}
	const inline float getTimeSample() const		 {float r = Scene::getRand(); return 1.f-r*r*r*m_shutterSpeed;} // Return a time sample that is weighted towards the end of the interval

    inline float fov() const                {return m_fov;}
    inline const Vector3 & viewDir() const  {return m_viewDir;}
    inline const Vector3 & lookAt() const   {return m_lookAt;}
    inline const Vector3 & up() const       {return m_up;}
    inline const Vector3 & eye() const      {return m_eye;}

    const Ray eyeRay(const unsigned int threadID, const int x, const int y, const int imageWidth, const int imageHeight) const;
	const Ray eyeRayAdaptive(const unsigned int threadID, const int x, const int y,
		                     const float minXOffset, const float maxXOffset, const float minYOffset, const float maxYOffset,
							 const int imageWidth, const int imageHeight) const;
    
    void drawGL();

	Vector3 m_eye;
	Vector3 m_up;
	Vector3 m_viewDir;
	Vector3 m_lookAt;
	float m_fov;
	float m_focusPlane;
	float m_aperture;
	float m_shutterSpeed;

private:
    void calcLookAt();

    int m_renderer;

    // main screen params
};

extern Camera * g_camera;

//--------------------------------------------------------

inline void Camera::setEye(float x, float y, float z)
{
    m_eye.set(x, y, z);
}

inline void Camera::setEye(const Vector3& eye)
{
    setEye(eye.x, eye.y, eye.z);
}

inline void Camera::setUp(float x, float y, float z)
{
    m_up.set(x, y, z);
    m_up.normalize();
}

inline void Camera::setUp(const Vector3& up)
{
    setUp(up.x, up.y, up.z);
}

inline void Camera::setViewDir(float x, float y, float z)
{
    m_viewDir.set(x, y, z);
    m_viewDir.normalize();
}

inline void Camera::setViewDir(const Vector3& vd)
{
    setViewDir(vd.x, vd.y, vd.z);
}

inline void Camera::setLookAt(float x, float y, float z)
{
    Vector3 dir = Vector3(x, y, z) - m_eye;
    setViewDir(dir);
}

inline void Camera::setLookAt(const Vector3& vd)
{
    m_lookAt = vd;
	setLookAt(vd.x, vd.y, vd.z);
}

#endif // CSE168_CAMERA_H_INCLUDED
