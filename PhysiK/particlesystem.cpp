#include "particlesystem.h"

#include "body.h"
#include "particlegroup.h"
#include "constraint.h"
#include "triangle.h"
#include "intersection.h"

PhysiK::ParticleSystem::ParticleSystem() :
	gravity(DEFAULT_GRAVITY),
	damping(DEFAULT_DAMPING),
	nbIterations(DEFAULT_NB_ITERATIONS){

	reset();
}

void PhysiK::ParticleSystem::addRigidBody(PhysiK::Body *body)
{
    std::cout<<"houray"<<std::endl;
    PhysiK::PhysicObject *temp = body;
    physicObjecs.push_back(body);
    return;

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

void PhysiK::ParticleSystem::addParticleGroup(PhysiK::ParticleGroup *particleGroup)
{
    PhysiK::PhysicObject *temp = particleGroup;
    physicObjecs.push_back(temp);

    // Temporary plane constraint to keep the particles from falling
    // TODO generate them only when they collide with the plane
    Particle* bodyParticles = particleGroup->getPositions();

#if 0 //for rigid body
    for(unsigned int i = 0; i<particle->nbParticles;i++){
        for(unsigned int j = 0; j<particle->nbParticles;j++){
            solver.pushConstraint(new DistanceConstraint(&bodyParticles[i],&bodyParticles[j]));
        }
    }

    for(unsigned int i = 0; i < particleGroup->nbParticles; ++i){
        float radius = particleGroup->radius;
        const float box_size = 20-(radius*2);

        if (!particleGroup->isKinematic) {

            solver.pushConstraint(new CollisionConstraint(&bodyParticles[i], vec3(0.f,  1.f, 0.f),  radius));
            solver.pushConstraint(new CollisionConstraint(&bodyParticles[i], vec3(0.f, -1.f, 0.f), -box_size));
            solver.pushConstraint(new CollisionConstraint(&bodyParticles[i], vec3( 0.f, 0.f, -1.f), -box_size/2.f));
            solver.pushConstraint(new CollisionConstraint(&bodyParticles[i], vec3( 0.f, 0.f,  1.f), -box_size/2.f));
            solver.pushConstraint(new CollisionConstraint(&bodyParticles[i], vec3( 1.f, 0.f,  0.f), -box_size/2.f));
            solver.pushConstraint(new CollisionConstraint(&bodyParticles[i], vec3(-1.f, 0.f,  0.f), -box_size/2.f));
        }
    }

#endif
}

void PhysiK::ParticleSystem::genIntersectionConstraints()
{

	PHT.clear();
	for(PhysicObject * object : physicObjecs)
		if(ParticleGroup * particles = dynamic_cast<ParticleGroup *>(object))
			PHT.addObject(particles);

    // find particle to particle intersections
    // Need to store intersection for velocityUpdate
    ptpIntersections.clear();
    PHT.generateIntersection(ptpIntersections);

	for(IntersectionParticleParticle& intersection : ptpIntersections)
		solver.pushTemporaryConstraint(intersection.getConstraint());


	THT.clear();
	for(PhysicObject * object : physicObjecs)
		if(Body * body = dynamic_cast<Body *>(object))
			THT.addObject(body);

	pttIntersections.clear();
	PHT.generateIntersectionWithTriangles(pttIntersections,THT);

	for(IntersectionParticleTriangle& intersection : pttIntersections)
		solver.pushTemporaryConstraint(intersection.getConstraint());

}

void PhysiK::ParticleSystem::velocityUpdate(float deltaT)
{
    // for each intersection, generate a collision impulse
    // glm::vec3 v2 = -2 * glm::dot(v1, n) * n + v1;

    for (IntersectionParticleParticle& inter : ptpIntersections) {

        // To change later (no wanted behaviour)
        inter.getParticle1()->impulsion += inter.getWorks2(deltaT);
        inter.getParticle2()->impulsion += inter.getWorks1(deltaT);
    }

}

void PhysiK::ParticleSystem::nextSimulationStep(float deltaT)
{
    // integrator

    for(PhysicObject* po : physicObjecs) {
        //po->getPositions()[0].velocity.print();
        po->preUpdate(deltaT, gravity, damping);
    }

    genIntersectionConstraints();

    solver.solve(nbIterations);

    solver.clearTemporaryConstraint();

    velocityUpdate(deltaT);

    for(PhysicObject* po : physicObjecs)
        po->postUpdate(deltaT);


}

void PhysiK::ParticleSystem::reset()
{
    solver.clearConstraints();
    solver.clearTemporaryConstraint();

    physicObjecs.clear();
    ptpIntersections.clear();

    Body * myBody = new Body(8, 12, true);

    //reversed inside out cube

	myBody->getPositions()[0].pos = vec3( 10, 0, 10);
	myBody->getPositions()[1].pos = vec3( 10, 0,-10);
	myBody->getPositions()[2].pos = vec3(-10, 0,-10);
	myBody->getPositions()[3].pos = vec3(-10, 0, 10);

	myBody->getPositions()[4].pos = vec3( 10,20, 10);
	myBody->getPositions()[5].pos = vec3( 10,20,-10);
	myBody->getPositions()[6].pos = vec3(-10,20,-10);
	myBody->getPositions()[7].pos = vec3(-10,20, 10);

    for(int i = 0; i < 8 ; i++){
        myBody->getPositions()[i].omega=0;
        myBody->getOldPositions()[i]=myBody->getPositions()[i].pos;
    }
    //		   7-------4
	//		  /|      /|
	//		 / |     / |
	//		6--|----5  |
	//		|  3----|--0
	//		|  /    | /
	//		| /     |/
	//		2-------1

	//top
	myBody->getTriangles()[0] = Triangle(0,1,2);
	myBody->getTriangles()[1] = Triangle(0,2,3);

	//bottom
	myBody->getTriangles()[2] = Triangle(6,5,4);
	myBody->getTriangles()[3] = Triangle(6,4,7);

	//front
	myBody->getTriangles()[4] = Triangle(2,1,5);
	myBody->getTriangles()[5] = Triangle(2,5,6);

	//back
	myBody->getTriangles()[6] = Triangle(4,0,3);
	myBody->getTriangles()[7] = Triangle(4,3,7);

	//right
	myBody->getTriangles()[8] = Triangle(1,0,4);
	myBody->getTriangles()[9] = Triangle(1,4,5);

	//left
	myBody->getTriangles()[10] = Triangle(7,3,2);
	myBody->getTriangles()[11] = Triangle(7,2,6);

	//todo : sleep
	addRigidBody(myBody);


    THT.clear();
    PHT.clear();
}
