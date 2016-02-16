#include "body.h"
#include "triangle.h"

PhysiK::Body::Body(unsigned int nbParticles, unsigned int nbTriangles, bool isKinematic):
    PhysicObject(nbParticles,isKinematic),nbTriangles(nbTriangles){
    triangles = new Triangle[nbTriangles]();
}

const PhysiK::Triangle * PhysiK::Body::getTriangles() const{
    return triangles;
}

PhysiK::Triangle * PhysiK::Body::getTriangles(){
    return triangles;
}

PhysiK::Body::~Body(){
    delete[] triangles;
}
