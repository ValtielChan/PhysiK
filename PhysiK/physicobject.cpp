#include "physicobject.h"

PhysiK::PhysicObject::PhysicObject(int nbPosition){
    particles = new Particle[nbPosition]();
	oldPositions = new vec3[nbPosition]();
}

PhysiK::Particle *PhysiK::PhysicObject::getPositions()const
{
	return particles;
}


PhysiK::vec3 *PhysiK::PhysicObject::getOldPositions()const
{
    return oldPositions;
}

PhysiK::vec3 PhysiK::PhysicObject::getDeltaP(unsigned int offset)const
{
    return getPositions()[offset].pos - oldPositions[offset];
}

void PhysiK::PhysicObject::preUpdate(float dt, float gravity, float damping)
{
    for(unsigned int i=0; i<nbParticles; ++i)
    {
        particles[i].velocity.y += particles[i].omega == 0 ? 0 : gravity * dt;
        particles[i].velocity *= damping;
        particles[i] = particles[i].pos + particles[i].velocity*dt;
    }
}

void PhysiK::PhysicObject::postUpdate(float dt)
{
    for(unsigned int i=0; i<nbParticles; ++i)
    {
        particles[i].velocity = (particles[i].pos - oldPositions[i])/dt;
        particles[i].velocity+=particles[i].impulsion*particles[i].omega;
        particles[i].impulsion+=vec3();
        oldPositions[i]=particles[i].pos;
    }
}

PhysiK::PhysicObject::~PhysicObject(){
    delete particles;
	delete oldPositions;
}
