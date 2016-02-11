#ifndef TRIANGLEHASHTABLE_H
#define TRIANGLEHASHTABLE_H
#include <unordered_map>
#include <vector>
#include <tuple>
#include "vec3.h"

namespace PhysiK {

	class Body;
	class IntersectionParticuleTriangle;
	class IntersectionParticuleParticule;
	class ParticleGroup;

	/**
	 * @brief Efficient data structure to generate intersection
	 */
	template <typename Object>
	class HashTable
	{

		protected:

			using m_pair = std::pair<Object *, unsigned int>;
			using m_vector = std::vector<m_pair>;
			using m_map = std::unordered_map<vec3,m_vector,vec3>;
            m_map voxelGrid;

		public:

			void clear(){
                voxelGrid.clear();
                voxelGrid.rehash(0);
			}

	};

	class TriangleHashTable : public HashTable<Body>{
		public:
			void generateIntersection(std::vector<IntersectionParticuleTriangle> &intersections);
			void addObject(Body * body);
	};

	class ParticleHashTable : public HashTable<ParticleGroup>{
		public:
			void generateIntersection(std::vector<IntersectionParticuleParticule> &intersections);
			void addObject(ParticleGroup* body);
	};

}

#endif // TRIANGLEHASHTABLE_H
