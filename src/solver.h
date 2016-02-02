#ifndef SOLVER_H
#define SOLVER_H
#include <vector>

namespace PhysiK {

	class Constraint;

	class Solver
	{
		private:

			std::vector<Constraint *> constraints;

			std::vector<Constraint *> temporaryConstraints;

		public:

			void solve();

			void pushConstraint(Constraint * constraint);

			void pushTemporaryConstraint(Constraint * constraint);

			void clearTemporaryConstriant();

	};

}

#endif // SOLVER_H
