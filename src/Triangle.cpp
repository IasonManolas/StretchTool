#include "Triangle.h"
#include "tiny_obj_loader.h"
#define EPSILON 0.000001 //needed for triangle_intersection function
Triangle::Triangle(void)
{
}

Triangle::Triangle(Vector3D v1,Vector3D v2,Vector3D v3)
{
	v1=v1;
	v2=v2;
	v3=v3;
	n=((v2-v1)%(v3-v1)).Normalize();
}
Triangle::Triangle(tinyobj::mesh_t mesh,int indexInIndices)
{
	int i=indexInIndices;
	Vector3D firstVertex,secondVertex,thirdVertex; //of triangle
	firstVertex.Set(mesh.positions[3*mesh.indices[i]],mesh.positions[3*mesh.indices[i]+1],mesh.positions[3*mesh.indices[i]+2]);
	secondVertex.Set(mesh.positions[3*mesh.indices[i+1]],mesh.positions[3*mesh.indices[i+1]+1],mesh.positions[3*mesh.indices[i+1]+2]);
	thirdVertex.Set(mesh.positions[3*mesh.indices[i+2]],mesh.positions[3*mesh.indices[i+2]+1],mesh.positions[3*mesh.indices[i+2]+2]);
	v1=firstVertex;
	v2=secondVertex;
	v3=thirdVertex;
	n=((v2-v1)%(v3-v1)).Normalize();
}
Triangle& Triangle::Set(Vector3D firstVert,Vector3D secondVert,Vector3D thirdVert)
{
            v1 = firstVert;
            v2 = secondVert;
            v3 = thirdVert;
			n=n=((v2-v1)%(v3-v1)).Normalize();
            return (*this);  
}

bool Triangle::triangle_intersection( const Vector3D   V1,  // Triangle vertices
                           const Vector3D   V2,
                           const Vector3D   V3,
                           const Vector3D    O,  //Ray origin
                           const Vector3D    D,  //Ray direction
                                 float &out )
{

  
Vector3D e1, e2;  //Edge1, Edge2
  Vector3D P, Q, T;
  float det, inv_det, u, v;
  float t;

  //Find vectors for two edges sharing V1
  e1= V2-V1;
  e2=V3-V1;
  //Begin calculating determinant - also used to calculate u parameter
  P=D%e2;
  //if determinant is near zero, ray lies in plane of triangle
  det = e1*P;
  //NOT CULLING
  if(det > -EPSILON && det < EPSILON) return false; //abs(det)<EPSILON
  inv_det = 1.f / det;

  //calculate distance from V1 to ray origin
  T= O- V1;

  //Calculate u parameter and test bound
  u = (T* P) * inv_det;
  //The intersection lies outside of the triangle
  if(u < 0.f || u > 1.f) return false;

  //Prepare to test v parameter
  Q= T% e1;

  //Calculate V parameter and test bound
  v = (D* Q) * inv_det;
  //The intersection lies outside of the triangle
  if(v < 0.f || u + v  > 1.f) return false;

  t = (e2*Q) * inv_det;

  if(t > EPSILON) { //ray intersection
    out = t;
    return true;
  }

  // No hit, no win
  return false;
}
Triangle::~Triangle(void)
{
}
