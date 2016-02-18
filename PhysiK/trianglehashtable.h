#ifndef TRIANGLEHASHTABLE_H
#define TRIANGLEHASHTABLE_H
#include <unordered_map>
#include <vector>
#include <tuple>
#include "vec3.h"
#include "intersection.h"

namespace PhysiK {

	class Body;
	class IntersectionParticleTriangle;
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

			/**
			 * @brief clear and free the hastable
			 */
			void clear(){
				voxelGrid.clear();
				voxelGrid.rehash(0);
			}
	};

	/**
	 * @brief A TriangleHash for triangles
	 */
	class TriangleHashTable : public HashTable<Body>{
			friend class ParticleHashTable;
		public:
			/**
			 * @brief add all the triangles of the body object to the triangle hashtable
			 */
			void addObject(Body * body);
	};

	/**
	 * @brief A TriangleHash for triangles
	 */
	class ParticleHashTable : public HashTable<ParticleGroup>{
		public:
			/**
			 * @brief add all the triangles of the particles objects to the particles hashtable
			 */
			void addObject(ParticleGroup* particles);
			/**
			 * @brief find all the intersection between the particles of the hastable/
			 * @param intersections For every intersection found, an IntersectionParticleTriangle object that represent the intersection is added the the vector
			 */
			void generateIntersection(std::vector<IntersectionParticleParticle> &intersections);
			/**
			 * @brief find all the intersection between a particles in the hastable and a triangle in the intersections object
			 * @param intersections For every intersection found, an IntersectionParticleTriangle object that represent the intersection is added the the vector
			 */
			void generateIntersectionWithTriangles(std::vector<IntersectionParticleTriangle> &intersections, TriangleHashTable &particlesHashtable);
	};


}

#endif // TRIANGLEHASHTABLE_H
