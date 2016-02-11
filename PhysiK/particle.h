#ifndef PARTICLE_H
#define PARTICLE_H
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
			Particle(vec3 particle, float omega = 1, vec3 vel = vec3(0,0,0)) : pos(particle), omega(omega), velocity(vel) {}

			/**
			 * @brief Default constructor
			 */
            Particle() : pos(), omega(0), velocity(vec3(0,0,0)) {}

	};

	}

#endif // PARTICLE_H
