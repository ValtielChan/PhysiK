#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include <vector>
#include "trianglehashtable.h"
#include "solver.h"

#include "body.h"
#include "particle.h"
#include "constraint.h"
#include "triangle.h"

namespace PhysiK {

	class Body;
	class ParticleGroup;
	class PhysicObject;

	/**
	 * @brief This class allow to add soft and rigid bodies, particles, and compute
	 * the main loop
	 */
	class ParticleSystem
	{
		private:

            std::vector<PhysicObject *> physicObjecs;

			TriangleHashTable TNT;

			Solver solver;

		public:

			/**
			 * @brief add a body and generate the constraint to make a rigid body
			 * @param body the body to add
			 */
			void addRigidBody(Body * body);

			/**
			 * @brief add a body and generate the constraint to make a soft body
			 */
			void addSoftBody(Body * body);

			/**
			 * @brief add a particle system
			 */
			void addParticleGroup(ParticleGroup * particle);

			/**
			 * @brief generate intersections for the next iteration of the loop
			 */
			void genIntersectionConstraint();

			/**
			 * @brief execute the next iteration of the loop
			 * @param deltaT the time enlapsed since the last iteration
			 */
			void nextSimulationStep(float deltaT);

	};

	}

#endif // PARTICLESYSTEM_H
