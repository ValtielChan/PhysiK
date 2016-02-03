#include "vec3.h"
#include <cassert>

PhysiK::vec3::vec3(const unsigned int fst, const unsigned int snd, const unsigned int thr):
	positions{fst,snd,thr}
{

}

const unsigned int& PhysiK::vec3::operator [](const unsigned int offset) const
{
	assert(offset<3);
	return positions[offset];
}

unsigned int& PhysiK::vec3::operator [](const unsigned int offset)
{
	assert(offset<3);
	return positions[offset];
}
