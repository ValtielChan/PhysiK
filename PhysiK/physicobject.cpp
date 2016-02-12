#include "physicobject.h"

PhysiK::PhysicObject::PhysicObject(int nbPosition){
    particles = new Particle[nbPosition]();
	oldPositions = new vec3[nbPosition]();
}

void PhysiK::PhysicObject::computeBarycenter()
{

    float xSum = 0.f, ySum = 0.f, zSum = 0.f;
    for(unsigned int i = 0; i < nbParticles; ++i){

        xSum += particles[i].pos.x;
        ySum += particles[i].pos.y;
        zSum += particles[i].pos.z;
    }

    xSum /= nbParticles;
    ySum /= nbParticles;
    zSum /= nbParticles;

	barycenter = vec3(xSum, ySum, zSum);
}

PhysiK::Particle *PhysiK::PhysicObject::getPositions()
{
    return particles;
}

void PhysiK::PhysicObject::preUpdate(float dt, float gravity, float damping)
{
    for(unsigned int i=0; i<nbParticles; ++i)
    {
        particles[i].velocity.y += gravity * dt;
        particles[i].velocity *= damping;
        particles[i] = particles[i].pos + particles[i].velocity*dt;
    }
}

void PhysiK::PhysicObject::postUpdate(float dt)
{
    for(unsigned int i=0; i<nbParticles; ++i)
    {
        particles[i].velocity = (particles[i].pos - oldPositions[i])/dt;
        oldPositions[i]=particles[i].pos;
    }
}

PhysiK::PhysicObject::~PhysicObject(){
    delete particles;
	delete oldPositions;
}
