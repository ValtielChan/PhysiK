#ifndef TRIANGLEHASHTABLE_H
#define TRIANGLEHASHTABLE_H
#include <unordered_map>
#include <vector>
#include <tuple>
#include "vec3.h"

namespace PhysiK {

	class PhysicObject;
	class Intersection;

	/**
	 * @brief Efficient data structure to generate intersection
	 */
	class TriangleHashTable
	{

		private:

			std::unordered_map<vec3,std::tuple<PhysicObject *, unsigned int>,vec3> Vortex;

		public:

			/**
			 * @brief guess what
			 * @param intersections vector where to store the intersections
			 */
			void generateIntersection(std::vector<Intersection>& intersections);

	};

}

#endif // TRIANGLEHASHTABLE_H
