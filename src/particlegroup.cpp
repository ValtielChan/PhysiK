#include "particlegroup.h"

PhysiK::ParticleGroup::ParticleGroup(int particleAmount, char* dataPtr, int dataStride,
              float particleRadius, float particleMass) :
    PhysicObject(),
    nbParticles(particleAmount),
    mass(particleMass),
    radius(particleRadius)
{
    float omega = 1/mass;
    PhysiK::PhysicObject::positions = new vec3[nbParticles];
    PhysiK::PhysicObject::oldPositions = new vec3[nbParticles];
    PhysiK::PhysicObject::velocities = new vec3[nbParticles];
    for(in i=0; i<nbParticles; ++i)
    {
        float* ptr = (float*)dataPtr;
        positions[i] = PhysiK::vec3(ptr[0], ptr[1], ptr[2], omega);
        oldPositions[i] = positions[i];
        velocities[i] = PhysiK::vec3(0, 0, 0, 0);
        dataPtr += dataStride;
    }
}

PhysiK::ParticleGroup::~ParticleGroup()
{
    delete positions;
    delete oldPositions;
    delete velocities;
}
