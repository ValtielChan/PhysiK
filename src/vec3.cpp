#include "vec3.h"
#include <cassert>

PhysiK::vec3::vec3(const float fst, const float snd, const float thr):
	positions{fst,snd,thr}{

}

const float& PhysiK::vec3::operator [](const unsigned int offset) const{
	assert(offset<3);
	return positions[offset];
}

float& PhysiK::vec3::operator [](const unsigned int offset){
	assert(offset<3);
	return positions[offset];
}
