#ifndef VEC3_H
#define VEC3_H

#include <iostream>

namespace PhysiK {

	/**
	 * @brief A vertex
	 */
	class vec3
	{

		public:

			float x,y,z;

			vec3(const float fst = 0, const float snd = 0, const float thr = 0);

			/**
			 * @brief hash the parameter
			 */
			unsigned int operator()(const vec3& to_hash) const;

			const float& operator[](const unsigned int offset) const;

			float& operator[](const unsigned int offset);

			vec3 operator*(const float value) const;

			vec3 operator+(const vec3 value) const;

			vec3 operator+(const float value) const;

			vec3& operator+=(const vec3 value);

			vec3& operator-=(const vec3 value);

			vec3 operator/(const float value) const;

			vec3 operator-(const vec3 value) const;

			vec3 &operator*=(const float value);

			bool operator ==(const vec3 value) const;

			vec3 operator-() const;

			float length() const;

			float lengthSquared() const;

			vec3 normalize() const;

			vec3 cross(vec3 value) const;

			float dot(vec3 value) const;

			vec3 toVoxel() const;

			vec3 center() const;

            void print();

			//these value should affect performance
			static float voxelSize;//un truc très grand pour commencer

	};

}

#endif // VEC3_H
