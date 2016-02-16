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

	class DistanceConstraint : public Constraint
	{
			float dst;
		public:
			DistanceConstraint(Particle *pos1, Particle *pos2);
			float eval() const;
	};

	class MinDistanceConstraint : public Constraint
	{
			float dst;
		public:
			MinDistanceConstraint(Particle *pos1, Particle *pos2, float dst);
			float eval() const;
	};

	class CollisionParticuleTriangleConstraint : public Constraint{
			float size;
		public:
			/**
			 * @brief construct a CollisionConstraint with a triangle.
			 * the triangle is defined clock-wise (maybe) with tree vertices(pt1, pt2, pt3)
			 */
			CollisionParticuleTriangleConstraint(Particle *pos, Particle *pt1, Particle *pt2, Particle *pt3, float size=0);
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
			CollisionConstraint(Particle *particle, vec3 normal, float delta);
			float eval() const;
	};

	class VolumeConstraint : public Constraint{
		private:
			float delta = 0;
		public:
			void addVolume(Particle * p1,Particle * p2,Particle * p3,Particle * p4);
			float volume(vec3 p1, vec3 p2, vec3 p3, vec3 p4) const;
			float eval() const;
	};


}


#endif // CONSTRAINT_H
