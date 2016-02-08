#include "particlesystem.h"


void PhysiK::ParticleSystem::addRigidBody(PhysiK::Body *body)
{
    PhysiK::PhysicObject *temp = body;
    physicObjecs.push_back(temp);

    // Constraint building
    // Set of distance constraints for RigidBody

    // Barycenter computing (lourd, à voir si on garde)
    Particle* bodyParticles = body->getPositions();
    int nbParticles = body->nbParticles;

    float xSum = 0.f, ySum = 0.f, zSum = 0.f;
    for(int i = 0; i < nbParticles; ++i){

        xSum += bodyParticles[i].pos.x;
        ySum += bodyParticles[i].pos.y;
        zSum += bodyParticles[i].pos.z;
    }

    xSum /= nbParticles;
    ySum /= nbParticles;
    zSum /= nbParticles;

    body->barycenter = Particle(vec3(xSum, ySum, zSum));

    // Add distance constraint between each triangles vertex and barycenter
    const Triangle* bodyTriangles = body->getTriangles();
    int nbTriangles = body->nbTriangles;

    for(int i = 0; i < nbTriangles; ++i){

        Triangle cur = bodyTriangles[i];
        solver.pushConstraint(new DistanceConstraint(&bodyParticles[cur[1]], &body->barycenter));
        solver.pushConstraint(new DistanceConstraint(&bodyParticles[cur[2]], &body->barycenter));
        solver.pushConstraint(new DistanceConstraint(&bodyParticles[cur[3]], &body->barycenter));
    }
}

void PhysiK::ParticleSystem::addSoftBody(PhysiK::Body *body)
{
    PhysiK::PhysicObject *temp = body;
    physicObjecs.push_back(temp);
}

void PhysiK::ParticleSystem::addParticleGroup(PhysiK::ParticleGroup *particle)
{
    /*::PhysicObject *temp = particle;
    physicObjecs.push_back(temp);*/
}

void PhysiK::ParticleSystem::genIntersectionConstraint()
{

}

void PhysiK::ParticleSystem::nextSimulationStep(float deltaT)
{

}
