#include "intersection.h"

#include "body.h"
#include "particlegroup.h"
#include "triangle.h"
#include "constraint.h"

bool PhysiK::IntersectionParticleTriangle::intersect(vec3 oldPostion) const{

    // http://heigeas.free.fr/laure/ray_tracing/triangle.html

	vec3 p = particle->pos;
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

bool PhysiK::IntersectionParticleTriangle::intersect() const{
	Particle * A[3];
	for(int i = 0 ; i < 3 ; i++)
		A[i] = colider->getPositions()+colider->getTriangles()[triangle][i];

	return CollisionParticuleTriangleConstraint(particle,A[0],A[1],A[1]).eval()!=0;
}

PhysiK::Constraint * PhysiK::IntersectionParticleTriangle::getConstraint() const{
	Particle * A[3];
	for(int i = 0 ; i < 3 ; i++)
		A[i] = colider->getPositions()+colider->getTriangles()[triangle][i];

	return new CollisionParticuleTriangleConstraint(particle,A[0],A[1],A[2]);
}

bool PhysiK::IntersectionParticleParticle::intersect() const{
	return (getParticle1()->pos-getParticle2()->pos).length()<(particles1->radius+particles2->radius);
}

PhysiK::vec3 PhysiK::IntersectionParticleParticle::intersectPos() const{
	return (getParticle1()->pos*particles2->radius+getParticle2()->pos*particles1->radius)/(particles1->radius+particles2->radius);
}

PhysiK::Constraint * PhysiK::IntersectionParticleParticle::getConstraint() const{
	return new MinDistanceConstraint(getParticle1(), getParticle2(), particles1->radius+particles2->radius);
}

PhysiK::Particle * PhysiK::IntersectionParticleParticle::getParticle1() const{
	return particles1->getPositions()+offset1;
}

PhysiK::Particle * PhysiK::IntersectionParticleParticle::getParticle2() const{
	return particles2->getPositions()+offset2;
}

PhysiK::vec3 PhysiK::IntersectionParticleParticle::getWorks1(float deltaT)const{
	return particles1->getDeltaP(offset1)*deltaT*particles1->mass;
}

PhysiK::vec3 PhysiK::IntersectionParticleParticle::getWorks2(float deltaT)const{
	return particles2->getDeltaP(offset2)*deltaT*particles2->mass;
}
