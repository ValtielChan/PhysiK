#ifndef CONSTRAINT_H
#define CONSTRAINT_H
#include <vector>

namespace PhysiK {

	class vec3;


	/**
	 * @brief Compute the constraint (see 1st repport) to apply to the list of vertex
	 */
	class Constraint
	{
		private:

			/**
			 * @brief the list of vertex
			 */
			std::vector<vec3 *> positions;

		public:

			/**
			 * @brief Compute the value of the constraint to minimize
			 */
			virtual float eval();

			/**
			 * @brief Compute the value of the gradient
			 * @param vertex the vertex where to compute the gradient
			 */
			virtual float grad(vec3 * vertex);

			/**
			 * @brief Compute the value of lambda
			 * @param vertex the vertex where to compute the lambda value
			 */
			virtual float lambda(vec3 * vertex);

	};

}


#endif // CONSTRAINT_H
