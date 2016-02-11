#ifndef PHYSICOBJECT_H
#define PHYSICOBJECT_H

#include "particle.h"

namespace PhysiK {

	class vec3;
	class Particle;

	/**
	 * @brief Internal representation of an object
	 */
	class PhysicObject
	{
		protected:

			PhysicObject(int nbPosition);

            Particle * positions;

            vec3 * newPositions;

            vec3 * velocities;

		public:

			unsigned int nbParticles;
			vec3 barycenter;

			/**
             * @brief return a pointer to the positions of the vertices for the user
			 */
			Particle * getPositions();

            /**
             * @brief return a pointer to the velocities
             */
            vec3 * getVelocities();

			/**
			 * @brief compute de barycenter of all particles *_*
			 */
			void computeBarycenter();

			/**
			 * @brief preUpdate and postUpdate are two stages of one simulation step
			 * the first is called at the beginning of the integrator loop, and the second
			 * just after solving the constraints system.
			 */
			void preUpdate(float dt, float gravity, float damping);
			void postUpdate(float dt);

			/**
			 * @brief make it virtual
			 * (won't compile if the polymorphism is not respected)
			 */
			virtual ~PhysicObject();

	};

	}

#endif // PHYSICOBJECT_H
