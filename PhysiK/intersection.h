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

			Particle * particule1;
			Particle * particule2;
			float radius;

		public:

			IntersectionParticuleParticule(Particle * particule1,Particle * particule2,float radius)
				:particule1(particule1),particule2(particule2),radius(radius){}
			bool intersect()const;
			Constraint * getConstraint() const;

	};

}

#endif // INTERSECTION_H
