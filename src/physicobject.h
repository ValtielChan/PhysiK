#ifndef PHYSICOBJECT_H
#define PHYSICOBJECT_H

namespace PhysiK {

	class vec3;

	class PhysicObject
	{
		private:

			unsigned int nbParticle;

			vec3 * positions;

			vec3 * oldPositions;

			vec3 * velocities;

		public:

			vec3 * getData();

			/**
			 * @brief make it virtual
			 * (won't compile if the polymorphism is not respected)
			 */
			virtual ~PhysicObject();

	};

}

#endif // PHYSICOBJECT_H
