#include "body.h"
#include "triangle.h"
#include "particlegroup.h"

PhysiK::Body::Body(unsigned int nbParticles, unsigned int nbTriangles, float myMass, bool isKinematic):
	PhysicObject(nbParticles,isKinematic),nbTriangles(nbTriangles),mass(myMass){
	triangles = new Triangle[nbTriangles]();
	for(unsigned int i = 0 ; i < nbParticles ; i++)
		getPositions()[i].omega = isKinematic ? 0 : 1.f/(mass/float(nbParticles));
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

PhysiK::vec3 PhysiK::Body::computeBarycenter()
{
	barycenter = vec3(0);
    for(unsigned int i = 0; i < nbParticles; ++i)
        barycenter += particles[i].pos;
    barycenter /= nbParticles;
	return barycenter;
}

PhysiK::ParticleGroup * PhysiK::Body::getParticlesGroup(){
	ParticleGroup * group = new ParticleGroup();
	group->nbParticles = nbParticles;
	group->mass=mass;
	group->isKinematic=isKinematic;
	group->radius=0;
	group->oldPositions=oldPositions;
	group->particles=particles;
	return group;
}
