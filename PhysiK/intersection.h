#ifndef INTERSECTION_H
#define INTERSECTION_H
#include "vec3.h"

namespace PhysiK {

	class Constraint;
	class Body;
	class Particle;
	class ParticleGroup;

	class IntersectionParticleTriangle
	{
		private:

			Body *colider;
			unsigned int triangle;
			Particle * particle;
			float size;

		public:

			IntersectionParticleTriangle(Body * colider, unsigned int triangle, Particle * particle,float size=0)
				:colider(colider),triangle(triangle),particle(particle),size(size){}

			bool intersect(vec3 oldPostion) const;
			bool intersect() const;

			Constraint * getConstraint();

	};

	class IntersectionParticleParticle
	{

		private:

			ParticleGroup * particles1;
			ParticleGroup * particles2;
			unsigned int offset1;
			unsigned int offset2;

		public:

			IntersectionParticleParticle(ParticleGroup * particle1, unsigned int offset1,ParticleGroup * particle2,unsigned int offset2)
				:particles1(particle1),particles2(particle2),offset1(offset1),offset2(offset2){}
			bool intersect()const;
			vec3 intersectPos()const;
			Constraint * getConstraint() const;
			Particle *getParticle1() const;
			Particle *getParticle2() const;
			vec3 getWorks1(float deltaT) const;
			vec3 getWorks2(float deltaT) const;
	};

}

#endif // INTERSECTION_H
