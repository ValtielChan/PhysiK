#ifndef PHYSICOBJECT_H
#define PHYSICOBJECT_H

namespace PhysiK {

	class vec3;

	/**
	 * @brief Internal representation of an object
	 */
	class PhysicObject
	{
		protected:

			unsigned int nbParticle;

			vec3 * positions;

			vec3 * oldPositions;

			vec3 * velocities;

		public:

			/**
			 * @brief return a pointer to the position of the vertex for the user
			 */
			vec3 * getData();

			/**
			 * @brief make it virtual
			 * (won't compile if the polymorphism is not respected)
			 */
			virtual ~PhysicObject();

	};

	}

#endif // PHYSICOBJECT_H
