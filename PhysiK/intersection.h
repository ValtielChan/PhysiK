#ifndef INTERSECTION_H
#define INTERSECTION_H
#include "vec3.h"

namespace PhysiK {

	class Constraint;
	class Body;
	class Particle;
	class ParticleGroup;

	class IntersectionParticuleTriangle
	{
		private:

			Body *colider;
			unsigned int triangle;
			Particle * particule;

		public:

			IntersectionParticuleTriangle(Body * colider, unsigned int triangle, Particle * particule)
				:colider(colider),triangle(triangle),particule(particule){}

			bool intersect(vec3 oldPostion) const;

			Constraint * getConstraint() const;

	};

	class IntersectionParticuleParticule
	{

		private:

            Particle * particle1;
            Particle * particle2;
			float radius;

		public:

            IntersectionParticuleParticule(Particle * particle1,Particle * particle2,float radius)
                :particle1(particle1),particle2(particle2),radius(radius){}
			bool intersect()const;
			Constraint * getConstraint() const;

            Particle *getParticle1() { return particle1; }
            Particle *getParticle2() { return particle2; }
	};

}

#endif // INTERSECTION_H
