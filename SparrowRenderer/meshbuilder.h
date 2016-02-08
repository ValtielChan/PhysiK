#ifndef MESHBUILDER_H
#define MESHBUILDER_H

#include "mesh.h"

class MeshBuilder : public Mesh
{
public:
    void addTriangle(int i1, int i2, int i3);

    void addPosition(float x, float y, float z);
    void addPosition(const glm::vec3 &position);
    void addNormal(float x, float y, float z);
    void addNormal(const glm::vec3 &normal);
    void addTexCoord(float u, float v);
    void addTexCoord(const glm::vec2 &texCoord);

    void addVertex(const glm::vec3 &position, const glm::vec3 &normal);
    void addVertex(const glm::vec3 &position, const glm::vec2 &texCoord);
    void addVertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &texCoord);

    void setMaterial(Material* myMaterial);
};

#endif // MESHBUILDER_H
