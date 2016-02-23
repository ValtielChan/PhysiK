#include "particlemesh.h"
#include <glm/ext.hpp>
#include <SparrowRenderer/phongmaterial.h>
#include <SparrowRenderer/sparrowrenderer.h>
#include <SparrowRenderer/glassert.h>

#include <chrono>

ParticleMesh::ParticleMesh(ParticleProperties properties, const glm::vec3* positions) :
    Sphere(NULL, 4, properties.radius),
    group(properties.amount, (float*)positions, sizeof(glm::vec3), properties.radius, properties.mass, properties.isKinematic)
{
    PhongMaterial *mat = new PhongMaterial();
    mat->diffuse = glm::vec3(properties.r, properties.g, properties.b);
    mat->ambient = mat->diffuse/5;
    mat->specular = mat->diffuse;
    mat->shininess = 20;
    setMaterial(mat);

    texCoords.clear();

    while(instances_offsets.size() < properties.amount)
        instances_offsets.push_back(glm::vec3(0));

    updatePositions();
}

void ParticleMesh::updatePositions()
{
    PhysiK::Particle *particles = group.getPositions();
    if(SparrowRenderer::isModernOpenGLAvailable() && vao != 0)
    {
        glm::vec3 * ptr = beginUpdateBuffer(Mesh::INSTANCE_BUFFER);
#pragma omp parallel for
        for(std::size_t i=0; i<instances_offsets.size(); ++i)
        {
            ptr[i].x = particles[i].pos.x;
            ptr[i].y = particles[i].pos.y;
            ptr[i].z = particles[i].pos.z;
        }
        endUpdateBuffer();
    }
    else
    {
#pragma omp parallel for
        for(std::size_t i=0; i<instances_offsets.size(); ++i)
        {
            instances_offsets[i].x = particles[i].pos.x;
            instances_offsets[i].y = particles[i].pos.y;
            instances_offsets[i].z = particles[i].pos.z;
        }
    }
}

PhysiK::ParticleGroup* ParticleMesh::getParticleGroup()
{
    return &group;
}

void ParticleMesh::applyOffset(int particleId, glm::vec3 moveVec)
{
    PhysiK::Particle *particles = group.getPositions();
    particles[particleId].pos.x += moveVec.x;
    particles[particleId].pos.y += moveVec.y;
    particles[particleId].pos.z += moveVec.z;
    PhysiK::vec3 *oldPos = group.getOldPositions();
    oldPos[particleId].x += moveVec.x;
    oldPos[particleId].y += moveVec.y;
    oldPos[particleId].z += moveVec.z;
}
