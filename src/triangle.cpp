#include "triangle.h"
#include <cassert>

PhysiK::Triangle::Triangle(const unsigned int fst, const unsigned int snd, const unsigned int thr):
	VertexOffset{fst,snd,thr}{

}

const unsigned int& PhysiK::Triangle::operator [](const unsigned int offset) const{
	assert(offset<3);
	return VertexOffset[offset];
}

unsigned int& PhysiK::Triangle::operator [](const unsigned int offset){
	assert(offset<3);
	return VertexOffset[offset];
}
