#include "vec3.h"
#include <cassert>

PhysiK::vec3::vec3(const float fst, const float snd, const float thr, const float omega):
	x(fst),y(snd),z(thr),omega(omega){

}

const float& PhysiK::vec3::operator [](const unsigned int offset) const{
	assert(offset<4);
	return (&x)[offset];
}

float& PhysiK::vec3::operator [](const unsigned int offset){
	assert(offset<4);
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
	for(int i = 0 ; i < 4 ; i++)
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
	for(int i = 0 ; i < 4 ; i++)
		to_return[i]+=value[i];
	return to_return;
}

