#ifndef INTERSECTION_H
#define INTERSECTION_H
#include "vec3.h"

namespace PhysiK {

	class Constraint;
	class Body;
	class Particle;
	class ParticleGroup;

	/**
	 * @brief represent an intersection between a particles and a trianlge
	 */
	class IntersectionParticleTriangle
	{
		private:

			Body *colider;
			unsigned int triangle;
			Particle * particle;
			float size;

		public:

			/**
			 * @brief IntersectionParticleTriangle
			 * @param colider the Body where is the triangle
			 * @param triangle the offset of the trianle in the Body
			 * @param particle the particle
			 * @param size the size of the particle
			 */
			IntersectionParticleTriangle(Body * colider, unsigned int triangle, Particle * particle,float size=0)
				:colider(colider),triangle(triangle),particle(particle),size(size){}

			/**
			 * @brief return if there is an intersection between the triangle, and the segment defined by the center of the partcle and the position in the argument
			 */
			bool intersect(vec3 oldPostion) const;
			/**
			 * @brief return if there is an intersection between the particle and the triangle
			 */
			bool intersect() const;
			/**
			 * @brief return the constraint corresponding to the intersection
			 */
			Constraint * getConstraint();

	};

	/**
	 * @brief represent an intersection between two particles
	 */
	class IntersectionParticleParticle
	{

		private:

			ParticleGroup * particles1;
			ParticleGroup * particles2;
			unsigned int offset1;
			unsigned int offset2;

		public:

			/**
			 * @brief IntersectionParticleParticle
			 *
			 * @param particle1 ParticleGroup where is the first particle
			 * @param offset1 the offset of the first particle in the particle group
			 * @param particle1 ParticleGroup where is the second particle
			 * @param offset1 the offset of the second particle
			 */
			IntersectionParticleParticle(ParticleGroup * particle1, unsigned int offset1,ParticleGroup * particle2,unsigned int offset2)
				:particles1(particle1),particles2(particle2),offset1(offset1),offset2(offset2){}
			/**
			 * @brief return if there is an intersection between the two particles
			 */
			bool intersect()const;
			/**
			 * @brief get the postion of the contact point
			 */
			vec3 intersectPos()const;
			/**
			 * @brief return the constraint corresponding to the intersection
			 */
			Constraint * getConstraint() const;
			Particle *getParticle1() const;
			Particle *getParticle2() const;
			vec3 getWorks1(float deltaT) const;
			vec3 getWorks2(float deltaT) const;
	};

}

#endif // INTERSECTION_H
