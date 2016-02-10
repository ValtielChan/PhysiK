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

			ParticleGroup * colider;
			unsigned int particuleColider;
			Particle * particule;
			float radius;

		public:

			IntersectionParticuleParticule(ParticleGroup * colider, unsigned int particuleColider,Particle * particule,float radius)
				:colider(colider),particuleColider(particuleColider),particule(particule),radius(radius){}
			bool intersect()const;
			Constraint * getConstraint() const;

	};

}

#endif // INTERSECTION_H
