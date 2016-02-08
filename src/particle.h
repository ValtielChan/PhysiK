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
            Particle(vec3 particule, float omega) : pos(particule), omega(omega) {}

            /**
             * @brief Default constructor
             */
            Particle() : pos(), omega(0) {}

	};

}

#endif // PARTICULE_H
