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
			float omega;
			/**
			 * @brief Construct a Particle with a postion and a weight
			 * @param omega equal to 1/mass
			 */
<<<<<<< HEAD
			Particle(vec3 particule, float omega = 1) : pos(particule), omega(omega) {}

			/**
			 * @brief Default constructor
			 */
			Particle() : pos(), omega(0) {}
=======
            Particle(vec3 particule, float omega = 1) : pos(particule), omega(omega) {}

            /**
             * @brief Default constructor
             */
            Particle() : pos(), omega(1) {}
>>>>>>> fd1a7a1751377fcfdf1c741aabdcebe7430e4dfb

	};

	}

#endif // PARTICULE_H
