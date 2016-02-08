#ifndef PHYSICOBJECT_H
#define PHYSICOBJECT_H

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

			unsigned int nbParticles;

			Particle * positions;

			vec3 * oldPositions;

			vec3 * velocities;

		public:

			/**
             * @brief return a pointer to the position of the vertices for the user
             */
            Particle * getPositions();

			/**
			 * @brief make it virtual
			 * (won't compile if the polymorphism is not respected)
			 */
			virtual ~PhysicObject();

	};

	}

#endif // PHYSICOBJECT_H
