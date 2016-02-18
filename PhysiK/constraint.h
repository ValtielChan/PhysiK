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
			 * @brief the list of vertices
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
			 */
			virtual float lambda();
			virtual ~Constraint(){}
	};

	/**
	 * @brief enforce two particles to stay at the same distance.
	 * eval() is equal to zero only when the two particles are at the same distance
	 */
	class DistanceConstraint : public Constraint
	{
			float dst;
		public:
			DistanceConstraint(Particle *pos1, Particle *pos2);
			float eval() const;
	};

	/**
	 * @brief enforce a minimum distance between two particles.
	 * eval() is equal to zero only when the distance is greater than dst
	 */
	class MinDistanceConstraint : public Constraint
	{
			float dst;
		public:
			MinDistanceConstraint(Particle *pos1, Particle *pos2, float dst);
			float eval() const;
	};

	/**
	 * @brief enforce a maximum distances between two particles.
	 * eval() is equal to zero only when the distance is lower than dst
	 */
	class MaxDistanceConstraint : public Constraint
	{
			float dst;
		public:
			MaxDistanceConstraint(Particle *pos1, Particle *pos2, float dst);
			float eval() const;
	};

	/**
	 * @brief prevent a triangle and a particle from collision.
	 * If the particle size is zero, eval() is equal to zero only above the triangle
	 */
	class CollisionParticuleTriangleConstraint : public Constraint{
			float size;
		public:
			/**
			 * @brief construct a CollisionConstraint with a triangle.
			 * the triangle is defined clock-wise (maybe) with tree vertices(pt1, pt2, pt3)
			 * @param pos the particle
			 * @param size the size of the particle
			 */
			CollisionParticuleTriangleConstraint(Particle *pos, Particle *pt1, Particle *pt2, Particle *pt3, float size=0);
			float eval() const;
	};


	/**
	 * @brief keep a particle above a plane.
	 * eval() is equal to zero only when the particles is above the plane
	 */
	class CollisionConstraint : public Constraint{
		private:
			/**
			 * @brief the normal of the plane
			 * @warning the upper of the plane is in the direction of the normal
			 */
			vec3 normal;
			/**
			 * @brief displacement of the plane from the origin
			 * compute it with pos.x * x + pos.y * y + pos.z*z where (x,y,z) is a point of the plane
			 */
			float delta;
		public:
			/**
			 * @brief construct a CollisionConstraint with the plane equation
			 * @param particle the particle to keep above the plane
			 * @param normal the normal of the plane
			 * @param delta delta = (distance of the plane form the orgin) * normal.normalize(). You can compute it wiht normal.dot(a vertex in the plane)
			 */
			CollisionConstraint(Particle *particle, vec3 normal, float delta);
			float eval() const;
			/**
			 * @brief eval the constraint without instancing the class. The parameter
			 * are the same that the constructor
			 * @return the value of the constraint
			 */
			static float quickEval(const Particle * particle, vec3 normal, float delta);
	};

	/**
	 * @brief Keep the sum of all tetraedres added constant.
	 * eval() is equal to zero when the volume of the tetraedres are equal to delta
	 */
	class VolumeConstraint : public Constraint{
		private:
			float delta = 0;
		public:
			/**
			 * @brief add a tetraedre defined by the four partilces p1 p2 p3 and p4
			 * delta is incremented by the volume of the tetraedre
			 */
			void addVolume(Particle * p1,Particle * p2,Particle * p3,Particle * p4);
			/**
			 * @brief compute the total volume
			 */
			float volume(vec3 p1, vec3 p2, vec3 p3, vec3 p4) const;
			float eval() const;
	};


}


#endif // CONSTRAINT_H
