#include "constraint.h"
#include "vec3.h"
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
	for(vec3 * vertex : positions){
		float norm = grad(vertex).length();
		sum += vertex->omega*norm*norm;
	}
	return eval()/sum;
}

PhysiK::DistanceConstraint::DistanceConstraint(vec3 *pos1, vec3 *pos2)
{
    positions.push_back(pos1);
    positions.push_back(pos2);
}

float PhysiK::DistanceConstraint::eval()
{
    float xDiff = positions[0]->x - positions[1]->x;
    float yDiff = positions[0]->y - positions[1]->y;
    float zDiff = positions[0]->z - positions[1]->z;

    return sqrt(xDiff*xDiff + yDiff*yDiff + zDiff*zDiff);
}
