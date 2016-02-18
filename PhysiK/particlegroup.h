#ifndef PARTICLEGROUP_H
#define PARTICLEGROUP_H
#include "physicobject.h"

namespace PhysiK {

	/**
	 * @brief Represents a set of particles that share the same attributes (radius and mass)
	 */
	class ParticleGroup : public PhysicObject
	{

			friend class Body;

		private:

			ParticleGroup(){}

		public:

			float mass;
			float radius;

			/**
			 * @param particleAmount is the amount of positions to be read from the data pointer
			 * @param dataPtr is the pointer to the positions
			 * @param dataStride is the number of bytes between 2 positions in the data from the pointer
			 *
			 * @param particleRadius will be the radius of all particles of this group
			 * @param particleMass will be the mass of all particles of this group
			 */
			ParticleGroup(int particleAmount, float* dataPtr, int dataStride,
						  float particleRadius, float particleMass, bool isKinematic = false);
	};

}

#endif // PARTICLEGROUP_H
