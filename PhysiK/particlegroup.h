#ifndef PARTICLEGROUP_H
#define PARTICLEGROUP_H
#include "physicobject.h"

namespace PhysiK {

	/**
	 * @brief Represents a set of particle
	 * that share the same baic attributes (radius and mass)
	 */
	class ParticleGroup : public PhysicObject
	{

		private:

			float radius;
			float mass;

		public:

			/**
			 * @param particleAmount is the amount of positions to be read from the data pointer
			 * @param dataPtr is the pointer to the positions
			 * @param dataStride is the number of bytes between 2 positions in the data from the pointer
			 *
			 * @param particleRadius will be the radius of all particles of this group
			 * @param particleMass will be the mass of all particles of this group
			 */
			ParticleGroup(int particleAmount, char* dataPtr, int dataStride,
						  float particleRadius, float particleMass);

			~ParticleGroup();
	};

	}

#endif // PARTICLEGROUP_H
