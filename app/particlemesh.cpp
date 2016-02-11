#include "particlemesh.h"
#include <glm/ext.hpp>
#include <SparrowRenderer/phongmaterial.h>

ParticleMesh::ParticleMesh(ParticleProperties properties, const glm::vec3* positions) :
    Sphere(NULL, 1, properties.radius),
    group(properties.amount, (float*)positions, 0, properties.radius, properties.mass)
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
}

void ParticleMesh::updatePositions()
{
    for(glm::vec3 &pos : instances_offsets)
    {
        pos.x = group.getPositions()->pos.x;
        pos.y = group.getPositions()->pos.y;
        pos.z = group.getPositions()->pos.z;
    }
}

PhysiK::ParticleGroup* ParticleMesh::getParticleGroup()
{
    return &group;
}
