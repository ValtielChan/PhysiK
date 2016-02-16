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
            vec3 barycenter;

		public:

            Body(int nbParticles, int nbTriangles, float mass, bool kinematic = true);
            ~Body();

			unsigned int nbTriangles;
            float mass;

			/**
             * @brief return a pointer to the triangles for the user
			 */
			const Triangle * getTriangles() const;
			/**
             * @brief return a pointer to the triangles for the user
			 */
			Triangle * getTriangles();

            void computeBarycenter();

            vec3 getBarycenter() {return barycenter;}
	};

	}

#endif // BODY_H
