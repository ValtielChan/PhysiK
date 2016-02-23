#include "particlegroup.h"
#include "particle.h"

PhysiK::ParticleGroup::ParticleGroup(int particleAmount, float* dataPtr, int dataStride,
									 float particleRadius, float particleMass, bool isKinematic) :
	PhysicObject(particleAmount,isKinematic),
	mass(particleMass),
	radius(particleRadius)
{
	nbParticles = particleAmount;
	float omega = isKinematic || mass==0 ? 0 : 1/mass;
	for(unsigned int i=0; i<nbParticles; ++i)
	{
		PhysiK::vec3 pos(dataPtr[0], dataPtr[1], dataPtr[2]);
		particles[i] = PhysiK::Particle(pos, omega);
		oldPositions[i] = particles[i].pos;
		dataPtr = (float*)((char*)dataPtr + dataStride);
	}
}
