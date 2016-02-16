#ifndef BODY_H
#define BODY_H
#include "physicobject.h"

namespace PhysiK {

	class Triangle;

	/**
	 * @brief Add topological information to a body
	 */
	class Body : public PhysicObject
	{
		private:

			Triangle * triangles;

		public:

			Body(unsigned int nbParticles,unsigned int nbTriangles, bool isKinematic = false);

			unsigned int nbTriangles;

			/**
			 * @brief return a pointer to the triangle for the user
			 */
			const Triangle * getTriangles() const;
			/**
			 * @brief return a pointer to the triangle for the user
			 */
			Triangle * getTriangles();

			~Body();

	};

	}

#endif // BODY_H
