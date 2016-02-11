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

    // Temporary plane constraint to keep the particles from falling
    // TODO generate them only when they collide with the plane
    Particle* bodyParticles = particle->getPositions();

    for(int i = 0; i < particle->nbParticles; ++i)
        solver.pushConstraint(new CollisionConstraint(&bodyParticles[i], vec3(0.f, 0.f, 1.f), 0.f));

}

void PhysiK::ParticleSystem::genIntersectionConstraints()
{

	PHT.clear();
	for(PhysicObject * object : physicObjecs)
		if(ParticleGroup * particles = dynamic_cast<ParticleGroup *>(object))
			PHT.addObject(particles);

	/*for(PhysicObject * object : physicObjecs)
		if(Body * body = dynamic_cast<Body *>(object))
			THT.addObject(body);*/

    // find particle to particle intersections

    // Need to store intersection for velocityUpdate
    ptpIntersections.clear();
    PHT.generateIntersection(ptpIntersections);

	for(IntersectionParticleParticle& intersection: ptpIntersections)
		solver.pushTemporaryConstraint(intersection.getConstraint());

    // find particle to plane intersections


    // MILESTONE 2 : find particle to triangle intersections
}

void PhysiK::ParticleSystem::velocityUpdate()
{
    // for each intersection, generate a collision impulse
    // glm::vec3 v2 = -2 * glm::dot(v1, n) * n + v1;
    for (IntersectionParticleParticle inter : ptpIntersections) {
        Particle* p1 = inter.getParticle1();
        Particle* p2 = inter.getParticle2();

        vec3 n = p1->pos - p2->pos;
        n = n.normalize();

        vec3 impulseP1 = (n * p1->velocity.dot(n)) * -2.0f + p1->velocity;
        vec3 impulseP2 = (n.opposite() * p1->velocity.dot(n.opposite())) * -2.0f + p1->velocity;

        // To change later (no wanted behaviour)
        p1->velocity += impulseP1;
        p1->velocity += impulseP2;
    }
}

void PhysiK::ParticleSystem::nextSimulationStep(float deltaT)
{
    // d'après le papier PBD

    for(PhysicObject* po : physicObjecs)
        po->preUpdate(deltaT, gravity , damping);

    genIntersectionConstraints();

    solver.solve(nbIterations);

    solver.clearTemporaryConstraint();

    for(PhysicObject* po : physicObjecs)
        po->postUpdate(deltaT);

    velocityUpdate();
}

void PhysiK::ParticleSystem::reset()
{
    solver.clearConstraints();
    solver.clearTemporaryConstraint();

    physicObjecs.clear();
    ptpIntersections.clear();

    THT.clear();
    PHT.clear();
}
