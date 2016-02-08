#include "particlegroup.h"
#include "particle.h"

PhysiK::ParticleGroup::ParticleGroup(int particleAmount, char* dataPtr, int dataStride,
									 float particleRadius, float particleMass) :
	PhysicObject(),
	mass(particleMass),
	radius(particleRadius)
{
	nbParticles = particleAmount;
	float omega = 1/mass;
	PhysiK::PhysicObject::positions = new Particle[nbParticles];
	PhysiK::PhysicObject::oldPositions = new vec3[nbParticles];
	PhysiK::PhysicObject::velocities = new vec3[nbParticles];
	for(int i=0; i<nbParticles; ++i)
	{
		float* ptr = (float*)dataPtr;
		PhysiK::vec3 pos(ptr[0], ptr[1], ptr[2]);
		positions[i] = PhysiK::Particle(pos, omega);
		oldPositions[i] = pos;
		velocities[i] = PhysiK::vec3(0, 0, 0);
		dataPtr += dataStride;
	}
}

PhysiK::ParticleGroup::~ParticleGroup()
{
	delete positions;
	delete oldPositions;
	delete velocities;
}
