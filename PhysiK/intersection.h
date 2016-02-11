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

		public:

			IntersectionParticleTriangle(Body * colider, unsigned int triangle, Particle * particle)
				:colider(colider),triangle(triangle),particle(particle){}

			bool intersect(vec3 oldPostion) const;

			Constraint * getConstraint() const;

	};

	class IntersectionParticleParticle
	{

		private:

            Particle * particle1;
            Particle * particle2;
			float radius;

		public:

            IntersectionParticleParticle(Particle * particle1,Particle * particle2,float radius)
                :particle1(particle1),particle2(particle2),radius(radius){}
			bool intersect()const;
			Constraint * getConstraint() const;

            Particle *getParticle1() { return particle1; }
            Particle *getParticle2() { return particle2; }
	};

}

#endif // INTERSECTION_H
