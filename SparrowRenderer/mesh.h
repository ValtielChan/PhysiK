#ifndef MESH_H
#define MESH_H

#include "glew.h"
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Material;
class Shader;

struct Mesh
{
protected:

public: // TODO : see if there is a way to set this protected
    // geometry data

    typedef struct
    {
        glm::vec3 tangent;
		glm::vec3 binormal;
    } Tangents;


    Material* material;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> instances_offsets;
    std::vector<Tangents> tangents;
    std::vector<unsigned int> indices;

protected:

    // opengl

    enum {
        // required buffers
        POSITION_BUFFER,
        // indices buffers
        INDICES_BUFFER,
        // optionnal buffers :
        NORMAL_BUFFER, TEXCOORD_BUFFER, TANGENT_BUFFER,

        // instanciation buffer
        INSTANCE_BUFFER,

        NB_BUFFERS
    };

    GLuint vao;
    int nb_buffers;
    GLuint vbo[NB_BUFFERS];

public:
    Mesh();
    ~Mesh();

    void initGL(bool isDynamic = false);
    void draw(Shader* shader = NULL, bool drawNormals = true, bool drawTexCoord = true, bool drawTangents = true);
    void destroyGL();

    // merge same vertices
    void mergeVertices();
    bool operator() (const int& vertId1, const int& vertId2) const;
    // require positions and indices
    void computeNormals();
    // require normals and texCoord
    void computeTangents();

    bool hasNormals() const
    {
        return normals.size() != 0;
    }

    bool hasTexCoords() const
    {
        return texCoords.size() != 0;
    }

    bool hasTangents() const
    {
        return tangents.size() != 0;
    }

    bool hasInstances() const
    {
        return instances_offsets.size() != 0;
    }
};

#endif // MESH_H
