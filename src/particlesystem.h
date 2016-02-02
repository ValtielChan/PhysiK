#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include <vector>
#include "trianglehashtable.h"
#include "solver.h"

namespace PhysiK {

	class Body;
	class ParticleGroup;
	class PhysicObject;

	class ParticleSystem
	{
		private:

			std::vector<PhysicObject *> PhysicObjecs;

			TriangleHashTable TNT;//kaboum

			Solver fortyTwo;//the solution

		public:

			void addRigidBody(Body * body);

			void addSoftBody(Body * body);

			void addParticleGroup(ParticleGroup * particle);

			void genIntersectionConstraint();

			void nextSimulationStep(float deltaT);

	};

}

#endif // PARTICLESYSTEM_H
