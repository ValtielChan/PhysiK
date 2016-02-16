#include "body.h"
#include "triangle.h"

PhysiK::Body::Body(int nbParticles, int nbTriangles, bool kinematic) :
    PhysicObject(nbParticles),
    nbTriangles(nbTriangles)
{
    bool isKinematic = kinematic;
    triangles = new Triangle[nbTriangles]();
}

PhysiK::Body::~Body(){
    delete triangles;
}

const PhysiK::Triangle * PhysiK::Body::getTriangles() const{
    return triangles;
}

PhysiK::Triangle * PhysiK::Body::getTriangles(){
    return triangles;
}

void PhysiK::Body::computeBarycenter()
{
    float xSum = 0.f, ySum = 0.f, zSum = 0.f;
    for(unsigned int i = 0; i < nbParticles; ++i){

        xSum += particles[i].pos.x;
        ySum += particles[i].pos.y;
        zSum += particles[i].pos.z;
    }

    xSum /= nbParticles;
    ySum /= nbParticles;
    zSum /= nbParticles;

    barycenter = vec3(xSum, ySum, zSum);
}
