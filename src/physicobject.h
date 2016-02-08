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

            PhysicObject();

			Particle * positions;

			vec3 * oldPositions;

            vec3 * velocities;

		public:

			unsigned int nbParticles;
			Particle barycenter;

			/**
			 * @brief return a pointer to the position of the vertices for the user
			 */
			Particle * getPositions();

            /**
             * @brief compute de barycenter of all particles *_*
             */
            void computeBarycenter();

			/**
			 * @brief make it virtual
			 * (won't compile if the polymorphism is not respected)
			 */
			virtual ~PhysicObject();

	};

	}

#endif // PHYSICOBJECT_H
