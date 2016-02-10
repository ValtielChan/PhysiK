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

        for(int j=0 ;  j<3 ; j++){
            //piramide constraint
            solver.pushConstraint(new DistanceConstraint (&bodyParticles[cur[j]], &body->barycenter));
            //trianlge constraint
            solver.pushConstraint(new DistanceConstraint (&bodyParticles[cur[j]], &bodyParticles[cur[(j+1)%3]]));
            //default constraint
            solver.pushConstraint(new CollisionConstraint(&bodyParticles[cur[j]], vec3(0.f, 0.f, 1.f), 0.f));
        }

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
