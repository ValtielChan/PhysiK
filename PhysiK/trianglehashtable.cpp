#include "trianglehashtable.h"
#include "physicobject.h"
#include "particlegroup.h"
#include "intersection.h"

//this code is provided without any kind of waranty or purpose

void PhysiK::ParticleHashTable::addObject(ParticleGroup * group){
	for(unsigned int i = 0 ; i<group->nbParticles ;i++){
		for(float dx = 0 ; dx < group->radius ; dx+=vec3::VortexSize){
			for(float dy = 0 ; dy < group->radius ; dy+=vec3::VortexSize){
				for(float dz = 0 ; dz < group->radius ; dz+=vec3::VortexSize){
					vec3 p = group->getPositions()[i].pos;
					vec3 pos = vec3(p.x+dx,p.y+dy,p.z+dz);
					Vortex[pos.toVortex()].push_back(std::make_pair(group,i));
				}
			}
		}
	}
}

void PhysiK::ParticleHashTable::generateIntersection(std::vector<IntersectionParticuleParticule> &intersections){
	for(auto& plop : Vortex){
		m_vector& test = plop.second;
		for(m_pair& pair1 : test){
			for(m_pair& pair2 : test){
				Particle& particule2 = pair2.first->getPositions()[pair2.second];
				IntersectionParticuleParticule to_test(pair1.first,pair1.second,&particule2,pair2.first->radius);
				if(to_test.intersect())
					intersections.push_back(to_test);
			}
		}
	}
}
