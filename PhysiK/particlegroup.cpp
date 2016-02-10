#include "particlegroup.h"
#include "particle.h"

PhysiK::ParticleGroup::ParticleGroup(int particleAmount, char* dataPtr, int dataStride,
									 float particleRadius, float particleMass) :
	PhysicObject(particleAmount),
	mass(particleMass),
	radius(particleRadius)
{
	nbParticles = particleAmount;
	float omega = 1/mass;
	for(int i=0; i<nbParticles; ++i)
	{
		float* ptr = (float*)dataPtr;
		PhysiK::vec3 pos(ptr[0], ptr[1], ptr[2]);
		positions[i] = PhysiK::Particle(pos, omega);
		velocities[i] = PhysiK::vec3(0, 0, 0);
		dataPtr += dataStride;
	}
}
