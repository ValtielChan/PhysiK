#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include <vector>
#include <iostream>
#include "trianglehashtable.h"
#include "solver.h"

#include "particle.h"
#include "intersection.h"

#define DEFAULT_GRAVITY -9.81f // 1 unité = 1 m
#define DEFAULT_DAMPING 0.99f
#define DEFAULT_NB_ITERATIONS 1

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
			std::vector<IntersectionParticleTriangle> pttIntersections;

			Solver solver;

			float gravity;
			float damping;
			unsigned int nbIterations;

			/**
			 * @brief generate intersections for the next iteration of the loop
			 */
			void genIntersectionConstraints();

			/**
			 * @brief velocityUpdate generates impultions for each collision
			 */
			void velocityUpdate(float deltaT);
			/**
			 * @brief add a Body as a particle system for the physic
			 */
			void toParticles(Body * body);

			/**
			 * @brief add a minimum distance contraint to all edge of the body mesh.
			 * is used by the addSoftBody and addCloth methods
			 */
			void addClothConstraint(Body *body);

		public:
			ParticleSystem();
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
			 * @brief add a body and generate the constraint to make it look like a cloth
			 */
			void addCloth(Body * body);
			/**
			 * @brief add an invisble scene to the scene for tests (deprecated)
			 */
			void addCube();
			/**
			 * @brief add a particle group
			 */
			void addParticleGroup(ParticleGroup * particleGroup);
			/**
			 * @brief execute the next iteration of the loop
			 * @param deltaT the time enlapsed since the last iteration
			 */
			void nextSimulationStep(float deltaT);
			/**
			 * @brief set the nomber of iteration the solver should perform
			 */
			void setNbIterations(unsigned int _nbIterations){
				nbIterations=_nbIterations;
			}
			/**
			 * @brief clear the scene
			 */
			void reset();

	};

	}

#endif // PARTICLESYSTEM_H
