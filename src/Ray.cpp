#include "Ray.h"
#include "ProxyObject.h"
#include "TriangleMesh.h"

const void HitInfo::getAllInfos(Vector3 &N, Vector3 &geoN, float &uCoord, float &vCoord) const
{
	if (!obj)												// Do nothing if there's no object
	{
		return;
	}

	TriangleMesh::TupleI3 ti3;								// Get pointer and index for the mesh
	TriangleMesh* theMesh = obj->m_mesh;
	u_int meshIndex       = obj->m_index;

	ti3           = theMesh->m_vertexIndices[meshIndex];	// Get the geometric normal
	Vector3 edge0 = theMesh->m_vertices[ti3.y] - theMesh->m_vertices[ti3.x];
	Vector3 edge1 = theMesh->m_vertices[ti3.z] - theMesh->m_vertices[ti3.x];
	geoN		  = cross(edge0, edge1).normalized();

	ti3       = theMesh->m_normalIndices[meshIndex];		// Get the interpolated normal
	float c   = 1.0f-a-b;
	N		  = Vector3((theMesh->m_normals[ti3.x]*c+theMesh->m_normals[ti3.y]*a+theMesh->m_normals[ti3.z]*b).normalized());

	if (m_proxy)
	{
		geoN = (m_proxy->getMatrix().m_transpose * geoN).normalized();
		N = (m_proxy->getMatrix().m_transpose * N).normalized();
	}

	if (theMesh->m_texCoordIndices)							// If possible, get the interpolated u, v coordinates
	{
		ti3    = theMesh->m_texCoordIndices[meshIndex];
		uCoord = theMesh->m_texCoords[ti3.x].x*c+theMesh->m_texCoords[ti3.y].x*a+theMesh->m_texCoords[ti3.z].x*b;
		vCoord = theMesh->m_texCoords[ti3.x].y*c+theMesh->m_texCoords[ti3.y].y*a+theMesh->m_texCoords[ti3.z].y*b;
	}
	else													// We always return texture coordinates
	{
		uCoord = a;
		vCoord = b;
	}
}

const void HitInfo::getInterpolatedNormal(Vector3& N) const
{
	if (!obj)												// Do nothing if there's no object
	{
		return;
	}

	TriangleMesh::TupleI3 ti3;								// Get pointer and index for the mesh
	TriangleMesh* theMesh = obj->m_mesh;
	u_int meshIndex       = obj->m_index;

	ti3       = theMesh->m_normalIndices[meshIndex];		// Get the interpolated normal
	float c   = 1.0f-a-b;
	N		  = Vector3((theMesh->m_normals[ti3.x]*c+theMesh->m_normals[ti3.y]*a+theMesh->m_normals[ti3.z]*b).normalized());
}

const void HitInfo::getGeoNormal(Vector3& geoN) const
{
	if (!obj)												// Do nothing if there's no object
	{
		return;
	}

	TriangleMesh::TupleI3 ti3;								// Get pointer and index for the mesh
	TriangleMesh* theMesh = obj->m_mesh;
	u_int meshIndex       = obj->m_index;

	ti3           = theMesh->m_vertexIndices[meshIndex];	// Get the geometric normal
	Vector3 edge0 = theMesh->m_vertices[ti3.y] - theMesh->m_vertices[ti3.x];
	Vector3 edge1 = theMesh->m_vertices[ti3.z] - theMesh->m_vertices[ti3.x];
	geoN		  = cross(edge0, edge1).normalized();
}

const void HitInfo::getUVs(float& uCoord, float &vCoord) const
{
	if (!obj)												// Do nothing if there's no object
	{
		return;
	}

	TriangleMesh::TupleI3 ti3;								// Get pointer and index for the mesh
	TriangleMesh* theMesh = obj->m_mesh;
	u_int meshIndex       = obj->m_index;
	float c   = 1.0f-a-b;

	if (theMesh->m_texCoordIndices)							// If possible, get the interpolated u, v coordinates
	{
		ti3    = theMesh->m_texCoordIndices[meshIndex];
		uCoord = theMesh->m_texCoords[ti3.x].x*c+theMesh->m_texCoords[ti3.y].x*a+theMesh->m_texCoords[ti3.z].x*b;
		vCoord = theMesh->m_texCoords[ti3.x].y*c+theMesh->m_texCoords[ti3.y].y*a+theMesh->m_texCoords[ti3.z].y*b;
	}
	else													// We always return texture coordinates
	{
		uCoord = a;
		vCoord = b;
	}
}