#ifndef BODY_H
#define BODY_H
#include "physicobject.h"

namespace PhysiK {

	class Triangle;

	/**
	 * @brief Add topological information to a body
	 */
	class Body : PhysicObject
	{
		private:

			unsigned int nbTriangle;

			Triangle * triangles;

		public:

			//the constructor miss

			/**
			 * @brief return a pointer to the triangle for the user
			 */
			const Triangle * getTriangles();

	};

}

#endif // BODY_H
