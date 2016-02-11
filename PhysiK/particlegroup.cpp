#include "particlegroup.h"
#include "particle.h"

PhysiK::ParticleGroup::ParticleGroup(int particleAmount, float* dataPtr, int dataStride,
									 float particleRadius, float particleMass) :
	PhysicObject(particleAmount),
	mass(particleMass),
	radius(particleRadius)
{
	nbParticles = particleAmount;
	float omega = 1/mass;
	for(int i=0; i<nbParticles; ++i)
	{
		PhysiK::vec3 pos(dataPtr[0], dataPtr[1], dataPtr[2]);
		positions[i] = PhysiK::Particle(pos, omega);
		velocities[i] = PhysiK::vec3(0, 0, 0);
		dataPtr = (float*)((char*)dataPtr + dataStride);
	}
}
