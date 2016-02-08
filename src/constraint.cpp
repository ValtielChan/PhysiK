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
	for(Particule * vertex : positions){
		float norm = grad(&(vertex->pos)).length();
		sum += vertex->omega*norm*norm;
	}
	return eval()/sum;
}

PhysiK::DistanceConstraint::DistanceConstraint(Particule *pos1, Particule *pos2){
	positions.push_back(pos1);
	positions.push_back(pos2);
}

float PhysiK::DistanceConstraint::eval(){
	return (positions[0]->pos-positions[0]->pos).length();
}
