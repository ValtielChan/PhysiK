#include "bodymesh.h"
#include <SparrowRenderer/mesh.h>
#include <SparrowRenderer/sparrowrenderer.h>
#include <cstring>
//#include <eigen3/Eigen/Eigen>

BodyMesh::BodyMesh(Mesh* myMesh, BodyProperties myProperties) :
    properties(myProperties),
    body(myMesh->positions.size(), myMesh->indices.size(), myProperties.mass, myProperties.isKinematic)
{
    mesh = myMesh;

    PhysiK::vec3 *oldPos = body.getOldPositions();
    std::memcpy(oldPos, mesh->positions.data(), sizeof(glm::vec3)*mesh->positions.size());
    PhysiK::Triangle *tri = body.getTriangles();
    std::memcpy(tri, mesh->indices.data(), sizeof(unsigned int)*mesh->indices.size());
    PhysiK::Particle *pos = body.getPositions();
    for(int i=0; i<mesh->positions.size(); ++i)
    {
        pos[i].pos.x = mesh->positions[i].x;
        pos[i].pos.y = mesh->positions[i].y;
        pos[i].pos.z = mesh->positions[i].z;
    }
}

void BodyMesh::update()
{
    if(!properties.isKinematic)
    {
        if(properties.type == BodyProperties::RIGID)
            updateTransform();
        else
            updatePositions();
    }
}

void BodyMesh::updateTransform()
{
    PhysiK::vec3 center;
    body.computeBarycenter();

    PhysiK::vec3 translation = center - body.getBarycenter();

    // TODO compute rotation

    //Eigen::Dynamic
    /*Eigen::Matrix<float,16,16> A;
    Eigen::Matrix<float,16,1> X;
    Eigen::Matrix<float,16,1> B;

    PhysiK::vec3 points[4];

    for(int i = 0 ; i < 4 ; i++){
        for(int j = 0 ; j < 4 ; j++){
            for(int k = 0 ; k < 4 ; k++){
                //A(j*4+k,1)=points[i+j];
            }
        }
    }*/
    // TODO apply translation and rotation to modelMatrix
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

void BodyMesh::applyOffset(glm::vec3 moveVec)
{
    PhysiK::Particle *particles = body.getPositions();
    PhysiK::vec3 *oldPos = body.getOldPositions();
#pragma omp parallel for
    for(std::size_t i=0; i<mesh->positions.size(); ++i)
    {
        particles[i].pos.x += moveVec.x;
        particles[i].pos.y += moveVec.y;
        particles[i].pos.z += moveVec.z;
        oldPos[i].x += moveVec.x;
        oldPos[i].y += moveVec.y;
        oldPos[i].z += moveVec.z;
    }
}
