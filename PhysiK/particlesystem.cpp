#include "particlesystem.h"


void PhysiK::ParticleSystem::addRigidBody(PhysiK::Body *body)
{
    PhysiK::PhysicObject *temp = body;
    physicObjecs.push_back(temp);

    // Constraint building
    // Set of distance constraints for RigidBody

    // Temp
    // On pourrait le faire directement dans le constructeur des PhysicObject
    body->computeBarycenter();

    // Add distance constraint between each triangles vertex and barycenter
    Particle* bodyParticles = body->getPositions();
    const Triangle* bodyTriangles = body->getTriangles();
    int nbTriangles = body->nbTriangles;

    for(int i = 0; i < nbTriangles; ++i){

        Triangle cur = bodyTriangles[i];
        solver.pushConstraint(new DistanceConstraint(&bodyParticles[cur[1]], &body->barycenter));
        solver.pushConstraint(new DistanceConstraint(&bodyParticles[cur[2]], &body->barycenter));
        solver.pushConstraint(new DistanceConstraint(&bodyParticles[cur[3]], &body->barycenter));

        //Simple collision constraints
        solver.pushConstraint(new CollisionConstraint(&bodyParticles[cur[1]], vec3(0.f, 0.f, 1.f), 0.f));
        solver.pushConstraint(new CollisionConstraint(&bodyParticles[cur[2]], vec3(0.f, 0.f, 1.f), 0.f));
        solver.pushConstraint(new CollisionConstraint(&bodyParticles[cur[3]], vec3(0.f, 0.f, 1.f), 0.f));
    }
}

void PhysiK::ParticleSystem::addSoftBody(PhysiK::Body *body)
{
    PhysiK::PhysicObject *temp = body;
    physicObjecs.push_back(temp);

    // Wait for soft constraints
}

void PhysiK::ParticleSystem::addParticleGroup(PhysiK::ParticleGroup *particle)
{
    PhysiK::PhysicObject *temp = particle;
    physicObjecs.push_back(temp);

    // Only basic collision constraints for now
    Particle* bodyParticles = particle->getPositions();
    int nbParticles = particle->nbParticles;

    for(int i = 0; i < nbParticles; ++i)
        solver.pushConstraint(new CollisionConstraint(&bodyParticles[i], vec3(0.f, 0.f, 1.f), 0.f));

}

void PhysiK::ParticleSystem::genIntersectionConstraint()
{

}

void PhysiK::ParticleSystem::nextSimulationStep(float deltaT)
{

}
