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

            vec3 barycenter;
			Triangle * triangles;

		public:

            Body(unsigned int nbParticles,unsigned int nbTriangles, float myMass, bool isKinematic = false);

            ~Body();

			unsigned int nbTriangles;
            float mass;

			/**
			 * @brief return a pointer to the triangle for the user
			 */
			const Triangle * getTriangles() const;
			/**
			 * @brief return a pointer to the triangle for the user
			 */
			Triangle * getTriangles();

            void computeBarycenter();

            vec3 getBarycenter() {return barycenter;}
	};

	}

#endif // BODY_H
