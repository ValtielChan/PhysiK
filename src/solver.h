#ifndef SOLVER_H
#define SOLVER_H
#include <vector>

namespace PhysiK {

	class Constraint;

	/**
	 * @brief Allow to set-up a system
	 */
	class Solver
	{
		private:

			std::vector<Constraint *> constraints;

			std::vector<Constraint *> temporaryConstraints;

		public:

			/**
			 * @brief Try to find a system where the value of the constraint are minimised
			 */
			void solve();

			/**
			 * @brief add a permanent constraint
			 */
			void pushConstraint(Constraint * constraint);

			/**
			 * @brief add a temporary constraint
			 */
			void pushTemporaryConstraint(Constraint * constraint);

			/**
			 * @brief clear all the temporary constraint
			 */
			void clearTemporaryConstriant();

	};

}

#endif // SOLVER_H
