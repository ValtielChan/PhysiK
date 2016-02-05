#ifndef CONSTRAINT_H
#define CONSTRAINT_H
#include <vector>

namespace PhysiK {

	class vec3;

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
			std::vector<vec3 *> positions;

		public:

			/**
			 * @brief Compute the value of the constraint to minimize
			 */
			virtual float eval() = 0;

			/**
			 * @brief Compute the value of the gradient
			 * @param vertex the vertex where to compute the gradient
			 */
			virtual vec3 grad(const vec3 * vertex);

			/**
			 * @brief Compute the value of lambda
			 * @warning there is a second definition of lambda with a sigma on the paper
			 */
			virtual float lambda();
	};

    // Concrete constraints (To put in separate files maybe)
    class DistanceConstraint : public Constraint
    {
    public:
        DistanceConstraint(vec3 *pos1, vec3 *pos2);
        float eval();
    };

}


#endif // CONSTRAINT_H
