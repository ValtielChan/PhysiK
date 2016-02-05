#ifndef VEC3_H
#define VEC3_H

namespace PhysiK {

	/**
	 * @brief A vertex with an omega value
	 */
	class vec3
	{

		public:

			float x,y,z;
			float omega;

			vec3(const float fst = 0, const float snd = 0, const float thr = 0, const float omega = 0);

			/**
			 * @brief hash the parameter
			 * @param victim the victim
			 */
			unsigned int operator()(const vec3& victim) const;

			const float& operator[](const unsigned int offset) const;

			float& operator[](const unsigned int offset);

			vec3 operator*(const float snd) const;

			vec3 operator+(const vec3 snd) const;

			vec3 operator/(const float snd) const;

			vec3 operator-(const vec3 snd) const;

			float length() const;

			float setPos(const vec3 snd);

	};

}

#endif // VEC3_H
