#include "body.h"
#include "triangle.h"

PhysiK::Body::Body(unsigned int nbParticles, unsigned int nbTriangles, float myMass, bool isKinematic):
    PhysicObject(nbParticles,isKinematic),nbTriangles(nbTriangles),mass(myMass){
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

void PhysiK::Body::computeBarycenter()
{
    barycenter = vec3(0);
    for(unsigned int i = 0; i < nbParticles; ++i)
        barycenter += particles[i].pos;
    barycenter /= nbParticles;
}
