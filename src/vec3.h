#ifndef VEC3_H
#define VEC3_H

namespace PhysiK {

	class vec3
	{
		public:

			/**
			 * @brief hash the parameter
			 * @param victim the victim
			 * @return
			 */
			unsigned int operator()(const vec3& victim) const;
	};

}

#endif // VEC3_H
