#include "constraint.h"
#include "particle.h"
#include <math.h>

PhysiK::vec3 PhysiK::Constraint::grad(vec3 *input){
	const float epsilon = 0.001f;
	vec3 gradient;
	float baseC = eval();

	input->x += epsilon;
	gradient.x = eval() - baseC;
	input->x -= epsilon;

	input->y += epsilon;
	gradient.y = eval() - baseC;
	input->y -= epsilon;

	input->z += epsilon;
	gradient.z = eval() - baseC;
	input->z -= epsilon;

	return gradient/epsilon;
}

float PhysiK::Constraint::lambda(){
	float sum = 0.0f;
	for(Particle * vertex : positions)
		sum+=grad(&(vertex->pos)).lengthSquared()*vertex->omega;
	float res = eval();
	return sum==0?0:res/sum;
}

PhysiK::CollisionParticuleTriangleConstraint::CollisionParticuleTriangleConstraint(Particle *pos, Particle *pt1, Particle *pt2, Particle *pt3, float size):size(size){
	positions.push_back(pos);
	positions.push_back(pt1);
	positions.push_back(pt2);
	positions.push_back(pt3);
}

float PhysiK::CollisionParticuleTriangleConstraint::eval() const{
	const vec3& pos = positions[0]->pos;

	const vec3& pt1 = positions[1]->pos;
	const vec3& pt2 = positions[2]->pos;
	const vec3& pt3 = positions[3]->pos;

	vec3 vec1 = pt2-pt1;
	vec3 vec2 = pt3-pt1;
	vec3 normal=vec1.cross(vec2).normalize();
	float delta=normal.dot(pt1);
	return std::min(0.f,(normal.dot(pos)-delta)-size);
}

PhysiK::CollisionConstraint::CollisionConstraint(Particle *particle, vec3 normal, float delta):normal(normal),delta(delta){
    positions.push_back(particle);
}

float PhysiK::CollisionConstraint::eval() const{
	const Particle * pos = positions[0];
	return std::min(0.f,normal.dot(pos->pos)-delta);
}

PhysiK::DistanceConstraint::DistanceConstraint(Particle *pos1, Particle *pos2){
	dst=(pos1->pos-pos2->pos).length();
	positions.push_back(pos1);
	positions.push_back(pos2);
}

float PhysiK::DistanceConstraint::eval() const{
	return (positions[0]->pos-positions[1]->pos).length()-dst;
}

PhysiK::MinDistanceConstraint::MinDistanceConstraint(Particle *pos1, Particle *pos2, float _dst):dst(_dst){
	positions.push_back(pos1);
	positions.push_back(pos2);
}

float PhysiK::MinDistanceConstraint::eval() const{
	return std::min(0.f,(positions[0]->pos-positions[1]->pos).length()-dst);
}


void PhysiK::VolumeConstraint::addVolume(Particle *p1, Particle *p2, Particle *p3, Particle *p4){
	positions.push_back(p1);
	positions.push_back(p2);
	positions.push_back(p3);
	positions.push_back(p4);
}

float PhysiK::VolumeConstraint::volume(vec3 p1, vec3 p2, vec3 p3, vec3 p4) const{

	vec3 v1 = p1 - p2;
	vec3 v2 = p1 - p3;
	vec3 v3 = p1 - p4;

	return v1.cross(v2).dot(v3)/6.f;
}

float PhysiK::VolumeConstraint::eval() const{
    float total = 0;
    for(unsigned int i = 0 ; i < positions.size() ; i+=4)
        total+=volume(positions[i]->pos,positions[i+1]->pos,positions[i+2]->pos,positions[i+3]->pos);
    return total-delta;
}
