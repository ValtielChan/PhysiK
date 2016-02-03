#ifndef VEC3_H
#define VEC3_H

namespace PhysiK {

	/**
	 * @brief A vertex
	 */
	class vec3
	{

		private:

			unsigned int positions[3];

		public:

			vec3(const unsigned int fst, const unsigned int snd, const unsigned int thr);

			/**
			 * @brief hash the parameter
			 * @param victim the victim
			 * @return
			 */
			unsigned int operator()(const vec3& victim) const;

			const unsigned int& operator[](const unsigned int offset) const;

			unsigned int& operator[](const unsigned int offset);

	};

}

#endif // VEC3_H
