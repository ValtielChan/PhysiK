#include "trianglehashtable.h"
#include "physicobject.h"
#include "particlegroup.h"
#include "intersection.h"

//this code is provided without any kind of waranty or purpose

void PhysiK::ParticleHashTable::addObject(ParticleGroup * group){
	float radius = group->radius;
	int diameterInVoxel = 2.f*radius/vec3::voxelSize;
	float uper_limit = float(diameterInVoxel+2)*vec3::voxelSize-radius;
	for(unsigned int i = 0 ; i<group->nbParticles ;i++){
		for(float dx = -radius ; dx < uper_limit ; dx+=vec3::voxelSize){
			for(float dy = -radius ; dy < uper_limit ; dy+=vec3::voxelSize){
				for(float dz = -radius ; dz < uper_limit ; dz+=vec3::voxelSize){
					vec3 center = group->getPositions()[i].pos;
					vec3 currentPosition = vec3(center.x+dx,center.y+dy,center.z+dz);
					voxelGrid[currentPosition.toVoxel()].push_back(std::make_pair(group,i));
				}
			}
		}
	}
}

void PhysiK::ParticleHashTable::generateIntersection(std::vector<IntersectionParticleParticle> &intersections){
	for(auto& iterator : voxelGrid){
		m_vector& test = iterator.second;
		const vec3& voxel = iterator.first;
		for(unsigned int i = 0 ; i < test.size();i++){
			m_pair& pair1= test[i];
			for(unsigned int j= i+1 ; j < test.size();j++){
				m_pair& pair2= test[j];
				IntersectionParticleParticle to_test(pair1.first,pair1.second,pair2.first,pair2.second);
				if(to_test.getParticle1()!=to_test.getParticle2()
						&&to_test.intersect()
						&&to_test.intersectPos().toVoxel()==voxel)//itersection in the voxel (avoid duplicate intersections)
					intersections.push_back(std::move(to_test));
			}
		}
	}
}
