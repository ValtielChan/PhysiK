#include "triangle.h"
#include <cassert>

PhysiK::Triangle::Triangle(const unsigned int fst, const unsigned int snd, const unsigned int thr):
	vertexOffset{fst,snd,thr}{

}

const unsigned int& PhysiK::Triangle::operator [](const unsigned int offset) const{
	assert(offset<3);
	return vertexOffset[offset];
}

unsigned int& PhysiK::Triangle::operator [](const unsigned int offset){
	assert(offset<3);
	return vertexOffset[offset];
}
