#include "ProxyObject.h"
#include "Ray.h"
#include "TriangleMesh.h"

ProxyObject::ProxyObject(Objects* m, BVH* b, Matrix4x4& t)
{
	m_objects = m;
	m_Matrix = (ProxyMatrix*)_aligned_malloc(sizeof(ProxyMatrix), 16);
	*m_Matrix = ProxyMatrix(t);
	m_BVH = b;
	m_objectType = PROXY_OBJECT;
}

// Get the AABB for a Proxy object by transforming the objects AABB by it's matrix
// We need to generate a new AABB since the one from the object might not be axis-aligned any more
void ProxyObject::getAABB(AABB* outBox)
{
	AABB tmpBox = m_BVH->getAABB();
	Vector3 A	 = Vector3(tmpBox.bbMin.x, tmpBox.bbMin.y, tmpBox.bbMax.z);
	Vector3 B	 = Vector3(tmpBox.bbMin.x, tmpBox.bbMax.y, tmpBox.bbMin.z);
	Vector3 C	 = Vector3(tmpBox.bbMax.x, tmpBox.bbMin.y, tmpBox.bbMin.z);
	Vector3 D	 = Vector3(tmpBox.bbMin.x, tmpBox.bbMax.y, tmpBox.bbMax.z);
	Vector3 E    = Vector3(tmpBox.bbMax.x, tmpBox.bbMax.y, tmpBox.bbMin.z);
	Vector3 F    = Vector3(tmpBox.bbMax.x, tmpBox.bbMin.y, tmpBox.bbMax.z);
	A            = m_Matrix->m_transform.multiplyAndDivideByW(A);
	B            = m_Matrix->m_transform.multiplyAndDivideByW(B);
	C            = m_Matrix->m_transform.multiplyAndDivideByW(C);
	D            = m_Matrix->m_transform.multiplyAndDivideByW(D);
	E            = m_Matrix->m_transform.multiplyAndDivideByW(E);
	F            = m_Matrix->m_transform.multiplyAndDivideByW(F);
	tmpBox.bbMin = m_Matrix->m_transform.multiplyAndDivideByW(tmpBox.bbMin);
	tmpBox.bbMax = m_Matrix->m_transform.multiplyAndDivideByW(tmpBox.bbMax);
	AABB newBox = AABB();
	newBox.grow(A);
	newBox.grow(B);
	newBox.grow(C);
	newBox.grow(D);
	newBox.grow(E);
	newBox.grow(F);
	newBox.grow(tmpBox.bbMin);
	newBox.grow(tmpBox.bbMax);
	*outBox = newBox;
}

AABB ProxyObject::getAABB()
{
	AABB tmpBox = m_BVH->getAABB();
	Vector3 A	 = Vector3(tmpBox.bbMin.x, tmpBox.bbMin.y, tmpBox.bbMax.z);
	Vector3 B	 = Vector3(tmpBox.bbMin.x, tmpBox.bbMax.y, tmpBox.bbMin.z);
	Vector3 C	 = Vector3(tmpBox.bbMax.x, tmpBox.bbMin.y, tmpBox.bbMin.z);
	Vector3 D	 = Vector3(tmpBox.bbMin.x, tmpBox.bbMax.y, tmpBox.bbMax.z);
	Vector3 E    = Vector3(tmpBox.bbMax.x, tmpBox.bbMax.y, tmpBox.bbMin.z);
	Vector3 F    = Vector3(tmpBox.bbMax.x, tmpBox.bbMin.y, tmpBox.bbMax.z);
	A            = m_Matrix->m_transform.multiplyAndDivideByW(A);
	B            = m_Matrix->m_transform.multiplyAndDivideByW(B);
	C            = m_Matrix->m_transform.multiplyAndDivideByW(C);
	D            = m_Matrix->m_transform.multiplyAndDivideByW(D);
	E            = m_Matrix->m_transform.multiplyAndDivideByW(E);
	F            = m_Matrix->m_transform.multiplyAndDivideByW(F);
	tmpBox.bbMin = m_Matrix->m_transform.multiplyAndDivideByW(tmpBox.bbMin);
	tmpBox.bbMax = m_Matrix->m_transform.multiplyAndDivideByW(tmpBox.bbMax);
	AABB newBox = AABB();
	newBox.grow(A);
	newBox.grow(B);
	newBox.grow(C);
	newBox.grow(D);
	newBox.grow(E);
	newBox.grow(F);
	newBox.grow(tmpBox.bbMin);
	newBox.grow(tmpBox.bbMax);
	return newBox;
}

// Intersect a Proxy object. We generate a new ray that is a copy of the original ray but transformed
// into object space. Then we traverse the proxy's BVH.
const bool ProxyObject::intersect(const unsigned int threadID, HitInfo &result, const Ray& ray, const float tMin)
{
	Vector3 newRayO = m_Matrix->m_inverse.multiplyAndDivideByW(ray._o);
	Vector3 newRayD = m_Matrix->m_inverse * ray._d;
	HitInfo newHit;
	newHit.t = result.t;
	__declspec(align(128)) Ray newRay = Ray(threadID, newRayO, newRayD, ray.time, ray.r_IOR, 0, 0, ray.bounces_flags);

	bool hit = m_BVH->intersect(threadID, newHit, newRay, tMin);

	if (hit)
	{
		result.a = newHit.a;
		result.b = newHit.b;
		result.t = newHit.t;
		result.obj = newHit.obj;
		result.m_proxy = this;
	}
	return hit;
}

// Render the proxy object to the screen. We only display every nth polygon to maximize performance (n = m_displayNum)
void ProxyObject::renderGL()
{
	for (int i = 0; i < m_objects->size(); i+=m_displayNum)
	{
		Vector3 v0, v1, v2;
		TriangleMesh::TupleI3 ti3 = (*m_objects)[i]->m_mesh->m_vertexIndices[(*m_objects)[i]->m_index];// [m_index];
		v0 = m_Matrix->m_transform.multiplyAndDivideByW( (*m_objects)[i]->m_mesh->m_vertices[ti3.x] ); //vertex a of triangle
		v1 = m_Matrix->m_transform.multiplyAndDivideByW( (*m_objects)[i]->m_mesh->m_vertices[ti3.y] ); //vertex b of triangle
		v2 = m_Matrix->m_transform.multiplyAndDivideByW( (*m_objects)[i]->m_mesh->m_vertices[ti3.z] ); //vertex c of triangle

		glBegin(GL_TRIANGLES);
		glVertex3f(v0.x, v0.y, v0.z);
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glEnd();
	}	
}

// Precalculate geometry for the Proxy. It merely calls the preCalc routine for the geometry.
void ProxyObject::preCalc()
{
	if ((*m_objects)[0]->m_mesh->doPreCalc)
	{
		Objects::const_iterator objectIter;
		for (objectIter = m_objects->begin(); objectIter != m_objects->end(); objectIter++)
		{
			(*objectIter)->preCalc();
		}
	}
}

// Setup a proxy object by incorporating the geometry into an Objects container and
// building the BVH for the object.
void ProxyObject::setupProxy(TriangleMesh* mesh, Material* mat, Objects* m, BVH* b)
{
	int numObjs = mesh->m_numTris;
	Object* t = new Object[numObjs];

	int i = numObjs-1;
	while (i >= 0)
	{
		t[i].setMesh(mesh);
		t[i].setIndex(i);
		t[i].setMaterial(mat);
		m->push_back(&t[i]);
		i--;
	}
	b->build(m);
}

// Setup a proxy object consisting of multiple TriangleMesh objects.
void ProxyObject::setupMultiProxy(TriangleMesh* mesh[], int numObjs, Material* mat[], Objects* m, BVH* b)
{
	for (int j = 0; j < numObjs; j++)
	{
		int localNum = mesh[j]->m_numTris;
		Object* t = new Object[localNum];

		int i = localNum-1;
		while (i >= 0)
		{
			t[i].setMesh(mesh[j]);
			t[i].setIndex(i);
			t[i].setMaterial(mat[j]);
			m->push_back(&t[i]);
			i--;
		}
	}
	b->build(m);
}