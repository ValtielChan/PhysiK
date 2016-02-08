#include "meshbuilder.h"

void MeshBuilder::addPosition(float x, float y, float z)
{
    addPosition(glm::vec3(x, y, z));
}

void MeshBuilder::addPosition(const glm::vec3 &position)
{
    positions.push_back(position);
}

void MeshBuilder::addNormal(float x, float y, float z)
{
    addNormal(glm::vec3(x, y, z));
}

void MeshBuilder::addNormal(const glm::vec3 &normal)
{
    normals.push_back(normal);
}

void MeshBuilder::addTexCoord(float u, float v)
{
    addTexCoord(glm::vec2(u, v));
}

void MeshBuilder::addTexCoord(const glm::vec2 &texCoord)
{
    texCoords.push_back(texCoord);
}


void MeshBuilder::addVertex(const glm::vec3 &position, const glm::vec3 &normal)
{
    addPosition(position);
    addNormal(normal);
}

void MeshBuilder::addVertex(const glm::vec3 &position, const glm::vec2 &texCoord)
{
    addPosition(position);
    addTexCoord(texCoord);
}

void MeshBuilder::addVertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &texCoord)
{
    addVertex(position, normal);
    addTexCoord(texCoord);
}


void MeshBuilder::addTriangle(int i1, int i2, int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

void MeshBuilder::setMaterial(Material* myMaterial)
{
    material = myMaterial;
}
