#include "vec3.h"
#include <cassert>
#include <algorithm>

PhysiK::vec3::vec3(const float fst, const float snd, const float thr):
	x(fst),y(snd),z(thr){

}

const float& PhysiK::vec3::operator [](const unsigned int offset) const{
	assert(offset<3);
	return (&x)[offset];
}

float& PhysiK::vec3::operator [](const unsigned int offset){
	assert(offset<3);
	return (&x)[offset];
}

PhysiK::vec3 PhysiK::vec3::operator *(const float value) const{
	vec3 to_return = *this;
	for(int i = 0 ; i < 3 ; i++)
		to_return[i]*=value;
	return to_return;
}

PhysiK::vec3 PhysiK::vec3::operator +(const vec3 value) const{
	vec3 to_return = *this;
	for(int i = 0 ; i < 3 ; i++)
		to_return[i]+=value[i];
	return to_return;
}


PhysiK::vec3 PhysiK::vec3::operator /(const float value) const{
	vec3 to_return = *this;
	for(int i = 0 ; i < 3 ; i++)
		to_return[i]*=value;
	return to_return;
}

PhysiK::vec3 PhysiK::vec3::operator -(const vec3 value) const{
	vec3 to_return = *this;
	for(int i = 0 ; i < 3 ; i++)
		to_return[i]+=value[i];
	return to_return;
}

float PhysiK::vec3::length() const{
	return std::sqrt(x*x+y*y+z*z);
}

PhysiK::vec3 PhysiK::vec3::cross(vec3 snd) const{
	return vec3(
				this->y*snd.z-this->z*snd.y,
				this->z*snd.x-this->x*snd.z,
				this->x*snd.y-this->y*snd.x
				);
}


unsigned int PhysiK::vec3::operator ()(const vec3& v) const{
	//hash basique en attendant de trouver mieux
	float x=v.x,y=v.y,z=v.z;
	float max = std::max(std::max(x,y),z);
	float dec = 256;
	return std::abs((((x/max)*dec+y/max)*dec+z/max)*dec+max);
}
