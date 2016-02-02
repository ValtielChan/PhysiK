#ifndef CONSTRAINT_H
#define CONSTRAINT_H
#include <vector>

namespace PhysiK {

	class vec3;

	class Constraint
	{
		private:

			std::vector<vec3 *> positions;

		public:

			virtual float eval();

			virtual float grad(vec3 * vertex);

			virtual float lambda(vec3 * vertex);

	};

}


#endif // CONSTRAINT_H
