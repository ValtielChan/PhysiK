#include "bodymesh.h"
#include <SparrowRenderer/mesh.h>
#include <SparrowRenderer/sparrowrenderer.h>

BodyMesh::BodyMesh(Mesh* myMesh, BodyProperties myProperties) :
    properties(myProperties),
    body(myMesh->positions.size(), myMesh->indices.size(), myProperties.isKinematic)
{
    mesh = myMesh;
}

void BodyMesh::update()
{
    if(!properties.isKinematic)
    {
        if(properties.isRigid)
            updateTransform();
        else
            updatePositions();
    }
}

void BodyMesh::updateTransform()
{
    // TODO compute modelMatrix from particle positions
}

void BodyMesh::updatePositions()
{
    PhysiK::Particle *particles = body.getPositions();
    if(mesh->vao)
    {
        glm::vec3 * ptr = mesh->beginUpdateBuffer(Mesh::POSITION_BUFFER);
#pragma omp parallel for
        for(std::size_t i=0; i<mesh->positions.size(); ++i)
        {
            ptr[i].x = particles[i].pos.x;
            ptr[i].y = particles[i].pos.y;
            ptr[i].z = particles[i].pos.z;
        }
        mesh->endUpdateBuffer();
    }
    mesh->computeNormals();
}

PhysiK::Body* BodyMesh::getBody()
{
    return &body;
}
