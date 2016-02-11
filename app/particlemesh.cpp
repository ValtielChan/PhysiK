#include "particlemesh.h"
#include <glm/ext.hpp>
#include <SparrowRenderer/phongmaterial.h>

ParticleMesh::ParticleMesh(ParticleProperties properties, const glm::vec3* positions) :
    Sphere(NULL, 1, properties.radius),
    group(properties.amount, (float*)positions, sizeof(glm::vec3), properties.radius, properties.mass)
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
    for(int i=0; i<instances_offsets.size(); ++i)
    {
        instances_offsets[i].x = particles[i].pos.x;
        instances_offsets[i].y = particles[i].pos.y;
        instances_offsets[i].z = particles[i].pos.z;
    }
}

PhysiK::ParticleGroup* ParticleMesh::getParticleGroup()
{
    return &group;
}
