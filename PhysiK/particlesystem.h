#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include <vector>
#include "trianglehashtable.h"
#include "solver.h"

#include "particle.h"

#define DEFAULT_GRAVITY 9.81f
#define DEFAULT_DAMPING 0.9f
#define DEFAULT_NB_ITERATIONS 10

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

			TriangleHashTable THT;
			ParticleHashTable PHT;

			std::vector<IntersectionParticleParticle> ptpIntersections;

			Solver solver;

			float gravity;
			float damping;
			int nbIterations;

		public:

			ParticleSystem() :
				gravity(DEFAULT_GRAVITY),
				damping(DEFAULT_DAMPING),
				nbIterations(DEFAULT_NB_ITERATIONS)
			{}

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
			 * @brief add a particle group
			 */
			void addParticleGroup(ParticleGroup * particle);

			/**
			 * @brief generate intersections for the next iteration of the loop
			 */
			void genIntersectionConstraints();

			/**
			 * @brief velocityUpdate generates impultions for each collision
			 */
			void velocityUpdate();

			/**
			 * @brief execute the next iteration of the loop
			 * @param deltaT the time enlapsed since the last iteration
			 */
			void nextSimulationStep(float deltaT);

	};

	}

#endif // PARTICLESYSTEM_H
