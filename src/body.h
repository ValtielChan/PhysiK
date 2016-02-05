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

			unsigned int nbTriangle;

			Triangle * triangles;

		public:

            Body();
            ~Body();

			/**
			 * @brief return a pointer to the triangle for the user
			 */
			const Triangle * getTriangles();

	};

}

#endif // BODY_H
