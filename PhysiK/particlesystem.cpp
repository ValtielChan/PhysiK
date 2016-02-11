#include "particlesystem.h"

#include "body.h"
#include "particlegroup.h"
#include "constraint.h"
#include "triangle.h"
#include "intersection.h"

void PhysiK::ParticleSystem::addRigidBody(PhysiK::Body *body)
{
    PhysiK::PhysicObject *temp = body;
    physicObjecs.push_back(temp);

    // Constraint building

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
			solver.pushConstraint(new DistanceConstraint (&bodyParticles[cur[j]], &bodyParticles[0]));
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

	body->computeBarycenter();
    // Wait for soft constraints
    //solver.pushConstraint(new VolumeConstraint (body->barycenter));

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

void PhysiK::ParticleSystem::genIntersectionConstraints()
{

	PHT.clear();
	for(PhysicObject * object : physicObjecs)
		if(ParticleGroup * particules = dynamic_cast<ParticleGroup *>(object))
			PHT.addObject(particules);

	/*for(PhysicObject * object : physicObjecs)
		if(Body * body = dynamic_cast<Body *>(object))
			THT.addObject(body);*/

    // find particle to particle intersections

    std::vector<IntersectionParticuleParticule> intersections;
    PHT.generateIntersection(intersections);

    for(IntersectionParticuleParticule& intersection: intersections)
		solver.pushTemporaryConstraint(intersection.getConstraint());

    // find particle to plane intersections

    // MILESTONE 2 : find particle to triangle intersections
}

void PhysiK::ParticleSystem::velocityUpdate()
{
    // Apply gravity
    for (PhysicObject *po : physicObjecs) {

        vec3 *velocities = po->getVelocities();

        for (int i = 0; i < po->nbParticles; i++)
            velocities[i] += vec3(0.0f, -gravity, 0.0f);
    }

    // for each intersection, generate a collision impulse
}

void PhysiK::ParticleSystem::nextSimulationStep(float deltaT)
{
    // d'après le papier PBD

    for(PhysicObject* po : physicObjecs)
        po->preUpdate(deltaT, gravity , damping);

    genIntersectionConstraints();

    solver.solve(nbIterations);

	solver.clearTemporaryConstriant();

    for(PhysicObject* po : physicObjecs)
        po->postUpdate(deltaT);

    velocityUpdate();
}
