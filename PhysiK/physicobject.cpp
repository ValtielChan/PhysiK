#include "physicobject.h"

PhysiK::PhysicObject::PhysicObject(int nbPosition){
	positions = new Particle[nbPosition]();
	newPositions = new vec3[nbPosition]();
	velocities = new vec3[nbPosition]();
}

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

PhysiK::vec3 *PhysiK::PhysicObject::getVelocities()
{
    return velocities;
}

void PhysiK::PhysicObject::preUpdate(float dt, float gravity, float damping)
{
    for(int i=0; i<nbParticles; ++i)
    {
        velocities[i].y += gravity * dt;
        velocities[i] *= damping;
        newPositions[i] = positions[i].pos + velocities[i]*dt;
    }
}

void PhysiK::PhysicObject::postUpdate(float dt)
{
    for(int i=0; i<nbParticles; ++i)
    {
        velocities[i] = (newPositions[i] - positions[i].pos)/dt;
        positions[i].pos = newPositions[i];
    }
}

PhysiK::PhysicObject::~PhysicObject(){
	delete positions;
	delete newPositions;
	delete velocities;
}
