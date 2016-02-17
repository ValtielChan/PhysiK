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
public: // TODO : see if there is a way to set this protected
    // geometry data

    typedef struct
    {
        glm::vec3 tangent;
		glm::vec3 binormal;
    } Tangents;


    Material* material;
    bool isDoubleSided;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> instances_offsets;
    std::vector<Tangents> tangents;
    std::vector<GLuint> indices;

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

    GLuint vao; // TODO : this is supposed to be protected

protected:

    int nb_buffers;
    GLuint vbo[NB_BUFFERS];
    GLenum primitive_type;

public:

    Mesh();
    virtual ~Mesh();

    void initGL(bool isDynamic = false);
    void draw(Shader* shader = NULL, bool drawNormals = true, bool drawTexCoord = true, bool drawTangents = true);
    void destroyGL();

    glm::vec3* beginUpdateBuffer(int buffer);
    void endUpdateBuffer();

    /**
     * this class is intended to be used with the default GL_TRIANGLES primitive,
     * the methods mergeVertices, computeNormals, and computeTangents will probably have
     * unpredictable behavior with other primitive types.
     */
    void setPrimitiveType(GLenum type) {primitive_type = type;}

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
