#ifndef BODY_H
#define BODY_H
#include "physicobject.h"

namespace PhysiK {

	class Triangle;
	class ParticleGroup;

	/**
	 * @brief Add topological informations to a body
	 */
	class Body : public PhysicObject
	{
		private:

			Triangle * triangles;

		public:

			vec3 barycenter;

			Body(unsigned int nbParticles,unsigned int nbTriangles, float myMass, bool isKinematic = false);

			~Body();

			unsigned int nbTriangles;
			float mass;

			/**
			 * @brief return a pointer to the triangles table for the user
			 */
			const Triangle * getTriangles() const;
			/**
			 * @brief return a pointer to the triangles table for the user
			 */
			Triangle * getTriangles();

			vec3 computeBarycenter();

			ParticleGroup * getParticlesGroup();

			vec3 getBarycenter() {return barycenter;}
	};

	}

#endif // BODY_H
