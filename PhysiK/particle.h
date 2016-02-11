#ifndef PARTICULE_H
#define PARTICULE_H
#include "vec3.h"

namespace PhysiK {

	/**
	 * @brief The Particle class
	 */
	class Particle
	{
		public:

			vec3 pos;
            vec3 velocity;
			float omega;
			/**
			 * @brief Construct a Particle with a postion and a weight
			 * @param omega equal to 1/mass
			 */
            Particle(vec3 particule, float omega = 1, vec3 vel = vec3(0,0,0)) : pos(particule), omega(omega), velocity(vel) {}

			/**
			 * @brief Default constructor
			 */
            Particle() : pos(), omega(0), velocity(vec3(0,0,0)) {}

	};

	}

#endif // PARTICULE_H
