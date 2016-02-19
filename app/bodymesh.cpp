#include "bodymesh.h"
#include "PhysiK/triangle.h"
#include <SparrowRenderer/mesh.h>
#include <SparrowRenderer/sparrowrenderer.h>
#include <cstring>
#include <eigen3/Eigen/Eigen>
#include <glm/detail/func_matrix.hpp>

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
	body.computeBarycenter();
	for(int i = 0 ; i < 3 ; i++){
		iniPos[0][i]=body.barycenter[i];
		for(int j = 0 ; j < 3 ; j++){
			int offset = body.getTriangles()[0][i];
			iniPos[i+1][j]=body.getPositions()[offset].pos[j];
		}
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

    //Eigen::Dynamic
	Eigen::Matrix<float,16,16> A;
	Eigen::Matrix<float,16,1> X;
	Eigen::Matrix<float,16,1> B;

	for(int i = 0 ; i < 16 ; i++){
		for(int j = 0 ; j < 16 ; j++)
			A(i,j)=0;
		B(i,0)=0;
	}

	//maybe use more vertex.

	PhysiK::vec3 points[4] = {body.barycenter};
	PhysiK::vec3 res[4]={body.computeBarycenter()};

	for(int i = 0 ; i < 3 ; i++){
		for(int j = 0 ; j < 3 ; j++){
			int offset = body.getTriangles()[0][i];
			res[i+1][j]=body.getOldPositions()[offset][j];
			//points[i+1][j]=body.getPositions()[offset].pos[j];
		}
	}

	for(int i = 0 ; i < 4 ; i++){
		for(int j = 0 ; j < 3 ; j++){
			points[i][j]=iniPos[i][j];
		}
	}

	for(int i = 0 ; i < 4 ; i++)
		for(int j = 0 ; j < 4 ; j++){
			for(int k = 0 ; k < 4 ; k++)
				A(i*4+j,j*4+k)=k==3?1:points[i][k];
			B(i*4+j,0)=j==3?1:res[i][j];
		}


	X = A.fullPivHouseholderQr().solve(B);
	for(int i = 0 ; i < 4 ; i++)
		for(int j = 0 ; j < 4 ; j++)
			X(j+i*4);

	glm::mat4x4 transformationMatrix;
	for(int i = 0 ; i < 4 ; i++)
		for(int j = 0 ; j < 4 ; j++)
			transformationMatrix[i][j]=X(i*4+j,0);
	//because glsl use transposed matrices...
	modelMatrix=/*modelMatrix*/transpose(transformationMatrix);

#if 0
	std::cout<<"matrice A"<<std::endl;
	std::cout<<A<<std::endl;
	std::cout<<"matrice B"<<std::endl;
	std::cout<<B<<std::endl;

	std::cout<<"before"<<std::endl;
	points[0].print();
	std::cout<<"after"<<std::endl;
	res[0].print();
	std::cout<<"expected"<<std::endl;
	glm::vec4 resi = glm::vec4(points[0].x,points[0].y,points[0].z,1)*modelMatrix;
	std::cout<<resi.x<<" "<<resi.y<<" "<<resi.z<<" "<<resi.w<<std::endl;
#endif
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
