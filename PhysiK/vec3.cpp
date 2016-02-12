#include "vec3.h"
#include <cassert>
#include <algorithm>
#include <limits.h>

float PhysiK::vec3::voxelSize = 2;

PhysiK::vec3::vec3(const float fst, const float snd, const float thr):
	x(fst),y(snd),z(thr){
}

const float& PhysiK::vec3::operator[](const unsigned int offset) const{
	assert(offset<3);
	return (&x)[offset];
}

float& PhysiK::vec3::operator[](const unsigned int offset){
	assert(offset<3);
	return (&x)[offset];
}

PhysiK::vec3 PhysiK::vec3::operator*(const float value) const{
	vec3 to_return = *this;
	for(int i = 0 ; i < 3 ; i++)
		to_return[i]*=value;
	return to_return;
}

PhysiK::vec3 &PhysiK::vec3::operator*=(const float value){
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

PhysiK::vec3 PhysiK::vec3::operator+(const vec3 value) const{
	vec3 to_return = *this;
	for(int i = 0 ; i < 3 ; i++)
		to_return[i]+=value[i];
    return to_return;
}

PhysiK::vec3& PhysiK::vec3::operator+=(const PhysiK::vec3 value)
{
    for(int i = 0 ; i < 3 ; i++)
        (*this)[i]+=value[i];
    return *this;
}

PhysiK::vec3 PhysiK::vec3::operator/(const float value) const{
	vec3 to_return = *this;
	for(int i = 0 ; i < 3 ; i++)
		to_return[i]/=value;
	return to_return;
}

PhysiK::vec3 PhysiK::vec3::operator-(const vec3 value) const{
	vec3 to_return = *this;
	for(int i = 0 ; i < 3 ; i++)
		to_return[i]-=value[i];
	return to_return;
}

bool PhysiK::vec3::operator==(const vec3 value) const{
    return x==value.x && y==value.y && z==value.z;
}

PhysiK::vec3 PhysiK::vec3::opposite() const
{
    return vec3(-this->x, -this->y, -this->z);
}


float PhysiK::vec3::length() const{
    return std::sqrt(lengthSquared());
}

float PhysiK::vec3::lengthSquared() const{
    return x*x+y*y+z*z;
}

PhysiK::vec3 PhysiK::vec3::normalize() const
{
    float len = length();
    return vec3(this->x / len, this->y / len, this->y / len);
}

PhysiK::vec3 PhysiK::vec3::cross(vec3 snd) const{
	return vec3(
				this->y*snd.z-this->z*snd.y,
				this->z*snd.x-this->x*snd.z,
				this->x*snd.y-this->y*snd.x
				);
}

float PhysiK::vec3::dot(vec3 snd) const{
	return
			 this->x*snd.x
			+this->y*snd.y
			+this->z*snd.z;
}

PhysiK::vec3 PhysiK::vec3::toVoxel(){
    return vec3(int(x/vec3::voxelSize),int(y/vec3::voxelSize),int(z/vec3::voxelSize));
}

unsigned int PhysiK::vec3::operator()(const vec3& v) const{

	const unsigned int limit = std::pow(UINT_MAX,1.f/3.f)-1;

	unsigned int x = int(v.x) % limit;
	unsigned int y = int(v.y) % limit;
	unsigned int z = int(v.z) % limit;

	return x+y*limit+z*limit*limit;
}
