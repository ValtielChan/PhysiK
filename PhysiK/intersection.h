#ifndef INTERSECTION_H
#define INTERSECTION_H
#include "vec3.h"

namespace PhysiK {

	class Constraint;
	class Body;
	class Particle;

	class Intersection
	{
		private:

			Body *colider;

			unsigned int triangle;

			Particle * particule;

		public:

			Intersection(Body * colider, unsigned int triangle)
				:colider(colider),triangle(triangle){}

			bool intersect(vec3 oldPostion) const;

			Constraint * getConstraint() const;

	};

}

#endif // INTERSECTION_H
