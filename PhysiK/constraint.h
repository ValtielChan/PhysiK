#ifndef CONSTRAINT_H
#define CONSTRAINT_H
#include <vector>
#include "vec3.h"

namespace PhysiK {

	class vec3;
	class Particle;

	/**
	 * @brief Compute the constraint (see 1st report) to apply to the list of vertices
	 */
	class Constraint
	{

			friend class Solver;

		protected:

			/**
			 * @brief the list of vertex
			 */
			std::vector<Particle *> positions;

		public:

			/**
			 * @brief Compute the value of the constraint to minimize
			 */
			virtual float eval() const = 0;

			/**
			 * @brief Compute the value of the gradient
			 * @param vertex the vertex where to compute the gradient
			 */
			virtual vec3 grad(vec3 * vertex);

			/**
			 * @brief Compute the value of lambda
			 * @warning there is a second definition of lambda with a sigma on the paper
			 */
			virtual float lambda();
			virtual ~Constraint(){}
	};

	// Concrete constraints (To put in separate files maybe)
	class DistanceConstraint : public Constraint
	{
			float dst;
			bool min;
		public:
			DistanceConstraint(Particle *pos1, Particle *pos2, float dst = 0);
			float eval() const;
	};

	class CollisionConstraint : public Constraint{
		private:
			/**
			 * @brief the normal of the plan
			 * @warning the upper of the plan is in the direction of the normal
			 */
			vec3 normal;
			/**
			 * @brief displacement of the plan from the origin
			 * compute it with pos.x * x + pos.y * y + pos.z*z where (x,y,z) is a point of the plan
			 */
			float delta;
		public:
			/**
			 * @brief construct a CollisionConstraint with the plan equation
			 */
			CollisionConstraint(Particle *pos, vec3 normal, float delta);
			/**
			 * @brief construct a CollisionConstraint with a triangle.
			 * the triangle is defined clock-wise (maybe) with tree vertices(pt1, pt2, pt3)
			 */
			CollisionConstraint(Particle *pos, vec3 pt1, vec3 pt2, vec3 pt3);
			float eval() const;
	};

	/*class VolumeConstraint : public Constraint{
		private:
			float volume;
			vec3 center;
		public:
			VolumeConstraint(vec3 center);
			float volume();
			float eval();
	};*/


}


#endif // CONSTRAINT_H
