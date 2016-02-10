#include "constraint.h"
#include "particle.h"
#include <math.h>

PhysiK::vec3 PhysiK::Constraint::grad(const vec3 *input){
	const float epsilon = 0.1f;
	vec3 gradient;
	vec3 vertex = *input;
	float baseC = eval();

	vertex.x += epsilon;
	gradient.x = eval() - baseC;
	vertex.x -= epsilon;

	vertex.y += epsilon;
	gradient.y = eval() - baseC;
	vertex.y -= epsilon;

	vertex.z += epsilon;
	gradient.z = eval() - baseC;
	vertex.z -= epsilon;

	return gradient/epsilon;
}

float PhysiK::Constraint::lambda(){
	float sum = 0.0f;
	for(Particle * vertex : positions){
		float norm = grad(&(vertex->pos)).length();
		sum += vertex->omega*norm*norm;
	}
	return eval()/sum;
}

PhysiK::DistanceConstraint::DistanceConstraint(Particle *pos1, Particle *pos2, float dst):dst(dst){
	min=dst!=0;
	if(!min)
		dst=(pos1->pos-pos2->pos).length();
	positions.push_back(pos1);
	positions.push_back(pos2);
}

float PhysiK::DistanceConstraint::eval(){
	float curdst = (positions[0]->pos-positions[0]->pos).length();
	return min?std::min(0.f,curdst-dst):curdst;
}

PhysiK::CollisionConstraint::CollisionConstraint(Particle *pos, vec3 normal, float delta):normal(normal),delta(delta){
	positions.push_back(pos);
}

PhysiK::CollisionConstraint::CollisionConstraint(Particle *pos, vec3 pt1, vec3 pt2, vec3 pt3){
	positions.push_back(pos);
	vec3 vec1 = pt2-pt1;
	vec3 vec2 = pt3-pt1;
	normal=vec1.cross(vec2);
	delta=
			 normal.x*pt1.x
			+normal.y*pt1.y
			+normal.z*pt1.z;
}

float PhysiK::CollisionConstraint::eval(){
	Particle * pos = positions[0];
	return std::min(
				0.f,
				 normal.x*pos->pos.x
				+normal.y*pos->pos.y
				+normal.z*pos->pos.z
				-delta);
}

/*PhysiK::VolumeConstraint::VolumeConstraint(vec3 D){
	center=D;
}

Physik::VolumeConstraint::volume(vec3 p1, vec3 p2, vec3 p3, vec3 p4){
	vec3 v1 = p1 - p2;
	vec3 v2 = p1 - p3;
	vec3 v3 = p1 - p4;
	return v1.cross(v2).dot(v3)/6.f - volume;
}

float PhysiK::VolumeConstraint::eval(){
}*/
