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

void PhysiK::ParticleSystem::addUnmovableObject(PhysiK::Body * body){
	for(unsigned int i = 0 ; i < body->nbParticles ; i++){
		body->getPositions()[i].omega=0;
		body->getOldPositions()[i]=body->getPositions()[i].pos;
	}
	physicObjecs.push_back(body);
}

void PhysiK::ParticleSystem::addRigidBody(PhysiK::Body *body)
{
	std::cout<<body->mass<<std::endl;
	toParticles(body);
	physicObjecs.push_back(body);

	for(unsigned int i = 0 ; i<body->nbParticles ; i++){
		for(unsigned int j = i+1 ; j < body->nbParticles ; j++){
			solver.pushConstraint(new DistanceConstraint(body->getPositions()+i,body->getPositions()+j));
		}
	}
}

void PhysiK::ParticleSystem::addCloth(Body *body){
	physicObjecs.push_back(body);
	for(unsigned int i = 0; i<body->nbTriangles;i++){
		Particle * p1 = body->getPositions()+body->getTriangles()[i][0];
		Particle * p2 = body->getPositions()+body->getTriangles()[i][1];
		Particle * p3 = body->getPositions()+body->getTriangles()[i][2];
		solver.pushConstraint(new DistanceConstraint(p1,p2));
		solver.pushConstraint(new DistanceConstraint(p2,p3));
		solver.pushConstraint(new DistanceConstraint(p3,p1));
	}
}

void PhysiK::ParticleSystem::toParticles(Body *body){
	addParticleGroup(body->getParticlesGroup());
}

void PhysiK::ParticleSystem::addSoftBody(PhysiK::Body *body)
{

	addCloth(body);
	toParticles(body);
	physicObjecs.push_back(body);

	VolumeConstraint * volume = new VolumeConstraint();

	for(unsigned int i = 0; i<body->nbTriangles;i++){
		volume->addVolume(
				body->getPositions(),
				body->getPositions()+body->getTriangles()[i][0],
				body->getPositions()+body->getTriangles()[i][1],
				body->getPositions()+body->getTriangles()[i][2]
		);
	}

	solver.pushConstraint(volume);

}

void PhysiK::ParticleSystem::addParticleGroup(PhysiK::ParticleGroup *particleGroup)
{
	physicObjecs.push_back(particleGroup);

#if 0 //obsolete

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

	for(PhysicObject* po : physicObjecs)
		if(ParticleGroup * particles = dynamic_cast<ParticleGroup *>(po))
			particles->preUpdate(deltaT, gravity, damping);

    genIntersectionConstraints();

    solver.solve(nbIterations);

    solver.clearTemporaryConstraint();

    velocityUpdate(deltaT);

	for(PhysicObject* po : physicObjecs)
		if(ParticleGroup * particles = dynamic_cast<ParticleGroup *>(po))
			particles->postUpdate(deltaT);


}

void PhysiK::ParticleSystem::addCube(){

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

	//		   7-------4
	//		  /|      /|
	//		 / |     / |
	//		6--|----5  |
	//		|  3----|--0
	//		|  /    | /
	//		| /     |/
	//		2-------1

	//bottom
	myBody->getTriangles()[0] = Triangle(0,1,2);
	myBody->getTriangles()[1] = Triangle(0,2,3);

	//top
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

	addUnmovableObject(myBody);

}

void PhysiK::ParticleSystem::reset()
{
    solver.clearConstraints();
    solver.clearTemporaryConstraint();

	physicObjecs.clear();

	for(PhysicObject * to_clear : physicObjecs)
		delete to_clear;
	ptpIntersections.clear();

	THT.clear();
    PHT.clear();
}
