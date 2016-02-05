#ifndef VEC3_H
#define VEC3_H

namespace PhysiK {

	/**
	 * @brief A vertex
	 */
	class vec3
	{

		private:

			float positions[3];

		public:

			vec3(const float fst, const float snd, const float thr);

			/**
			 * @brief hash the parameter
			 * @param victim the victim
			 * @return
			 */
			unsigned int operator()(const vec3& victim) const;

			const float& operator[](const unsigned int offset) const;

			float& operator[](const unsigned int offset);

	};

}

#endif // VEC3_H
