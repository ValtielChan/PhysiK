#include "particlegroup.h"
#include "particle.h"

PhysiK::ParticleGroup::ParticleGroup(int particleAmount, float* dataPtr, int dataStride,
                                     float particleRadius, float particleMass, bool isKinematic) :
	PhysicObject(particleAmount),
	mass(particleMass),
    radius(particleRadius)
{
    this->isKinematic = isKinematic;

	nbParticles = particleAmount;
	float omega = 1/mass;
	for(unsigned int i=0; i<nbParticles; ++i)
	{
		PhysiK::vec3 pos(dataPtr[0], dataPtr[1], dataPtr[2]);
        particles[i] = PhysiK::Particle(pos, omega, isKinematic);
        oldPositions[i] = particles[i].pos;
		dataPtr = (float*)((char*)dataPtr + dataStride);
	}
}
