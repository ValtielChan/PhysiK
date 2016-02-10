#include "physicobject.h"

void PhysiK::PhysicObject::computeBarycenter()
{

    float xSum = 0.f, ySum = 0.f, zSum = 0.f;
    for(int i = 0; i < nbParticles; ++i){

        xSum += positions[i].pos.x;
        ySum += positions[i].pos.y;
        zSum += positions[i].pos.z;
    }

    xSum /= nbParticles;
    ySum /= nbParticles;
    zSum /= nbParticles;

	barycenter = vec3(xSum, ySum, zSum);
}

PhysiK::Particle *PhysiK::PhysicObject::getPositions()
{
	return positions;
}
