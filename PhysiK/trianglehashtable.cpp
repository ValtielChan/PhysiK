#include "trianglehashtable.h"
#include "physicobject.h"
#include "particlegroup.h"
#include "body.h"
#include "intersection.h"
#include "triangle.h"
#include "constraint.h"
#include <math.h>

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
					intersections.push_back(to_test);
			}
		}
	}
}

void PhysiK::TriangleHashTable::addObject(Body *body){
	for(unsigned int triangleOffset = 0 ; triangleOffset<body->nbTriangles ; triangleOffset++){
		Particle * particules[3];
		for(int j = 0 ; j < 3 ; j++)
			particules[j] = body->getPositions()+body->getTriangles()[triangleOffset][j];
		vec3 min;
		vec3 max;

		//on cherche le minimum et le maximum
		for(int j = 0 ; j < 3 ; j++){
			min[j] = std::min(std::min(particules[0]->pos[j],particules[1]->pos[j]),particules[2]->pos[j]);
			max[j] = std::max(std::max(particules[0]->pos[j],particules[1]->pos[j]),particules[2]->pos[j]);
		}

		vec3 min_voxel = min_voxel.toVoxel();
		vec3 max_voxel = min_voxel.toVoxel()+1;


		for(float x = min_voxel.x ; x < max_voxel.x ; x++){
			for(float y = min_voxel.y ; y < max_voxel.y ; y++){
				for(float z = min_voxel.z ; z < max_voxel.z ; z++){
					vec3 voxel(x,y,z);
					Particle par((voxel+0.5)*vec3::voxelSize);
					//aproximate the cube with a particule
					CollisionParticuleTriangleConstraint to_test(&par,particules[0],particules[1],particules[2]);
					if(to_test.eval()<vec3::voxelSize*sqrt(2.f)/2.f){
						voxelGrid[voxel].push_back(std::make_pair(body,triangleOffset));
					}
				}
			}
		}
	}
}

void PhysiK::TriangleHashTable::generateIntersection(std::vector<IntersectionParticleTriangle> &intersections){
	for(auto& iterator : voxelGrid){
		m_vector& test = iterator.second;
		const vec3& voxel = iterator.first;
		for(unsigned int i = 0 ; i < test.size();i++){
			m_pair& pair1= test[i];
			for(unsigned int j = 0 ; j < test.size();j++){
				m_pair& pair2= test[j];
				for(int vertexOffset = 0 ; vertexOffset < 3 ; vertexOffset++){
					int particuleOffset = pair2.first->getTriangles()[pair2.second][vertexOffset];
					Particle * particle = pair2.first->getPositions()+particuleOffset;
					//warning : consider triangle only in clock wise order
					IntersectionParticleTriangle to_test(pair1.first,pair1.second,particle);
					vec3 oldPosition = pair2.first->getOldPositions()[particuleOffset];
					if(to_test.intersect(particle->pos.toVoxel()==voxel)&&to_test.intersect(oldPosition))
						intersections.push_back(to_test);
				}
			}
		}
	}
}

void PhysiK::ParticleHashTable::generateIntersectionWithTriangles(std::vector<IntersectionParticleTriangle> &intersections, TriangleHashTable &particlesHashtable){
	for(auto& iterator : voxelGrid){
		m_vector& test = iterator.second;
		const vec3& voxel = iterator.first;
		for(const m_pair& pair1 : test){
			Particle * particle = pair1.first->getPositions()+pair1.second;
			if(particle->pos.toVoxel()==voxel){
				auto second_iterator = particlesHashtable.voxelGrid.find(voxel);
				if(second_iterator!=particlesHashtable.voxelGrid.end()){
					TriangleHashTable::m_vector& triangles = second_iterator->second;
					for(TriangleHashTable::m_pair& pair2 : triangles){
						IntersectionParticleTriangle to_test(pair2.first,pair2.second,particle);
						if(to_test.intersect()){
							intersections.push_back(to_test);
						}
					}
				}
			}
		}
	}
}
