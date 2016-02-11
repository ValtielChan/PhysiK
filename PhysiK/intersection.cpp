#include "intersection.h"

#include "body.h"
#include "particlegroup.h"
#include "triangle.h"
#include "constraint.h"

#include <glm/common.hpp>

bool PhysiK::IntersectionParticuleTriangle::intersect(vec3 oldPostion) const{

	//http://heigeas.free.fr/laure/ray_tracing/triangle.html

	vec3 p = particule->pos;
	vec3 d = oldPostion-p;

	vec3 A[3];
	for(int i = 0 ; i < 3 ; i++)
		A[i] = colider->getPositions()[colider->getTriangles()[triangle][i]].pos;

	vec3 u = A[0] - A[1];
	vec3 v = A[0] - A[2];

	vec3 w = p - A[0];

	float D = -u.cross(v).dot(d);

	float a = -w.cross(v).dot(d)/D;
	float b = -u.cross(w).dot(d)/D;
	float t =  u.cross(v).dot(w)/D;

	return a >= 0 && b >= 0 && a + b <= 1 && t < 1 && t>= 0;

}

PhysiK::Constraint * PhysiK::IntersectionParticuleTriangle::getConstraint() const{
	vec3 A[3];
	for(int i = 0 ; i < 3 ; i++)
		A[i] = colider->getPositions()[colider->getTriangles()[triangle][i]].pos;

	return new CollisionConstraint(particule,A[0],A[1],A[2]);
}

bool PhysiK::IntersectionParticuleParticule::intersect() const{
    return (particle1->pos-particle2->pos).length()<radius;
}

PhysiK::Constraint * PhysiK::IntersectionParticuleParticule::getConstraint() const{
    return new DistanceConstraint(particle1, particle2, radius);
}
