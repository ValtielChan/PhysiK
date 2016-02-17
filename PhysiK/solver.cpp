#include "solver.h"
#include "particle.h"
#include "constraint.h"
#include <cassert>
void PhysiK::Solver::solve(unsigned int nbIterations){

	//half for constraint, half for temporaryConstraints
	for(unsigned int i = 0 ; i < nbIterations * 2 ; i++){

		std::vector<Constraint *>& constraintToPerform = i%2 == 0 ? constraints:temporaryConstraints;

		for(Constraint * constraint : constraintToPerform){

			std::vector<vec3> displacement;

			float lambda = constraint->lambda();

			for(Particle * position: constraint->positions)
				displacement.push_back(constraint->grad(&(position->pos)) * - lambda * position->omega);

			for(std::size_t j = 0 ; j < constraint->positions.size() ; j++)
				constraint->positions[j]->pos+=displacement[j];

			constraint->update();

			displacement.clear();

		}
	}
}

void PhysiK::Solver::pushConstraint(Constraint *constraint){
	constraints.push_back(constraint);
}

void PhysiK::Solver::pushTemporaryConstraint(Constraint *constraint){
	temporaryConstraints.push_back(constraint);
}

void PhysiK::Solver::clearTemporaryConstraint(){
	for(Constraint * contrainte: temporaryConstraints)
		delete contrainte;
	temporaryConstraints.clear();
}

void PhysiK::Solver::clearConstraints()
{
    constraints.clear();
}
