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
			float omega;
			vec3 velocity;
			vec3 impulsion;
			/**
			 * @brief Construct a Particle with a postion and a weight
			 * @param omega equal to 1/mass
			 */
			Particle(vec3 particle, float omega = 1, vec3 velocity = vec3(0.f,0.f,0.f)) : pos(particle), omega(omega), velocity(velocity) {}

			/**
			 * @brief Default constructor
			 */
			Particle() : pos(), omega(0), velocity(vec3(0.f,0.f,0.f)) {}

	};

	}

#endif // PARTICLE_H
